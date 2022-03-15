#include "EXEPCH.h"
#include "AudioResource.h"
#include "source/os/interface/Audio.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    AudioResource::AudioResource(const ResourceID& id)
        : Resource(id)
        , m_pAudio(nullptr)
    {
        //
    }

    AudioResource::~AudioResource()
    {
    }

    Resource::LoadResult AudioResource::Load(eastl::vector<std::byte>&& data)
    {
        m_data = data;

        delete m_pAudio;
        m_pAudio = new Audio();

        return LoadResult::kKeptRawData;

        /*delete m_pAudio;
        m_pAudio = new Audio();
        if (m_pAudio->LoadFromMemory(data.data(), data.size()))
            return LoadResult::kKeptRawData;
        return LoadResult::kFailed;*/
    }

    void AudioResource::Unload()
    {
        delete m_pAudio;
        m_pAudio = nullptr;
    }

    void AudioResource::PlayAsMusic()
    {
        EXE_ASSERT(m_data.size() > 0);

        m_pAudio->PlayAsMusic(m_data.data(), m_data.size());
    }

    void AudioResource::PlayAsSound()
    {
        EXE_ASSERT(m_data.size() > 0);

        m_pAudio->PlayAsSound(m_data.data(), m_data.size());
    }

    void AudioResource::Stop()
    {
    }
}