#pragma once
#include "Source/Engine/Game/GameObjectSystem/Components/Component.h"

namespace Exelius
{
	class SpritesheetResource;

	class SpriteComponent
		: public Component
	{

		SpritesheetResource* m_pSpritesheetResource;

	public:
		DEFINE_COMPONENT(SpriteComponent);

		SpriteComponent()
			: m_pSpritesheetResource(nullptr)
		{
			//
		}

		virtual ~SpriteComponent();

		virtual bool Initialize(GameObject* pOwner) final override;
		virtual bool Initialize(GameObject* pOwner, const rapidjson::Value& jsonComponentData) final override;

		virtual void Render() const final override;
	};
}