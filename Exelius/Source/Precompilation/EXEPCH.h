#pragma once

#ifdef EXE_PLATFORM_WINDOWS // This is not currently defined anywhere yet - will be added to the build system when windows code becomes a priority.
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

//#include <memory>
#include <EASTL/memory.h>
//#include <utility>
#include <EASTL/utility.h>
//#include <algorithm>
#include <EASTL/algorithm.h>
//#include <functional>
#include <EASTL/functional.h>

//#include <string>
#include <EASTL/string.h>
//#include <array>
#include <EASTL/array.h>
//#include <vector>
#include <EASTL/vector.h>
//#include <unordered_map>
#include <EASTL/unordered_map.h>
//#include <unordered_set>
#include <EASTL/unordered_set.h>
#include <EASTL/list.h>
#include <EASTL/chrono.h>

#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>

#include <Source/Debugging/EngineLog.h>
#include <Source/Utility/Containers/Vector2.h>

#ifdef EXE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif