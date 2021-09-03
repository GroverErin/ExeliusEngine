#pragma once
#include "Source/Resource/ResourceHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	namespace ResourceType
	{
		static constexpr Type kTextFile = kInvalid + 1;
		static constexpr Type kTexture = kTextFile + 1;
		static constexpr Type kSpritesheet = kTexture + 1;

		static constexpr Type kLastEngineResourceTypeValue = kSpritesheet;
	}
}