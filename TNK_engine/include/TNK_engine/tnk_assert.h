#pragma once

#include "log.h"

#ifdef TNK_PLATFORM_WINDOWS
	#define TNK_BREAK __debugbreak();

#elif TNK_PLATFORM_MACOS
	#define TNK_BREAK __builtin_debugtrap();

#else TNK_PLATFORM_LINUX
	#define TNK_BREAK __builtin_trap();

#endif


#ifdef TNK_CONFIG_DEBUG
#define TNK_ASSERT(x, msg) if((x)) {} else { TNK_FATAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", #x, msg, __FILE__, __LINE__); TNK_BREAK}
#else
#define TNK_ASSERT(x, msg) (void)0
#endif