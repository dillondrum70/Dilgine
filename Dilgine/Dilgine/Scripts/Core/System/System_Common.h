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

namespace gpr460 {

	const int DEFAULT_FRAME_RATE = 60;

	#ifdef __EMSCRIPTEN__
		typedef std::string string;
	#else
		typedef std::wstring string;
	#endif

	class System
	{
	public:
		virtual ~System() {}

		virtual void Init() = 0;

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
	};

	extern EngineState* engine;

}

#endif
