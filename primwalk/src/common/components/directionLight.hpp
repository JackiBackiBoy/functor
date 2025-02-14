#pragma once

// primwalk
#include "../../core.hpp"

// vendor
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace pw {
	struct PW_API DirectionLight {
		glm::vec3 direction = { 2.0f, 1.0f, 1.0f };
		glm::vec3 color = { 1.0f, 1.0f, 1.0f };
	};
}