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

#include "SDL2\SDL.h"

#include "World.h"

namespace gpr460 {

	#ifdef __EMSCRIPTEN__
		typedef std::string string;
	#else
		typedef std::wstring string;
	#endif

	struct EngineState
	{
		SDL_Renderer* renderer;
		System* system;
		World* world;
		Uint32 frameStart;
		bool quit;
		int frame;
	};

	class System
	{
	public:
		static EngineState engine;

		virtual ~System() {}

		virtual void Init() = 0;

		virtual void Shutdown() = 0;

		virtual void ErrorMessage(const gpr460::string& message) = 0;

		virtual void LogToErrorFile(const gpr460::string& message) = 0;

		static System* Create();

		//static void CALLBACK OverlapComplete(DWORD errorCode, DWORD length, LPOVERLAPPED overlapped);
	};

}

#endif
