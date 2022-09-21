#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef __EMSCRIPTEN__
	#include "System_Emscripten.h"
#else
	#include "System_Win32.h"
#endif

#endif