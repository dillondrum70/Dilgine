#ifndef SYSTEM_EMSCRIPTEN_H
#define SYSTEM_EMSCRIPTEN_H

#include "System_Common.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

namespace gpr460 {

	//error constants
	const gpr460::string ERROR_PRESSED_K = "Error: User pressed k";

	class System_Emscripten : public System
	{
		~System_Emscripten() override {}

		void Init() override;

		void Shutdown() override;

		void ErrorMessage(const gpr460::string& message) override;

		void LogToErrorFile(const gpr460::string& message) override;
	};
}

#endif
#endif