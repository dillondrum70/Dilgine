#ifndef SYSTEM_COMMON_H
#define SYSTEM_COMMON_H

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

#include "SDL2\SDL.h"
#include "SDL2/SDL_vulkan.h"
#include "vulkan/vulkan.h"

#include "World.h"
#include "VulkanContent.h"
#include "StackAllocator.h"
#include "Input.h"

namespace gpr460 {

	const int DEFAULT_FRAME_RATE = 60;

	#ifdef __EMSCRIPTEN__
		typedef std::string string;
	#else
		typedef std::wstring string;
	#endif

		//Same name as error message prefaces with CODE_
	//enum ErrorCodes
	//{
	//	CODE_ERROR_ERROR_LOGGING_FAILED = 0,	//May also throw 0 if window not initialized, happens before error system is set up
	//	CODE_ERROR_CREATE_SDL_RENDERER_FAILED,
	//	CODE_ERROR_MISSING_ENGINE_RENDERER,
	//	CODE_ERROR_CREATE_VULKAN_INSTANCE_FAILED,
	//	CODE_ERROR_CREATE_DEBUG_MESSENGER_FAILED,
	//	CODE_ERROR_VALIDATION_LAYER_DNE,
	//	CODE_ERROR_NO_GPU_VULKAN_SUPPORT,
	//	CODE_ERROR_NO_CAPABLE_GPU,
	//	CODE_ERROR_QUEUE_FAMILY_INCOMPLETE,
	//	CODE_ERROR_EXTENSIONS_UNSUPPORTED,
	//	CODE_ERROR_SWAP_CHAIN_INADEQUATE,
	//	CODE_ERROR_CREATE_LOGICAL_DEVICE_FAILED,
	//	CODE_ERROR_CREATE_SURFACE_FAILED,
	//	CODE_ERROR_CREATE_SWAPCHAIN_FAILED,
	//	CODE_ERROR_CREATE_IMAGE_VIEW_FAILED,
	//	CODE_ERROR_OPEN_FILE_VULKAN_FAILED,
	//	CODE_ERROR_CREATE_SHADER_MODULE_FAILED,
	//	CODE_ERROR_CREATE_PIPELINE_LAYOUT_FAILED,
	//	CODE_ERROR_CREATE_RENDER_PASS_FAILED,
	//	CODE_ERROR_CREATE_PIPELINE_FAILED,
	//	CODE_ERROR_CREATE_FRAMEBUFFER_FAILED,
	//	CODE_ERROR_ALLOCATE_COMMAND_BUFFER_FAILED,
	//	CODE_ERROR_BEGIN_COMMAND_BUFFER_FAILED,
	//	CODE_ERROR_RECORD_COMMAND_BUFFER_FAILED,
	//	CODE_ERROR_CREATE_SEMAPHORES_AND_FENCES_FAILED,
	//	CODE_ERROR_SUBMIT_DRAW_FAILED,
	//	CODE_ERROR_ACQUIRE_SWAPCHAIN_IMAGE_FAILED,
	//	CODE_ERROR_PRESENT_SWAPCHAIN_IMAGE_FAILED,
	//	CODE_ERROR_CREATE_BUFFER_FAILED,
	//	CODE_ERROR_FIND_MEMORY_TYPE_FAILED,
	//	CODE_ERROR_ALLOCATE_BUFFER_FAILED,
	//	CODE_ERROR_CREATE_DESCRIPTOR_SET_LAYOUT_FAILED,
	//	CODE_ERROR_CREATE_DESCRIPTOR_POOL_FAILED,
	//	CODE_ERROR_ALLOCATE_DESCRIPTOR_SETS_FAILED,
	//	CODE_ERROR_LOAD_TEXTURE_IMAGE_FAILED,
	//	CODE_ERROR_MISSING_GAMEOBJECT_REFERENCE,
	//	CODE_ERROR_MISSING_TRANSFORM_REFERENCE,
	//	CODE_ERROR_MISSING_RECTCOLLIDER_REFERENCE,
	//	CODE_ERROR_MISSING_RECTRENDERER_REFERENCE,
	//	CODE_ERROR_GAMEOBJECT_OVERFLOW,
	//	CODE_ERROR_COMPONENT_OVERFLOW,
	//	CODE_ERROR_LEVEL_OPEN_FAIL
	//};

	class System
	{
	public:
		virtual ~System() {}

		virtual void Init() = 0;

		virtual void TrackMemory() = 0;
		virtual void LogMemory() = 0;

		virtual void Shutdown() = 0;

		virtual void ErrorMessage(const gpr460::string& message) = 0;
		virtual void LogToErrorFile(const gpr460::string& message) = 0;

		static System* Create();

		//static void CALLBACK OverlapComplete(DWORD errorCode, DWORD length, LPOVERLAPPED overlapped);
	};

	struct EngineState
	{
		EngineState() { renderer = nullptr; system = nullptr; world = nullptr; frameStart = 0; quit = false; frame = DEFAULT_FRAME_RATE; }

		SDL_Renderer* renderer;
		EngineVulkan vulkanEngine;
		System* system;
		World* world;
		Uint32 frameStart;
		bool quit;
		int frame;
		StackAllocator stack;
		Input input;
	};

	extern EngineState* engine;

}

#endif
