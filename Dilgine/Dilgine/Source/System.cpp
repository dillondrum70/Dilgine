#include "Header/System.h"

void gpr460::System::Init()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	if (!AllocConsole())
	{
		ErrorMessage(L"Console Failed to Open");
	}

	freopen("CONOUT$", "w", stdout);

	errorFile = INVALID_HANDLE_VALUE;
}

void gpr460::System::Shutdown()
{
	if (errorFile != INVALID_HANDLE_VALUE)
	{
		//CloseHandle(errorFile);
	}
	
}

void gpr460::System::ErrorMessage(const gpr460::string& message)
{
	MessageBox(NULL, message.c_str(), L"Error", MB_OK | MB_ICONERROR);
}

void gpr460::System::LogToErrorFile(const gpr460::string& message)
{
	if (errorFile == INVALID_HANDLE_VALUE)
	{
		errorFile = CreateFileW(ERROR_FILENAME.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	}

	
	//LPOVERLAPPED overlap = LPOVERLAPPED();
	//LPOVERLAPPED_COMPLETION_ROUTINE routine = LPOVERLAPPED_COMPLETION_ROUTINE();
	//bool success = WriteFileEx(errorFile, message.c_str(), sizeof(message), overlap, routine);
	
	DWORD bytesWritten = 0;
	bool success = WriteFile(errorFile, message.c_str(), message.size() * sizeof(wchar_t), &bytesWritten, NULL);

	if (!success)
	{
		ErrorMessage(L"Error WriteFile Failed");
	}
}