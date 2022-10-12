#include "System_Win32.h"

#ifdef _WIN32
void gpr460::System_Win32::Init()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtMemCheckpoint(&memState);

	if (!AllocConsole())
	{
		ErrorMessage(L"Console Failed to Open");
	}

	freopen("CONOUT$", "w", stdout);

	errorFile = INVALID_HANDLE_VALUE;

	errorFile = CreateFileW(FILENAME_ERROR.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
}

void gpr460::System_Win32::Shutdown()
{
	if (errorFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(errorFile);
	}

	_CrtMemDumpAllObjectsSince(&memState);
}

void gpr460::System_Win32::ErrorMessage(const gpr460::string& message)
{
	MessageBox(NULL, message.c_str(), L"Error", MB_OK | MB_ICONERROR);
}

void gpr460::System_Win32::LogToErrorFile(const gpr460::string& message)
{
	if (errorFile == INVALID_HANDLE_VALUE)
	{
		errorFile = CreateFileW(FILENAME_ERROR.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

		//not asynchronous
		//errorFile = CreateFileW(ERROR_FILENAME.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	OVERLAPPED overlap = OVERLAPPED();
	overlap.Offset = SetFilePointer(errorFile, 0, 0, FILE_END); //finds position at end of file, number of bytes to read past before writing
	bool success = WriteFileEx(errorFile, (message + L"\n").c_str(), (message + L"\n").size() * sizeof(wchar_t), &overlap, /*OverlapComplete*/ nullptr);

	//not asynchronous
	//DWORD bytesWritten = 0;
	//bool success = WriteFile(errorFile, (message + L"\n").c_str(), (message + L"\n").size() * sizeof(wchar_t), &bytesWritten, NULL);

	if (!success)
	{
		ErrorMessage(L"Error WriteFile Failed");
	}
}

gpr460::System* gpr460::System::Create()
{
	return DBG_NEW System_Win32();
}

/*void CALLBACK gpr460::System::OverlapComplete(DWORD errorCode, DWORD length, LPOVERLAPPED overlapped)
{

}*/
#endif