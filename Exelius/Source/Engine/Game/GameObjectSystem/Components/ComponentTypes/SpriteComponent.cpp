#include "EXEPCH.h"
#include "SpriteComponent.h"
#include "Source/Engine/Resources/ResourceRetrieval.h"
#include "Source/OS/Interface/Graphics/Texture.h"
#include "Source/OS/Interface/Graphics/Sprite.h"

#include "Source/Engine/Application.h"
#include "Source/Engine/Game/GameObjectSystem/GameObject.h"
#include "Source/Engine/Game/GameObjectSystem/Components/ComponentTypes/TransformComponent.h"

namespace Exelius
{
	SpriteComponent::~SpriteComponent()
	{
	}

	bool SpriteComponent::Initialize(GameObject* pOwner, const rapidjson::Value& jsonComponentData)
	{
		EXE_ASSERT(pOwner);
		m_pOwner = pOwner;

		// Get the spritesheet associated with this sprite.
		auto spritesheetData = jsonComponentData.FindMember("Spritesheet");

		if (spritesheetData == jsonComponentData.MemberEnd())
		{
			EXELOG_ENGINE_WARN("Initialization for SpriteComponent failed: No Spritesheet data found.");
			return false;
		}

		EXE_ASSERT(spritesheetData->name.IsString());

		if (!ParseSpritesheet(spritesheetData->value))
			return false;

		// Now we should have a spritesheet, so get the sprite.
		// SpriteComponents should only have a single sprite,
		// so looping is not necessesary.
		auto spriteData = jsonComponentData.FindMember("Sprite");

		if (spriteData == jsonComponentData.MemberEnd())
		{
			EXELOG_ENGINE_WARN("Initialization for SpriteComponent failed: No Sprite data found.");
			return false;
		}

		// TODO: Maybe Unnecessary Assert
		EXE_ASSERT(spriteData->name.IsString());

		if (!ParseSprite(spriteData->value))
		{
			// Release the spritesheet
			Destroy();
			return false;
		}

		return true;
	}

	void SpriteComponent::Render() const
	{
		EXE_ASSERT(m_pOwner);

		// Get the transform component sibling.
		auto transformComponent = m_pOwner->GetComponent<TransformComponent>();

		auto* pSheet = m_spriteSheet.GetAs<SpritesheetResource>();

		if (!pSheet)
			return;

		if (transformComponent.IsValid())
		{
			pSheet->GetSprite(m_spriteID)->SetPosition(transformComponent->GetX() + m_xOffset, transformComponent->GetY() + m_yOffset);
			pSheet->GetSprite(m_spriteID)->SetScale(m_xScale, m_yScale);
		}

		pSheet->GetSprite(m_spriteID)->Render();
	}

	void SpriteComponent::Destroy()
	{
		m_spriteSheet.Release();
	}

	bool SpriteComponent::ParseSpritesheet(const rapidjson::Value& spritesheetData)
	{
		EXE_ASSERT(spritesheetData.IsString());

		// Must *NOT* hold a reference prior to this function call.
		EXE_ASSERT(!m_spriteSheet.IsReferenceHeld());
		m_spriteSheet.SetResourceID(spritesheetData.GetString());
		EXE_ASSERT(m_spriteSheet.IsReferenceHeld());

		m_spriteSheet.QueueLoad(true);

		return true;
	}

	bool SpriteComponent::ParseSprite(const rapidjson::Value& spriteData)
	{
		EXE_ASSERT(spriteData.IsObject());

		// We are inside of the "Sprite" value.
		// Get the name of the sprite and get it.

		auto nameMember = spriteData.FindMember("name");

		if (nameMember == spriteData.MemberEnd())
		{
			EXELOG_ENGINE_WARN("Initialization for SpriteComponent failed: Sprite has no 'name' value.");
			return false;
		}

		EXE_ASSERT(nameMember->value.IsString());

		//m_pSprite = m_pSpritesheetResource->GetSprite(nameMember->value.GetString());

		m_spriteID = nameMember->value.GetString();

		if (!m_spriteID.IsValid())
		{
			EXELOG_ENGINE_WARN("Failed to obtain sprite with name '{}'.", nameMember->value.GetString());
			return false;
		}

		// Everything below here can fail safely.

		auto xOffsetMember = spriteData.FindMember("xOffset");
		auto yOffsetMember = spriteData.FindMember("yOffset");
		auto xScaleMember = spriteData.FindMember("xScale");
		auto yScaleMember = spriteData.FindMember("yScale");

		// TODO: This could be better.
		if (xOffsetMember != spriteData.MemberEnd())
		{
			EXE_ASSERT(xOffsetMember->value.IsFloat());
			m_xOffset = xOffsetMember->value.GetFloat();
		}
		if (yOffsetMember != spriteData.MemberEnd())
		{
			EXE_ASSERT(yOffsetMember->value.IsFloat());
			m_yOffset = yOffsetMember->value.GetFloat();
		}
		if (xScaleMember != spriteData.MemberEnd())
		{
			EXE_ASSERT(xScaleMember->value.IsFloat());
			m_xScale = xScaleMember->value.GetFloat();
		}
		if (yScaleMember != spriteData.MemberEnd())
		{
			EXE_ASSERT(yScaleMember->value.IsFloat());
			m_yScale = yScaleMember->value.GetFloat();
		}

		return true;
	}
}