#ifndef SYSTEM_EMSCRIPTEN_H
#define SYSTEM_EMSCRIPTEN_H

#include "System_Common.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

namespace gpr460 {

	//error constants
	const gpr460::string ERROR_PRESSED_K = "Error: User pressed k";
	const gpr460::string ERROR_CREATE_SDL_RENDERER_FAILED = "Error: Creation of SDL_Renderer failed";
	const gpr460::string ERROR_MISSING_GAMEOBJECT_REFERENCE = "Error: Missing Reference to GameObject";
	const gpr460::string ERROR_MISSING_RECTCOLLIDER_REFERENCE = "Error: Missing Reference to RectangularCollider";

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