#ifndef VULKAN_CORE_H
#define VULKAN_CORE_H
//Supporting structures and includes necessary for Vulkan

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
//Force depth to be defined between 0 and 1
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define GLM_ENABLE_EXPERIMENTAL

///////// TinyObjLoader Defines ////////////
#define TINYOBJLOADER_IMPLEMENTATION

#include "vulkan/vulkan.h"
//SDL included in cpp
#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
//stb included in cpp
//tinyobjloader included in cpp

const int MAX_FRAMES_IN_FLIGHT = 2;

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

	//Need overload for using Vertex in a hashmap to make sure we don't load the same vertex more than once
	bool operator==(const Vertex& other) const
	{
		return position == other.position && color == other.color && texCoord == other.texCoord;
	}
};

namespace std
{
	template<> struct hash<Vertex>
	{
		size_t operator()(Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.position) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

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

#endif