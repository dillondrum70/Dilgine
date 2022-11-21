#ifndef VULKAN_CONTENT_H
#define VULKAN_CONTENT_H

#define DEBUGGING

#include <vector>
#include <optional>

#include "vulkan/vulkan.h"

struct SDL_Window;

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	//QueueFamilyIndices is complete if graphics family has a value (not null)
	bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;			//Minimum and maximum capabilities of swap chain
	std::vector<VkSurfaceFormatKHR> formats;		//Surface formats
	std::vector<VkPresentModeKHR> presentModes;		//Available presentation modes
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

	VkDebugUtilsMessengerEXT debugMessenger;

	//Physical graphics card Vulkan will use
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	//Logical device interface
	VkDevice logicalDevice;

	//Use vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue); to get device queue
	//Graphics queue, accepts command buffer to run on GPU
	VkQueue graphicsQueue;
	//Presentation queue
	VkQueue presentQueue;

	//Represents SDL window, Vulkan is platform agnostic and this allows it to interface with an abstract surface that can render images
	VkSurfaceKHR surface;

	//Swapchain object
	VkSwapchainKHR swapChain;

	//Images in swap chain that are referenced during rendering
	std::vector<VkImage> swapChainImages;

	//store swap chain image info
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkImageView> swapChainImageViews;

	//Validation layers to enable
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	//Device extensions to query GPU for support before choosing and later enable
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	//Only use validation layers if not debugging, can just define this macro to turn them off
#ifndef DEBUGGING
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	void CreateInstance(SDL_Window* window);				//Instance everything is done with
	void CreateDebugMessenger();							//Create messenger for printing validation layer errors
	void CreateSurface(SDL_Window* window);					//Platform agnostic representation of actual window that is drawn to
	void InitValidationLayers(VkInstanceCreateInfo* cInfo);	//Debugging symbols
	void ChoosePhysicalDevice();							//Choose which GPU to use, must support everything we need
	void CreateLogicalDevice();								//Create logical interface
	void CreateSwapChain(SDL_Window* window);				//Determine and create parameters for drawing
	void CreateImageViews();								//The view of an image, specifies how and what part of image to access

	//Check if validation layers are supported
	bool CheckValidationSupport();

	//Callback function for debug messaging
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	//Create debug messensger object
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	//Populate debug create info struct
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	//Destroy debug messenger
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	
	bool CheckDevice(VkPhysicalDevice device); //Check properties and supporting features of the passed GPU
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);//Check for extension support on a device

	//Find queue families supported by device
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	//Populate SwapChainSupportDetails
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	//Determine surface format
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

	//Determine best present mode
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

	//Choosing resolution of swap chain images
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, SDL_Window* window);
};

#endif