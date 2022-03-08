#pragma once
#include "source/engine/gameobjectsystem/components/Component.h"
#include "source/resource/ResourceHelpers.h"

#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class AnimationComponent
		: public Component
	{
		struct Animation
		{
			float m_animationFramerate = 1.0f;
			eastl::vector<StringIntern> m_frameIDs;
			bool m_doesLoop = false;
		};

		eastl::unordered_map<StringIntern, Animation> m_animations;

		ResourceID m_spriteSheetID;
		StringIntern m_defaultAnimationName;
		StringIntern m_currentAnimationName;
		StringIntern m_currentSpriteFrameName;
		size_t m_currentFrameIndex;
		float m_currentAnimationFramerate;

		float m_currentFrameTime;

	public:
		DEFINE_COMPONENT(AnimationComponent);

		AnimationComponent(GameObject* pOwner)
			: Component(pOwner)
			, m_currentFrameTime(0.0f)
			, m_currentFrameIndex(0)
			, m_currentAnimationFramerate(0.0f)
		{
			//
		}

		virtual bool Initialize() final override;
		virtual bool Initialize(const rapidjson::Value& jsonComponentData) final override;

		virtual void Destroy() final override;

		virtual void Render() const final override;

		virtual void Update() final override;

		void Play(const StringIntern& animationName);

		void Stop();

		bool IsPlaying();

	private:
		bool ParseSpritesheet(const rapidjson::Value& spritesheetData);

		bool ParseAnimations(const rapidjson::Value& animationData);
	};
}