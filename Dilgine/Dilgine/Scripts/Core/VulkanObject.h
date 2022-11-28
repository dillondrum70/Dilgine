#ifndef VULKAN_OBJECT_H
#define VULKAN_OBJECT_H

#include "VulkanCore.h"

//A collection of all the unique aspects behind a model in vulkan, this way models can be moved and changed individually
struct VulkanObject
{
	VkDescriptorSetLayout descriptorSetLayout;	//Layout for descriptor set (model-view projection matrix descriptor)
	VkPipelineLayout pipelineLayout;	//Defines how uniforms are passed to shaders
	VkPipeline graphicsPipeline;		//Graphics pipeline object

	std::vector<Vertex> vertices;	//This object's vertices
	std::vector<uint32_t> indices;	//This object's indices from verticies
	VkBuffer vertexBuffer;	//Stores list of each individual vertex in a mesh, no repeats
	VkDeviceMemory vertexBufferMemory;	//Memory for vertex buffer, sometimes we may want to destroy a buffer, but keep the memory allocated to construct something new in the same location
	VkBuffer indexBuffer;	//Stores list of indices in vertex array that define triangles
	VkDeviceMemory indexBufferMemory;	//Memory for index buffer

	//Memory buffers and memory to handle uniform buffers
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;	//This is where uniform buffers are written to from World

	VkDescriptorPool descriptorPool;	//Where descriptors are taken from
	std::vector<VkDescriptorSet> descriptorSets;
};

#endif