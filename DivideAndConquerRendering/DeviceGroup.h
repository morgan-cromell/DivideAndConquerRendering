#pragma once
#include <vector>
#include <memory>
#include <vulkan\vulkan.hpp>
#include "VkGroups.h"
class DeviceContext;
class DeviceGroup {
private:
	std::vector<DeviceContext> devices;

public:
	DeviceGroup();
	~DeviceGroup();
	void addDevice(vk::Instance& instance, vk::PhysicalDevice physicalDevice);
	void addDevice(vk::Instance& instance, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR& surface);
	

	vkGroups::PipelineGroup createGraphicsPipeline(vk::PipelineCache pipelineCache, const vk::GraphicsPipelineCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator);
	vkGroups::DescriptorPoolGroup createDescriptorPool(const vk::DescriptorPoolCreateInfo& poolInfo, vk::Optional<const vk::AllocationCallbacks> allocator);
};