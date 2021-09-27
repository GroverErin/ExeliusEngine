#pragma once
#include "source/engine/gameobjectsystem/components/Component.h"
#include "source/resource/ResourceHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{

	class SpriteComponent
		: public Component
	{
		ResourceID m_spriteSheetID;
		StringIntern m_spriteID;

		float m_xOffset;
		float m_yOffset;
		float m_xScale;
		float m_yScale;

	public:
		DEFINE_COMPONENT(SpriteComponent);

		SpriteComponent(GameObject* pOwner)
			: Component(pOwner)
			, m_xOffset(0.0f)
			, m_yOffset(0.0f)
			, m_xScale(1.0f)
			, m_yScale(1.0f)
		{
			//
		}

		virtual ~SpriteComponent() = default;

		virtual bool Initialize(const rapidjson::Value& jsonComponentData) final override;

		virtual void Render() const final override;

		virtual void Destroy() final override;

	private:
		bool ParseSpritesheet(const rapidjson::Value& spritesheetData);

		bool ParseSprite(const rapidjson::Value& spriteData);
	};
}