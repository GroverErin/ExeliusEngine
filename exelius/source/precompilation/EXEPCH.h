#pragma once

#ifdef EXE_PLATFORM_WINDOWS // This is not currently defined anywhere yet - will be added to the build system when windows code becomes a priority.
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#define EXELIUS_USE_EASTL_ALLOCATOR

#ifndef EXELIUS_USE_EASTL_ALLOCATOR
	#define EASTL_USER_CONFIG_HEADER <source/utility/thirdparty/CustomEASTLConfig.h>
#endif

#include <EASTL/memory.h>
#include <EASTL/utility.h>
#include <EASTL/algorithm.h>
#include <EASTL/functional.h>

#include <EASTL/string.h>
#include <EASTL/array.h>
#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>
#include <EASTL/unordered_set.h>
#include <EASTL/list.h>
#include <EASTL/chrono.h>

#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>

#include <source/utility/containers/Vector2.h>
#include <source/utility/generic/Macros.h>
#include <source/debug/Log.h>
#include <source/os/memory/TraceAllocator.h>

#ifdef EXE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
