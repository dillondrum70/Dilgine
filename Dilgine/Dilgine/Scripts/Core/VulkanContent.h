#ifndef VULKAN_CONTENT_H
#define VULKAN_CONTENT_H

#include <vector>
#include <optional>

#include "vulkan/vulkan.h"

struct SDL_Window;

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;

	//QueueFamilyIndices is complete if graphics family has a value (not null)
	bool isComplete() { return graphicsFamily.has_value(); }
};

class EngineVulkan
{
public:
	//Vulkan Instance
	VkInstance vInstance;

	//Initialize Vulkan
	void InitVulkan(SDL_Window* window);

	//Destroy relevant items
	void Cleanup();

private:

	//Physical graphics card Vulkan will use
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	//Logical device interface
	VkDevice device;

	//Graphics queue, accepts command buffer to run on GPU
	VkQueue graphicsQueue;
	//Use vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue); to get device queue

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	//Only use validation layers if not debugging, can just define this macro to turn them off
#ifndef NDEBUG
	const bool enableValidationLayers = false;
#else
	cosnt bool enableValidationLayers = true;
#endif

	void CreateInstance(SDL_Window* window);			//Instance everything is done with
	void InitValidationLayers(VkInstanceCreateInfo* cInfo);	//Debugging symbols
	void ChoosePhysicalDevice();	//Choose which GPU to use, must support everything we need
	void CreateLogicalDevice();		//Create logical interface

	//Check if validation layers are supported
	bool CheckValidationSupport();

	//Check properties and supporting features of the passed GPU
	bool CheckDevice(VkPhysicalDevice device);

	//Find queue families supported by device
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
};

#endif