#include "System_Emscripten.h"

#ifdef __EMSCRIPTEN__
void gpr460::System::Init()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
}

void gpr460::System::Shutdown()
{

}

void gpr460::System::ErrorMessage(const gpr460::string& message)
{
	//emscripten_run_script("alert(message)");
}

void gpr460::System::LogToErrorFile(const gpr460::string& message)
{
	//emscripten_run_script("console.log(message)");
}
#endif