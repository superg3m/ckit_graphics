#pragma once

#include "../../External_Libraries/ckit_types.h"

#if defined(PLATFORM_WINDOWS)
	#include "./Platforms/ckit_graphics_win32_native_window.c"
#elif defined(PLATFORM_LINUX)
	#include "./Platforms/ckit_graphics_linux_native_window.c"
#elif defined(PLATFORM_DARWIN)
	#include "./Platforms/ckit_graphics_darwin_native_window.c"
#endif