#ifndef SYSTEM_EMSCRIPTEN_H
#define SYSTEM_EMSCRIPTEN_H

#include "System_Common.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>


namespace gpr460 {

	//error constants
	const gpr460::string ERROR_PRESSED_K = "Do not press K!!!";
	const gpr460::string LOG_ERROR_PRESSED_K = "Error: User pressed 'k'";

	class SystemEmscripten
	{

	};

}
#endif
#endif