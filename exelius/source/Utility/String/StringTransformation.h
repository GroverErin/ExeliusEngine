#pragma once
#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	namespace String
	{
		void ToLower(eastl::string& stringToConvert);

		void ToUpper(eastl::string& stringToConvert);

		void ToFilepath(eastl::string& stringToConvert);
	}
}