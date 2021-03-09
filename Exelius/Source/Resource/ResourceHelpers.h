#pragma once
#include "Source/Utility/String/StringIntern.h"

namespace Exelius
{
	using ResourceID = StringIntern;

	namespace ResourceType
	{
		using Type = uint32_t;

		static constexpr Type kInvalid = 0;
	}

	enum class ResourceLoadStatus
	{
		kInvalid,	// Initial state, this is set by the constructor.
		kLoading,	// Set when the file begins the loading process.
		kLoaded,	// Set when the loader thread has completed loading this resource.
		kUnloading,	// Set when the loader thread begins unloading this resource.
		kUnloaded,	// Set when the loader thread finished unloading this resource.
	};
}