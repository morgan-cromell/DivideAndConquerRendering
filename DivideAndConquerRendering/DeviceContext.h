#pragma once

#include <vulkan\vulkan.hpp>
#include <SDL.h>
#include <memory>

class DeviceGroup;
class DeviceContext {
public:
	enum class DEVICE_MODE {WINDOW, HEADLESS};
	struct Swapchain {
		vk::Extent2D extent;
		vk::Format imageFormat;
		std::vector<vk::ImageView> imageViews;
		std::vector<vk::Image> images;
		std::vector<vk::Framebuffer> framebuffers;
		vk::SwapchainKHR swapchain;
	};
private:

	DeviceGroup* deviceGroup;
	DEVICE_MODE mode;

	vk::SurfaceKHR surface;
	vk::RenderPass presentRenderPass;
	
	vk::PhysicalDevice physicalDevice;
	vk::Device device;
	vk::RenderPass renderPass;
	vk::CommandPool commandPool;
	vk::Queue graphicsQueue;

	Swapchain swapchain;

	struct QueueFamilyIndices
	{
		int graphicsFamily = -1;
		int presentFamily = -1;
		bool headless = false;
		bool isComplete()
		{
			return graphicsFamily >= 0 && (presentFamily >= 0 || headless);
		}
	};

	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	
public:
	DeviceContext(DeviceGroup* group, vk::Instance& instance, vk::PhysicalDevice physicalDevice);
	DeviceContext(DeviceGroup* group, vk::Instance& instance, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR& surface);
	~DeviceContext();

	Swapchain& getSwapchain();

	vk::Device& getDevice();
private:
	void createDevice(vk::Instance& instance);
	void createRenderPass();
	void createPresentRenderPass();
	void createCommandBuffers();


	QueueFamilyIndices findQueueFamilies();

	//Swapchain functions
	void createSwapchain();
	void createSwapchainImageViews();
	SwapChainSupportDetails querySwapChainSupport();
	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes);
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);


};