#include "EXEPCH.h"
#include "Source/OS/Platform/SFML/SFMLTexture.h"

#include <SFML/Graphics/Texture.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	SFMLTexture::~SFMLTexture()
	{
		delete m_pSFMLTexture;
		m_pSFMLTexture = nullptr;
	}

	bool SFMLTexture::LoadFromMemory(const std::byte* pData, size_t dataSize)
	{
		EXE_ASSERT(pData);
		EXE_ASSERT(dataSize > 0);
		Log log("GraphicsInterface");

		m_pSFMLTexture = new sf::Texture();
		EXE_ASSERT(m_pSFMLTexture);

		if (!m_pSFMLTexture->loadFromMemory(pData, dataSize))
		{
			log.Warn("Failed to load an SFML texture from memory.");

			delete m_pSFMLTexture;
			m_pSFMLTexture = nullptr;
			return false;
		}

		return true;
	}
}