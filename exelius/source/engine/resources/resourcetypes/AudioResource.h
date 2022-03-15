#pragma once
#include "source/resource/Resource.h"
#include "source/os/platform/PlatformForwardDeclarations.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE_AUDIO(Audio);

	class AudioResource
		: public Resource
	{
		Audio* m_pAudio;
		eastl::vector<std::byte> m_data;
	public:
		AudioResource(const ResourceID& id);
		AudioResource(const AudioResource&) = delete;
		AudioResource(AudioResource&&) = delete;
		AudioResource& operator=(const AudioResource&) = delete;
		virtual ~AudioResource() final override;

		virtual LoadResult Load(eastl::vector<std::byte>&& data) final override;
		virtual void Unload() final override;

		void PlayAsMusic();

		void PlayAsSound();

		void Stop();

		Audio* GetAudio() const { return m_pAudio; }
	};
}