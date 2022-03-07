#include "EXEPCH.h"
#include "source/engine/gameobjectsystem/components/componenttypes/AudioComponent.h"
#include "source/resource/ResourceHandle.h"
#include "source/engine/resources/resourcetypes/AudioResource.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    bool AudioComponent::Initialize()
    {
        EXE_ASSERT(m_pOwner);
        return true;
    }

    bool AudioComponent::Initialize(const rapidjson::Value& jsonComponentData)
    {
        EXE_ASSERT(m_pOwner);

        auto audioFile = jsonComponentData.FindMember("file");

        EXE_ASSERT(audioFile != jsonComponentData.MemberEnd());
        EXE_ASSERT(audioFile->value.IsString());

        m_audioSourceID = audioFile->value.GetString();

        ResourceHandle audioResource(m_audioSourceID);
        audioResource.QueueLoad(true);
        audioResource.LockResource();

        auto isMusic = jsonComponentData.FindMember("isMusic");

        EXE_ASSERT(isMusic != jsonComponentData.MemberEnd());
        EXE_ASSERT(isMusic->value.IsBool());

        m_isMusic = isMusic->value.GetBool();

        auto loopCount = jsonComponentData.FindMember("loopCount");

        EXE_ASSERT(loopCount != jsonComponentData.MemberEnd());
        EXE_ASSERT(loopCount->value.IsInt());

        m_loopCount = loopCount->value.GetInt();

        return true;
    }

    void AudioComponent::Destroy()
    {
        ResourceHandle audioResource(m_audioSourceID);
        audioResource.UnlockResource();
    }

    void AudioComponent::Play()
    {
        ResourceHandle audioResource(m_audioSourceID);

        auto* pAudio = audioResource.GetAs<AudioResource>();

        if (!pAudio)
            return;

        if (m_isMusic)
            pAudio->PlayAsMusic();
        else
            pAudio->PlayAsSound();
    }

    void AudioComponent::Stop()
    {
        ResourceHandle audioResource(m_audioSourceID);

        auto* pAudio = audioResource.GetAs<AudioResource>();

        if (!pAudio)
            return;

        pAudio->Stop();
    }

    bool AudioComponent::IsPlaying()
    {
        return false;
    }
}