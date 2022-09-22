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
	//typedef std::wstring string;

	const gpr460::string ERROR_FILENAME = L"GameErrors.txt";

	//error constants
	const gpr460::string ERROR_PRESSED_K = L"Do not press K!!!";
	const gpr460::string LOG_ERROR_PRESSED_K = L"Error: User pressed 'k'";

	class SystemWin32
	{
	public:
		HANDLE errorFile;
	};

	/*class System
	{
	public:
		void Init();

		void Shutdown();

		void ErrorMessage(const gpr460::string& message);

		void LogToErrorFile(const gpr460::string& message);

		static void CALLBACK OverlapComplete(DWORD errorCode, DWORD length, LPOVERLAPPED overlapped);

	private:
		HANDLE errorFile;
	};*/
}
#endif

#endif