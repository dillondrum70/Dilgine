#ifndef SYSTEM_COMMON_H
#define SYSTEM_COMMON_H

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <string>

namespace gpr460 {

	#ifdef _EMSCRIPTEN_
		typedef std::string string;
	#else
		typedef std::wstring string;
	#endif

	class System
	{
	public:
		void Init();

		void Shutdown();

		void ErrorMessage(const gpr460::string& message);

		void LogToErrorFile(const gpr460::string& message);

		//static void CALLBACK OverlapComplete(DWORD errorCode, DWORD length, LPOVERLAPPED overlapped);

	private:
		//static HANDLE errorFile;
		void* systemSubclass;
	};

}

#endif
