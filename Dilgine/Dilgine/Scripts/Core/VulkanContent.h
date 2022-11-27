#ifndef VULKAN_CONTENT_H
#define VULKAN_CONTENT_H

#define DEBUGGING

#include <vector>
#include <optional>
#include <fstream>
#include <array>

////////////// GLM Defines /////////////////
#define GLM_FORCE_RADIANS
//Saves us from worrying about data alignment most of the time
//Does not work on nested structures
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES	

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

////////////// STB Defines /////////////////
#define STB_IMAGE_IMPLEMENTATION

#include "vulkan/vulkan.h"
#include "SDL2/SDL.h"
#include "glm/glm.hpp"

const int MAX_FRAMES_IN_FLIGHT = 2;

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

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoord;

	//Defines how to pass Vertex data format
	static VkVertexInputBindingDescription GetBindingDescription()
	{
		//Define rate to load data from memory, bytes  betweend data entries
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;	//Move to next data entry after each vertex

		return bindingDescription;
	}

	//How to handle vertex input
	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
	{
		//Two  descriptions tell us how to extract vertex attribute from vertex data in binding description, one for position, one for color
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
		attributeDescriptions[0].binding = 0;							//Which binding the per-vertex data comes from
		attributeDescriptions[0].location = 0;							//location directive of input in vertex shader
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;		//Type of data, R32G32 is same as vec2, 2 x 32 bit values
		attributeDescriptions[0].offset = offsetof(Vertex, position);	//Number of bytes since start of per-vertex data to read from

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
};

