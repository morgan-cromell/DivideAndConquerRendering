#include "Technique.h"
#include "Renderer.h"
#include "DeviceContext.h"

std::map<Material*, vkGroups::TechniqueGroup> Technique::loadedTechniques;
Technique::Technique(DeviceContext* deviceContext, Material * m, RenderState * r)
{

	material = m;
	renderState = r;


	//input assembly
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.pVertexInputState = &m->getVertexinputInfo();	//Specific for device? Don't think so. Should be same for the same shaders. So will only differ when we change technique.
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = r->getViewportState();
	pipelineInfo.pRasterizationState = r->getRasterizer();
	pipelineInfo.pMultisampleState = r->getMultisampling();
	pipelineInfo.pDynamicState = nullptr;
	pipelineInfo.pDepthStencilState = r->getDepthStencil();
	pipelineInfo.pColorBlendState = r->getColorBlending();
	pipelineInfo.subpass = 0;
	pipelineInfo.renderPass = deviceContext->getRenderpass(m->getRenderPassType());

	std::vector<vk::PipelineShaderStageCreateInfo> stages;
	m->getShaderStages(deviceContext, stages);

	pipelineInfo.stageCount = stages.size();
	pipelineInfo.pStages = stages.data();

	descriptorPool = deviceContext->getDevice().createDescriptorPool(material->getDescriptorPoolInfo());
	descriptorSetLayout = deviceContext->getDevice().createDescriptorSetLayout(material->getDescriptorSetLayoutInfo());

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &descriptorSetLayout;
	descriptorSets = deviceContext->getDevice().allocateDescriptorSets(allocInfo);


	vk::DescriptorSetLayout layouts[] = { descriptorSetLayout };
	vk::PipelineLayoutCreateInfo createInfo = material->getPipelineLayoutInfo();
	createInfo.setLayoutCount = 1;
	createInfo.pSetLayouts = layouts;
	pipelineLayout = deviceContext->getDevice().createPipelineLayout(createInfo);

	
	pipelineInfo.layout = pipelineLayout;
	

	pipeline = deviceContext->getDevice().createGraphicsPipeline({}, pipelineInfo);

}

Technique * Technique::createOrGetTechnique(DeviceContext * deviceContext, Material * m, RenderState* r)
{
	if (loadedTechniques.find(m) != loadedTechniques.end())
	{
		vkGroups::TechniqueGroup& techGroup = loadedTechniques.at(m);
		if (techGroup.sets.find(deviceContext) != techGroup.sets.end())
			return techGroup.sets.at(deviceContext);
		else
		{
			Technique* tech = new Technique(deviceContext, m, r);
			techGroup.sets.insert(std::make_pair(deviceContext, tech));
			return tech;
		}
			
	}
	else
	{
		loadedTechniques[m];
		vkGroups::TechniqueGroup& techGroup = loadedTechniques.at(m);
		Technique* tech = new Technique(deviceContext, m, r);
		techGroup.sets.insert(std::make_pair(deviceContext, tech));
		return tech;
	}
}

Technique::~Technique()
{
	
}

void Technique::bind(vk::CommandBuffer& commandBuffer)
{
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, descriptorSets, {});
}

std::vector<vk::DescriptorSet> Technique::getDescriptionSets()
{
	return descriptorSets;
}
