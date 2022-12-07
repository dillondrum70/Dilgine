#include "System_Emscripten.h"

#ifdef __EMSCRIPTEN__

void gpr460::System_Emscripten::Init()
{

}

void gpr460::System_Emscripten::Shutdown()
{

}

void gpr460::System_Emscripten::TrackMemory()
{

}

void gpr460::System_Emscripten::LogMemory()
{

}

void gpr460::System_Emscripten::ErrorMessage(const gpr460::string& message)
{
	gpr460::string pass = "alert('" + message + "')";
	emscripten_run_script(pass.c_str());
}

void gpr460::System_Emscripten::LogToErrorFile(const gpr460::string& message)
{
	gpr460::string pass = "console.log('" + message + "')";
	emscripten_run_script(pass.c_str());
}

gpr460::System* gpr460::System::Create()
{
	return DBG_NEW System_Emscripten();
}

#endif