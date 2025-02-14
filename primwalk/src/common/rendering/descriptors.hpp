#pragma once

// primwalk
#include "../../core.hpp"

// vendor
#include <vulkan/vulkan.h>

// std
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

namespace pw {
	struct UniformBufferObject;
	struct SwapChainSupportDetails;
	struct QueueFamilyIndices;
	class GraphicsDevice_Vulkan;

	// ******** Descriptor Set Layout ********
	class PW_API DescriptorSetLayout {
	public:
		// Nested builder class
		class Builder {
		public:
			Builder(GraphicsDevice_Vulkan& device) : m_Device(device) {};
			~Builder() = default;

			Builder& addBinding(
			uint32_t binding,
			VkDescriptorType descriptorType,
			VkShaderStageFlags stageFlags,
			uint32_t count = 1,
			VkDescriptorBindingFlags bindingFlags = 0);
			Builder& setLayoutFlags(VkDescriptorSetLayoutCreateFlags layoutFlags);
			std::unique_ptr<DescriptorSetLayout> build() const;

		private:
			GraphicsDevice_Vulkan& m_Device;
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;
			std::unordered_map<uint32_t, VkDescriptorBindingFlags> m_BindingFlags;
			VkDescriptorSetLayoutCreateFlags m_LayoutFlags = 0;
		};

		DescriptorSetLayout(
			GraphicsDevice_Vulkan& device,
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings,
			std::unordered_map<uint32_t, VkDescriptorBindingFlags> bindingFlags,
			VkDescriptorSetLayoutCreateFlags layoutFlags);

		~DescriptorSetLayout();

		// Getters
		inline VkDescriptorSetLayout getDescriptorSetLayout() const { return m_DescriptorSetLayout; }

	private:
		GraphicsDevice_Vulkan& m_Device;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;
		std::unordered_map<uint32_t, VkDescriptorBindingFlags> m_BindingFlags;
		VkDescriptorSetLayoutCreateFlags m_LayoutFlags;

		friend class DescriptorWriter;
	};

	// ******** Descriptor Pool ********
	class PW_API DescriptorPool {
	public:
		// Nested builder class
		class Builder {
		public:
			Builder(GraphicsDevice_Vulkan& device) : m_Device(device) {};
			~Builder() {};

			Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
			Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
			Builder& setMaxSets(uint32_t count);
			std::unique_ptr<DescriptorPool> build() const;

		private:
			GraphicsDevice_Vulkan& m_Device;
			uint32_t m_MaxSets = 100;
			VkDescriptorPoolCreateFlags m_PoolFlags = 0;
			std::vector<VkDescriptorPoolSize> m_PoolSizes;
		};

		DescriptorPool(
		GraphicsDevice_Vulkan& device,
		uint32_t maxSets,
		VkDescriptorPoolCreateFlags poolFlags,
		const std::vector<VkDescriptorPoolSize>& poolSizes);
		~DescriptorPool();

		bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

	private:
		GraphicsDevice_Vulkan& m_Device;
		VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;

		friend class DescriptorWriter;
	};

	// ******** Descriptor Writer ********
	class PW_API DescriptorWriter {
	public:
		DescriptorWriter(DescriptorSetLayout& setLayout, DescriptorPool& pool) :
			m_SetLayout(setLayout), m_Pool(pool) {};
		~DescriptorWriter() = default;

		DescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
		DescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo, uint32_t index = 0);

		bool build(VkDescriptorSet& set);
		void overwrite(VkDescriptorSet& set);

	private:
		std::vector<VkWriteDescriptorSet> m_Writes;
		DescriptorSetLayout& m_SetLayout;
		DescriptorPool& m_Pool;
	};
}

