#ifndef SYSTEM_EMSCRIPTEN_H
#define SYSTEM_EMSCRIPTEN_H

#include "System_Common.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

namespace gpr460 {

	//error constants
	const gpr460::string ERROR_PRESSED_K = "Error: User pressed k";
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