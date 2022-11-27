#ifndef SYSTEM_EMSCRIPTEN_H
#define SYSTEM_EMSCRIPTEN_H

#include "System_Common.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

namespace gpr460 {

	//error constants
	const gpr460::string ERROR_ERROR_LOGGING_FAILED = L"Error: Error logging failed";
	const gpr460::string FILENAME_LEVEL0 = "level0.dat";

	const gpr460::string ERROR_MISSING_ENGINE_RENDERER = "Error: Could not get SDL_Renderer* renderer on engine";
	const gpr460::string ERROR_CREATE_SDL_RENDERER_FAILED = "Error: Creation of SDL_Renderer failed";

	const gpr460::string ERROR_CREATE_VULKAN_INSTANCE_FAILED = "Error: Creation of Vulkan Instance failed";
	const gpr460::string ERROR_CREATE_DEBUG_MESSENGER_FAILED = "Error: Creation of Vulkan Debug Messenger failed";
	const gpr460::string ERROR_VALIDATION_LAYER_DNE = "Error: One or more validation layers were not found";
	const gpr460::string ERROR_NO_GPU_VULKAN_SUPPORT = "Error: Failed to find GPUs with Vulkan support";
	const gpr460::string ERROR_NO_CAPABLE_GPU = "Error: Failed to find capable GPU";
	const gpr460::string ERROR_QUEUE_FAMILY_INCOMPLETE = "Error: Queue families incomplete";
	const gpr460::string ERROR_EXTENSIONS_UNSUPPORTED = "Error: One or more extensions are not supported";
	const gpr460::string ERROR_SWAP_CHAIN_INADEQUATE = "Error: Swap chain is inadequate";
	const gpr460::string ERROR_CREATE_LOGICAL_DEVICE_FAILED = "Error: Creation of Vulkan logical device failed";
	const gpr460::string ERROR_CREATE_SURFACE_FAILED = "Error: Creation of Vulkan surface failed";
	const gpr460::string ERROR_CREATE_SWAPCHAIN_FAILED = "Error: Creation of Vulkan swapchain failed";
	const gpr460::string ERROR_CREATE_IMAGE_VIEW_FAILED = "Error: Creation of Vulkan image view failed";
	const gpr460::string ERROR_OPEN_FILE_VULKAN_FAILED = "Error: Failed to open a file for Vulkan";
	const gpr460::string ERROR_CREATE_SHADER_MODULE_FAILED = "Error: Failed to create shader module";
	const gpr460::string ERROR_CREATE_PIPELINE_LAYOUT_FAILED = "Error: Failed to create pipeline layout";
	const gpr460::string ERROR_CREATE_RENDER_PASS_FAILED = "Error: Failed to create render pass";
	const gpr460::string ERROR_CREATE_PIPELINE_FAILED = "Error: Failed to create graphics pipeline";
	const gpr460::string ERROR_CREATE_FRAMEBUFFER_FAILED = "Error: Failed to create frame buffer";
	const gpr460::string ERROR_CREATE_COMMAND_POOL_FAILED = "Error: Failed to create command pool";
	const gpr460::string ERROR_ALLOCATE_COMMAND_BUFFER_FAILED = "Error: Failed to allocate command buffers";
	const gpr460::string ERROR_BEGIN_COMMAND_BUFFER_FAILED = "Error: Failed to begin command buffer";
	const gpr460::string ERROR_RECORD_COMMAND_BUFFER_FAILED = "Error: Failed to record command buffer";
	const gpr460::string ERROR_CREATE_SEMAPHORES_AND_FENCES_FAILED = "Error: Failed to create semaphores and fences";
	const gpr460::string ERROR_SUBMIT_DRAW_FAILED = "Error: Failed to submit draw command buffer";
	const gpr460::string ERROR_ACQUIRE_SWAPCHAIN_IMAGE_FAILED = "Error: Failed to acuire image";
	const gpr460::string ERROR_CREATE_BUFFER_FAILED = "Error: Failed to create buffer";
	const gpr460::string ERROR_FIND_MEMORY_TYPE_FAILED = "Error: Failed to find memory type";
	const gpr460::string ERROR_ALLOCATE_BUFFER_FAILED = "Error: Failed to allocate buffer memory";
	const gpr460::string ERROR_CREATE_DESCRIPTOR_SET_LAYOUT_FAILED = "Error: Failed to create descriptor set layout";
	const gpr460::string ERROR_CREATE_DESCRIPTOR_POOL_FAILED = "Error: Failed to create descriptor pool";
	const gpr460::string ERROR_ALLOCATE_DESCRIPTOR_SETS_FAILED = "Error: Failed to allocate descriptor sets";
	const gpr460::string ERROR_LOAD_TEXTURE_IMAGE_FAILED = "Error: Failed to load texture image";
	const gpr460::string ERROR_CREATE_TEXTURE_IMAGE_FAILED = "Error: Failed to create texture image";
	const gpr460::string ERROR_ALLOCATE_TEXTURE_IMAGE_FAILED = "Error: Failed to allocate texture image memory";
	const gpr460::string ERROR_UNSUPPORTED_LAYOUT_TRANSITION_FAILED = "Error: Unsupported layout transition";
	const gpr460::string ERROR_CREATE_TEXTURE_IMAGE_VIEW_FAILED = "Error: Failed to create texture image view";
	const gpr460::string ERROR_CREATE_TEXTURE_SAMPLER_FAILED = "Error: Failed to create texture sampler";
	const gpr460::string ERROR_FIND_SUPPORTED_FORMAT_FAILED = "Error: Failed to find supported format";

	const gpr460::string ERROR_MISSING_GAMEOBJECT_REFERENCE = "Error: Missing Reference to GameObject";
	const gpr460::string ERROR_MISSING_TRANSFORM_REFERENCE = "Error: Missing Reference to Transform Component";
	const gpr460::string ERROR_MISSING_RECTCOLLIDER_REFERENCE = "Error: Missing Reference to RectangularCollider";
	const gpr460::string ERROR_MISSING_RECTRENDERER_REFERENCE = "Error: Missing Reference to RectangularRenderer";

	const gpr460::string ERROR_COMPONENT_EXISTS = "Error: Component to be created already exists";

	const gpr460::string ERROR_GAMEOBJECT_OVERFLOW = "Error: Too many game objects";
	const gpr460::string ERROR_COMPONENT_OVERFLOW = "Error: Too many components";

	const gpr460::string ERROR_LEVEL_OPEN_FAIL = "Level data file failed to open";

	class System_Emscripten : public System
	{
		~System_Emscripten() override {}

		void Init() override;

		void Shutdown() override;

		void ErrorMessage(const gpr460::string& message) override;

		void LogToErrorFile(const gpr460::string& message) override;
	};
}

#endif
#endif