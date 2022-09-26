#include "System_Emscripten.h"

#ifdef __EMSCRIPTEN__

void gpr460::System::Init()
{

}

void gpr460::System::Shutdown()
{

}

void gpr460::System::ErrorMessage(const gpr460::string& message)
{
	gpr460::string pass = "alert('" + message + "')";
	emscripten_run_script(pass.c_str());
}

void gpr460::System::LogToErrorFile(const gpr460::string& message)
{
	gpr460::string pass = "console.log('" + message + "')";
	emscripten_run_script(pass.c_str());
}

#endif