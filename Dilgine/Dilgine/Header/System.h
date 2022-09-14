#ifndef SYSTEM_H
#define SYSTEM_H

// THIS IS WINDOWS-ONLY
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include "SDL2/SDL.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <string>

namespace gpr460
{
	typedef std::wstring string;

	const gpr460::string ERROR_FILENAME = L"GameErrors.txt";

	class System
	{
	public:
		void Init();

		void Shutdown();

		void ErrorMessage(const gpr460::string& message);

		void LogToErrorFile(const gpr460::string& message);

	private:
		HANDLE errorFile;
	};
}

#endif