//Vertex data, position-color, and texture coordinates, don't need repeats
//Positive Directions: Left, Forward, Up
//Z is the vertical axis, Y is the front/back axis, z is left/right
const std::vector<Vertex> vertices = {
		//Top Face
		{{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},	//0 - Right, Back, Top
		{{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},	//1 - Left, Back Top
		{{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},		//2 - Left, Front, Top
		{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},	//3 - Right, Front, Top

		//Bottom Face
		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},	//4 - Right, Back, Bottom
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},	//5 - Left, Back, Bottom
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},	//6 - Left, Front, Bottom
		{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},	//7 - Right, Front, Bottom

		//Front Face
		{{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},		//8 - Left, Front, Top
		{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},	//9 - Right, Front, Top
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},	//10 - Left, Font, Bottom
		{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},	//11 - Right, Front, Bottom

		//Back Face
		{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},	//12 - Left, Back, Top
		{{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},	//13 - Right, Back, Top
		{{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},	//14 - Left, Back, Bottom
		{{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},	//15 - Right, Back, Bottom

		//Left Face
		{{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},	//16 - Left, Back, Top
		{{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},		//17 - Left, Front, Top
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},	//18 - Left, Back, Bottom
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},	//19 - Left, Front, Bottom

		//Right Face
		{{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},	//20 - Right, Back, Top
		{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},	//21 - Right, Front, Top
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},	//22 - Right, Back, Bottom
		{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}	//23 - Right, Front, Bottom
};

//Index buffer, allows us to take predefined vertices and order them as a standard vertex buffer would be
//Counter-clockwise order, 3 indices per triangle, indices map directly to indices defined in verticies vector
const std::vector<uint16_t> cubeIndices = {
	//Top face
	0, 1, 2,
	2, 3, 0,

	//Bottom Face
	6, 5, 4,
	4, 7, 6,

	//Front Face
	10, 9, 8,
	10, 11, 9,

	//Back Face
	12, 13, 14,
	13, 15, 14,

	//Left Face
	16, 18, 17,
	18, 19, 17,

	//Right Face
	21, 22, 20,
	21, 23, 22
};

//To translate C++ to GLM structures, data must be aligned
//This is aligned by default
//N = 4 bytes, 1 x 32 bit float
//Scalar - aligned by N (4 bytes)
//Vec2 - aligned by 2N
//vec3/vec4 - aligned by 4N
//mat4 - same as vec4
//Nested Structure - base alignment of members rounded up to multiple of 16
//Use "alignas(# of bytes)" to align
//i.e.
//glm::vec2 foo
//alignas(16) glm::mat4 bar
//Safe to always align

//Model-View Projection
struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

class EngineVulkan
{
public:
	
	VkInstance vInstance;		//Vulkan Instance
	VkDevice logicalDevice;		//Logical device interface

	//Use vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue); to get device queue
	VkQueue graphicsQueue;								//Graphics queue, accepts command buffer to run on GPU
	VkQueue presentQueue;								//Presentation queue

	VkSwapchainKHR swapChain;		//Swapchain object
	VkExtent2D swapChainExtent;		//Extents of camera

	std::vector<VkCommandBuffer> commandBuffers;	//Commands to change anything are submitted to this buffer

	//Used to synchronize GPU processes, i.e. can't run render until we have image
	std::vector <VkSemaphore> imageAvailableSemaphores;
	std::vector <VkSemaphore> renderFinishedSemaphores;
	std::vector <VkFence> inFlightFences;		//Pauses CPU until GPU finishes specified process

	VkBuffer vertexBuffer;	//Stores list of each individual vertex in a mesh, no repeats
	VkDeviceMemory vertexBufferMemory;	//Memory for vertex buffer, sometimes we may want to destroy a buffer, but keep the memory allocated to construct something new in the same location
	VkBuffer indexBuffer;	//Stores list of indices in vertex array that define triangles
	VkDeviceMemory indexBufferMemory;	//Memory for index buffer

	//Memory buffers and memory to handle uniform buffers
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkDescriptorPool descriptorPool;	//Where descriptors are taken from
	std::vector<VkDescriptorSet> descriptorSets;

	bool framebufferResized = false;

	uint32_t currentFrame = 0;

	//Initialize Vulkan
	void InitVulkan(SDL_Window* window);

	//Destroy relevant items
	void Cleanup();

	//Sumbit command to command buffer
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	//Recreate swapchain in the case that it is invalidated, i.e. resized
	void RecreateSwapChain(SDL_Window* window);

	//Callback for when framebuffer is resized, data will be an SDL_Window* and sdlEvent will be WINDOW_EVENT type with WINDOWEVENT_RESIZED event
	static int FramebufferResizeCallback(void* data, SDL_Event* sdlEvent);

private:

	//Holds index array of every model
	std::vector<const std::vector<uint16_t>*> allIndices;

	VkDebugUtilsMessengerEXT debugMessenger;

	
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;	//Physical graphics card Vulkan will use

	VkSurfaceKHR surface;	//Represents SDL window, Vulkan is platform agnostic and this allows it to interface with an abstract surface that can render images
	
	
	std::vector<VkImage> swapChainImages;	//Images in swap chain that are referenced during rendering
	VkFormat swapChainImageFormat;			//store swap chain image info
	std::vector<VkImageView> swapChainImageViews;

	VkRenderPass renderPass;			//The actual render pass object
	VkDescriptorSetLayout descriptorSetLayout;	//Layout for descriptor set (model-view projection matrix descriptor)
	VkPipelineLayout pipelineLayout;	//Defines how uniforms are passed to shaders
	VkPipeline graphicsPipeline;		//Graphics pipeline object

	std::vector<VkFramebuffer> swapChainFramebuffers;	//Frames are submitted to this buffer, how and when they are drawn is defined elsewhere

	VkCommandPool commandPool;	//Manage memory used to store command buffers

	VkImage depthImage;	//Image of depth information
	VkDeviceMemory depthImageMemory;	//Memory location of depth image
	VkImageView depthImageView;	//Image view to access depth image

	VkImage textureImage;	//Vulkan image data type for textures
	VkDeviceMemory textureImageMemory;	//Memory location for image
	VkImageView textureImageView;	//ImageView through which we access the image
	VkSampler textureSampler;	//Samples values from texture

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
	void CreateDescriptorSetLayout();						//Binding model-view projection matrix
	void CreateGraphicsPipeline();							//Handles rendering steps like vertex, geometry, and fragment shaders
	void CreateFramebuffers();								//Render pass attachments are used here, references VkImageView objects
	void CreateCommandPool();								//Manage command buffer memory and allocate command buffers from here
	void CreateDepthResources();							//Objects needed to acquire depth in view (Objects that are further away are not rendered on top of closer objects)
	void CreateTextureImage();								//Image used for texturing object
	void CreateTextureImageView();							//Images are accessed through ImageViews
	void CreateTextureSampler();							//Sample VkImage for colors
	void CreateVertexBuffer();								//Buffer of vertices that define mesh
	void CreateIndexBuffers(std::vector<const std::vector<uint16_t>*> indicesList);	//Buffer of indices corresponding to vertices arrary, 3-tuples of verticies make triangles
	void CreateUniformBuffers();							//Create all uniform buffers, i.e. model-view projection matrix buffer
	void CreateDescriptorPool();							//Pool that descriptor sets are allocated to
	void CreateDescriptorSets();							//
	void CreateCommandBuffers();							//All operations that are to be done are stored here
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

	//Cleanup last swapchain before recreation
	void CleanupSwapChain();

	//GPUs offer different types of memory to allocate from
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	//Create a memory buffer
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	//Copy data in one buffer to another
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	//Create image, allocate, and bind memory
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, 
		VkImage& image, VkDeviceMemory& imageMemory);

	//Reorder and rexecute command buffer, begin command buffer
	VkCommandBuffer BeginSingleTimeCommands();

	//End command buffer
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	//Put image in correct layout
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	//Copies data buffer to a VkImage
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	//Create an image view to access an image
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	//Used to find depth formats supported by the GPU, can be found to find any formats that support given features
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	//Find depth format supported by GPU
	VkFormat FindDepthFormat();

	//Lets us know if depth format contains stencil component
	bool HasStencilComponent(VkFormat format);
};

#endif