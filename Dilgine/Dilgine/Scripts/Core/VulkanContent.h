#ifndef VULKAN_CONTENT_H
#define VULKAN_CONTENT_H

#define DEBUGGING

#include <vector>
#include <optional>
#include <fstream>

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
	
	VkInstance vInstance;		//Vulkan Instance
	VkDevice logicalDevice;		//Logical device interface

	//Use vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue); to get device queue
	VkQueue graphicsQueue;								//Graphics queue, accepts command buffer to run on GPU
	VkQueue presentQueue;								//Presentation queue

	VkSwapchainKHR swapChain;				//Swapchain object

	VkCommandBuffer commandBuffer;	//Commands to change anything are submitted to this buffer

	//Used to synchronize GPU processes, i.e. can't run render until we have image
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;		//Pauses CPU until GPU finishes specified process

	//Initialize Vulkan
	void InitVulkan(SDL_Window* window);

	//Destroy relevant items
	void Cleanup();

	//Sumbit command to command buffer
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

private:

	VkDebugUtilsMessengerEXT debugMessenger;

	
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;	//Physical graphics card Vulkan will use

	VkSurfaceKHR surface;	//Represents SDL window, Vulkan is platform agnostic and this allows it to interface with an abstract surface that can render images
	
	
	std::vector<VkImage> swapChainImages;	//Images in swap chain that are referenced during rendering
	VkFormat swapChainImageFormat;			//store swap chain image info
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	VkRenderPass renderPass;			//The actual render pass object
	VkPipelineLayout pipelineLayout;	//Defines how uniforms are passed to shaders
	VkPipeline graphicsPipeline;		//Graphics pipeline object

	std::vector<VkFramebuffer> swapChainFramebuffers;	//Frames are submitted to this buffer, how and when they are drawn is defined elsewhere

	VkCommandPool commandPool;	//Manage memory used to store command buffers

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
	void CreateRenderPass();								//Handles information regarding rendering
	void CreateGraphicsPipeline();							//Handles rendering steps like vertex, geometry, and fragment shaders
	void CreateFramebuffers();								//Render pass attachments are used here, references VkImageView objects
	void CreateCommandPool();								//Manage command buffer memory and allocate command buffers from here
	void CreateCommandBuffer();								//All operations that are to be done are stored here
	void CreateSyncObjects();								//Create Semaphores and Fences
	
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

	//Read bytes from a file and return a byte array
	static std::vector<char> ReadFile(const std::string& filename);

	//Creates a shader module from given code
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
};

#endif