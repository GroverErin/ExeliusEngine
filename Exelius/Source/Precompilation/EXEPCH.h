#pragma once

#ifdef EXE_PLATFORM_WINDOWS // This is not currently defined anywhere yet - will be added to the build system when windows code becomes a priority.
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <Source/Debugging/EngineLog.h>

#ifdef EXE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif