#pragma once
#include <vulkan\vulkan.hpp>
#include "vkGroups.h"
#include "DeviceGroup.h"
#include "DeviceContext.h"
class Shader;

class Material
{
public:
	
private:
	vkGroups::ShaderGroup vertexShader;
	vkGroups::ShaderGroup fragmentShader;

	std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutbindings;
	std::map<vk::DescriptorType, int> poolSizes;
	std::vector<vk::DescriptorPoolSize> poolSizesValues;
	std::map<vk::ShaderStageFlagBits, size_t> pushConstants;
	std::vector<vk::PushConstantRange> pushConstantsValues;

	DeviceContext::RENDERPASS_TYPE type;

public:
	Material(std::string vertexFilename, std::string fragmentFilename, DeviceContext::RENDERPASS_TYPE renderPassType);
	~Material();

	vkGroups::ShaderGroup getVertexShader();
	vkGroups::ShaderGroup getFragmentShader();

	void getShaderStages(DeviceContext* devicem, std::vector<vk::PipelineShaderStageCreateInfo>& stages);

	vk::DescriptorPoolCreateInfo getDescriptorPoolInfo();
	vk::PipelineLayoutCreateInfo getPipelineLayoutInfo();
	vk::DescriptorSetLayoutCreateInfo getDescriptorSetLayoutInfo();
	vk::PipelineVertexInputStateCreateInfo getVertexinputInfo();
	DeviceContext::RENDERPASS_TYPE getRenderPassType();

protected:
	void addBinding(int binding, vk::DescriptorType type, vk::ShaderStageFlags stageFlags);
	void setPushConstant(vk::ShaderStageFlagBits shaderFlag, size_t size);

	std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
};