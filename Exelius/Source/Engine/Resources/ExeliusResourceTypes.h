#pragma once
#include "Source/Resource/ResourceHelpers.h"

namespace Exelius
{
	namespace ResourceType
	{
		static constexpr Type kTextFile = kInvalid + 1;
		static constexpr Type kTexture = kTextFile + 1;

		static constexpr Type kLastEngineResourceTypeValue = kTexture;
	}
}