#include "EXEPCH.h"

#include "Source/Utility/String/StringTransformation.h"

#include <locale>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	namespace String
	{
		void ToLower(eastl::string& stringToConvert)
		{
			eastl::transform(stringToConvert.begin(), stringToConvert.end(), stringToConvert.begin(), [](unsigned char c) -> unsigned char
				{
					return (unsigned char)std::tolower(c);
				});
		}
		
		void ToUpper(eastl::string& stringToConvert)
		{
			eastl::transform(stringToConvert.begin(), stringToConvert.end(), stringToConvert.begin(), [](unsigned char c) -> unsigned char
				{
					return (unsigned char)std::toupper(c);
				});
		}
		
		void ToFilepath(eastl::string& stringToConvert)
		{
			ToLower(stringToConvert);
			eastl::replace(stringToConvert.begin(), stringToConvert.end(), '\\', '/');
		}
	}
}