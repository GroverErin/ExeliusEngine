#pragma once
#include "source/engine/gameobjectsystem/components/Component.h"
#include "source/resource/ResourceHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class AudioComponent
		: public Component
	{
		ResourceID m_audioSourceID;
		bool m_isMusic;
		int m_loopCount;

	public:
		DEFINE_COMPONENT(AudioComponent);

		AudioComponent(GameObject* pOwner)
			: Component(pOwner)
			, m_isMusic(false)
			, m_loopCount(0)
		{
			//
		}

		virtual bool Initialize() final override;
		virtual bool Initialize(const rapidjson::Value& jsonComponentData) final override;

		virtual void Destroy() final override;

		void Play();

		void Stop();

		bool IsPlaying();
	};
}