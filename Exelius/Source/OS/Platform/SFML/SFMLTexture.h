#pragma once
#include <cstddef>

namespace sf
{
	class Texture;
}

namespace Exelius
{
	class SFMLTexture
	{
		sf::Texture* m_pSFMLTexture;
	public:
		bool LoadFromMemory(const std::byte* pData, size_t dataSize);

		sf::Texture* GetSFMLTexture() { return m_pSFMLTexture; }
	};
}