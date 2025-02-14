// primwalk
#include "shader.hpp"

// std
#include <fstream>
#include <stdexcept>

namespace pw {

	// ------ Vulkan ------
	std::vector<char> Shader_Vulkan::readFile(const std::string& path) {
		std::string truePath = BASE_DIR + path;
		std::ifstream file(truePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("VULKAN SHADER ERROR: Failed to open shader file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}
}
