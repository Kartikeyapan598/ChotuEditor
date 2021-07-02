#pragma once

#if defined(_WIN64) || defined (_WIN32)
	#define CR_WINDOWS
	#define _CRTDBG_MAP_ALLOC
	
	#include <crtdbg.h>

#elif defined(__linux__)
	#define CR_LINUX
#elif defined CR_CNDR
	#define CR_FLAME
#else
	#error Platform Not Defined!
#endif
