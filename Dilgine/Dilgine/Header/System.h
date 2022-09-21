#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef _EMSCRIPTEN_
	#include "Header/System_Emscripten.h"
#else
	#include "Header/System_Win32.h"
#endif

#endif