#pragma once
#include <cstddef>

namespace Exelius
{
	class SFMLTexture
	{
	public:
		bool LoadFromMemory([[maybe_unused]] const std::byte* pData, [[maybe_unused]] size_t dataSize) { return true; }
	};
}