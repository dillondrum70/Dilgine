#ifndef VULKAN_CONTENT_H
#define VULKAN_CONTENT_H

#include "vulkan/vulkan.h"

struct SDL_Window;

namespace dVulkan
{
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	//Only use validation layers if not debugging, can just define this macro to turn them off
#ifndef NDEBUG
	const bool enableValidationLayers = false;
#else
	cosnt bool enableValidationLayers = true;
#endif

	void InitVulkan(SDL_Window* window);

	bool CheckValidationSupport();
}
#endif