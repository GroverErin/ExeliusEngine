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
		// TODO:
		// Rule of 5
		~SFMLTexture();

		bool LoadFromMemory(const std::byte* pData, size_t dataSize);

		sf::Texture* GetSFMLTexture() const { return m_pSFMLTexture; }
	};
}