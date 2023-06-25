#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in float fragTexIndex;
layout(location = 2) in vec4 fragColor;
layout(location = 3) flat in uint fragTexCoordIndex;

layout(set = 2, binding = 0) uniform sampler2D vGlobalTextures[];

layout(location = 0) out vec4 outColor;

float median(vec3 msd) {
  return max(min(msd.r, msd.g), min(max(msd.r, msd.g), msd.b));
}

float screenPxRange() {
  vec2 unitRange = vec2(2.0) / vec2(textureSize(vGlobalTextures[int(fragTexIndex)], 0));
  vec2 screenTexSize = vec2(1.0) / fwidth(fragTexCoord);
  return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main() {
  float threshold = 0.5;
  vec3 msd = texture(vGlobalTextures[int(fragTexIndex)], fragTexCoord).rgb;
  float sd = median(msd);
  float screenPxDistance = screenPxRange() * (sd - threshold);
  float opacity = clamp(screenPxDistance + threshold, 0.0, 1.0);

  outColor = fragColor * vec4(1.0, 1.0, 1.0, opacity);
}