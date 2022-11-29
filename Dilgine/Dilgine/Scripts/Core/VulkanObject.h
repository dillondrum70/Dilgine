#ifndef VULKAN_OBJECT_H
#define VULKAN_OBJECT_H

#include "VulkanCore.h"

//A collection of all the unique aspects behind a model in vulkan, this way models can be moved and changed individually
class VulkanObject
{
public:
	VkDescriptorSetLayout descriptorSetLayout;	//Layout for descriptor set (model-view projection matrix descriptor)
	VkPipelineLayout pipelineLayout;	//Defines how uniforms are passed to shaders
	VkPipeline graphicsPipeline;		//Graphics pipeline object

	//These all point to structures and objects stored in EngineVulkan, only store one instance of a model and one instance of a texture
	VkImage* textureImage = nullptr;	//Vulkan image data type for textures
	VkDeviceMemory* textureImageMemory = nullptr;	//Memory location for image
	VkImageView* textureImageView = nullptr;	//ImageView through which we access the image
	VkSampler* textureSampler = nullptr;	//Samples values from texture

	std::vector<Vertex>* vertices = nullptr;	//This object's vertices
	std::vector<uint32_t>* indices = nullptr;	//This object's indices from verticies
	VkBuffer* vertexBuffer = nullptr;	//Stores list of each individual vertex in a mesh, no repeats
	VkDeviceMemory* vertexBufferMemory = nullptr;	//Memory for vertex buffer, sometimes we may want to destroy a buffer, but keep the memory allocated to construct something new in the same location
	VkBuffer* indexBuffer = nullptr;	//Stores list of indices in vertex array that define triangles
	VkDeviceMemory* indexBufferMemory = nullptr;	//Memory for index buffer

	//Memory buffers and memory to handle uniform buffers
	//std::vector<VkBuffer> uniformBuffers;
	//std::vector<VkDeviceMemory> uniformBuffersMemory;
	//std::vector<void*> uniformBuffersMapped;	//This is where uniform buffers are written to from World
	VkBuffer uniformBuffers[MAX_FRAMES_IN_FLIGHT];
	VkDeviceMemory uniformBuffersMemory[MAX_FRAMES_IN_FLIGHT];
	void* uniformBuffersMapped[MAX_FRAMES_IN_FLIGHT];

	VkDescriptorPool descriptorPool;	//Where descriptors are taken from
	std::vector<VkDescriptorSet> descriptorSets;

	void CreateObject(std::string modelFilePath, std::string textureFilePath);

	void CreateDescriptorSetLayout();	//Binding model-view projection matrix
	void CreateGraphicsPipeline();	//Handles rendering steps like vertex, geometry, and fragment shaders
	void CreateUniformBuffer();	//Create all uniform buffers, i.e. model-view projection matrix buffer
	void CreateDescriptorPool();	//Pool that descriptor sets are allocated to
	void CreateDescriptorSets();	//Set of descriptors for a model

	void DestroyObject();
};

#endif