#include "EXEPCH.h"
#include "SFMLAudio.h"

#include <SFML/Audio.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	SFMLAudio::~SFMLAudio()
	{
		delete m_pSFMLSoundBuffer;
		delete m_pSFMLMusic;
		delete m_pSFMLSound;
		m_pSFMLSoundBuffer = nullptr;
		m_pSFMLMusic = nullptr;
		m_pSFMLSound = nullptr;
	}

	bool SFMLAudio::PlayAsSound(const std::byte* pData, size_t dataSize)
	{
		if (!m_pSFMLSoundBuffer)
			m_pSFMLSoundBuffer = new sf::SoundBuffer();

		if (!m_pSFMLSoundBuffer->loadFromMemory(pData, dataSize))
		{
			Log log("GraphicsInterface");
			log.Warn("Failed to load an SFML Sound Buffer from memory.");

			delete m_pSFMLSoundBuffer;
			m_pSFMLSoundBuffer = nullptr;

			return false;
		}

		if (!m_pSFMLSound)
			m_pSFMLSound = new sf::Sound();

		m_pSFMLSound->setBuffer(*m_pSFMLSoundBuffer);

		m_pSFMLSound->play();

		return true;
	}

	bool SFMLAudio::PlayAsMusic(const std::byte* pData, size_t dataSize)
	{
		if (!m_pSFMLMusic)
			m_pSFMLMusic = new sf::Music();

		if (!m_pSFMLMusic->openFromMemory(pData, dataSize))
		{
			Log log("GraphicsInterface");
			log.Warn("Failed to load an SFML Music from memory.");

			delete m_pSFMLMusic;
			m_pSFMLMusic = nullptr;

			return false;
		}

		m_pSFMLMusic->play();

		return true;
	}
}