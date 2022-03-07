#pragma once
#include <cstddef>

/// <summary>
/// SFML namespace, used only in SFML specific code within Exelius.
/// </summary>
namespace sf
{
	class Music;
	class SoundBuffer;
	class Sound;
}

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class SFMLAudio
	{
		sf::Music* m_pSFMLMusic;
		sf::SoundBuffer* m_pSFMLSoundBuffer;
		sf::Sound* m_pSFMLSound;
	public:
		// TODO:
		// Rule of 5
		~SFMLAudio();

		bool PlayAsSound(const std::byte* pData, size_t dataSize);

		bool PlayAsMusic(const std::byte* pData, size_t dataSize);

		sf::Sound* GetSFMLSound() const { return m_pSFMLSound; }
		sf::Music* GetSFMLMusic() const { return m_pSFMLMusic; }
	};
}