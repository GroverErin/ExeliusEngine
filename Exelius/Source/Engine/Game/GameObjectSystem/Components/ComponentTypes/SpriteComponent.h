#pragma once
#include "Source/Engine/Game/GameObjectSystem/Components/Component.h"
#include "Source/OS/Platform/PlatformForwardDeclarations.h"

namespace Exelius
{
	class SpritesheetResource;

	FORWARD_DECLARE(Sprite);

	class SpriteComponent
		: public Component
	{

		SpritesheetResource* m_pSpritesheetResource;
		Sprite* m_pSprite;
		float m_xOffset;
		float m_yOffset;

	public:
		DEFINE_COMPONENT(SpriteComponent);

		SpriteComponent()
			: m_pSpritesheetResource(nullptr)
			, m_pSprite(nullptr)
			, m_xOffset(0.0f)
			, m_yOffset(0.0f)
		{
			//
		}

		virtual ~SpriteComponent();

		virtual bool Initialize(GameObject* pOwner) final override;
		virtual bool Initialize(GameObject* pOwner, const rapidjson::Value& jsonComponentData) final override;

		virtual void Render() const final override;

		virtual void Destroy() final override;

	private:

		bool ParseSpritesheet(const rapidjson::Value& spritesheetData);

		bool ParseSprite(const rapidjson::Value& spriteData);
	};
}