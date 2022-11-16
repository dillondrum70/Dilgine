#ifndef SYSTEM_WIN32_H
#define SYSTEM_WIN32_H

#ifdef _WIN32
// THIS IS WINDOWS-ONLY
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <crtdbg.h>

#include "System_Common.h"

namespace gpr460
{
	//filenames
	const gpr460::string FILENAME_ERROR = L"GameErrors.txt";
	const gpr460::string FILENAME_LEVEL0 = L"level0.dat";

	//error constants
	const gpr460::string ERROR_PRESSED_K = L"Error: User pressed 'k'";

	const gpr460::string ERROR_CREATE_SDL_RENDERER_FAILED = L"Error: Creation of SDL_Renderer failed";
	const gpr460::string ERROR_MISSING_ENGINE_RENDERER = L"Error: Could not get SDL_Renderer* renderer on engine";

	const gpr460::string ERROR_CREATE_VULKAN_INSTANCE_FAILED = L"Error: Creation of Vulkan Instance failed";

	const gpr460::string ERROR_MISSING_GAMEOBJECT_REFERENCE = L"Error: Missing Reference to GameObject";
	const gpr460::string ERROR_MISSING_TRANSFORM_REFERENCE = L"Error: Missing Reference to Transform Component";
	const gpr460::string ERROR_MISSING_RECTCOLLIDER_REFERENCE = L"Error: Missing Reference to RectangularCollider Component";
	const gpr460::string ERROR_MISSING_RECTRENDERER_REFERENCE = L"Error: Missing Reference to RectangularRenderer Component";

	const gpr460::string ERROR_COMPONENT_EXISTS = L"Error: Component to be created already exists";

	const gpr460::string ERROR_GAMEOBJECT_OVERFLOW = L"Error: Too many game objects";
	const gpr460::string ERROR_COMPONENT_OVERFLOW = L"Error: Too many components";

	const gpr460::string ERROR_LEVEL_OPEN_FAIL = L"Level data file failed to open";

	class System_Win32 : public System
	{
		~System_Win32() override {}

		void Init() override;

		void Shutdown() override;

		void ErrorMessage(const gpr460::string& message) override;

		void LogToErrorFile(const gpr460::string& message) override;

		_CrtMemState memState;

		HANDLE errorFile;
	};

}
#endif

#endif