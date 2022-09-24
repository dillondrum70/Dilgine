#include "System_Emscripten.h"

#ifdef __EMSCRIPTEN__

void gpr460::System::Init()
{

}

void gpr460::System::Shutdown()
{

}

EM_JS(void, error_message, (gpr460::string mess), {
	alert(String(mess));
	});

void gpr460::System::ErrorMessage(const gpr460::string& message)
{
	error_message(message.c_str());
	//emscripten_run_script("alert(message)");
	//EM_ASM({ console.log($) }, message);
}

EM_JS(void, log_error_message, (gpr460::string mess), {
	console.log(String(mess));
	});

void gpr460::System::LogToErrorFile(const gpr460::string& message)
{
	log_error_message(message.c_str());
	//emscripten_run_script("console.log(message)");
	//EM_ASM({ console.log($) }, message);
}

#endif