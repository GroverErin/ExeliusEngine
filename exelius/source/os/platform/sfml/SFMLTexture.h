#pragma once
#include <cstddef>

/// <summary>
/// SFML namespace, used only in SFML specific code within Exelius.
/// </summary>
namespace sf
{
	class Texture;
}

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
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