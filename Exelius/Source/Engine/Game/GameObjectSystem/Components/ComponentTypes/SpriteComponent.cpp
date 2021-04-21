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

	bool SpriteComponent::Initialize(GameObject* pOwner)
	{
		EXE_ASSERT(pOwner);
		m_pOwner = pOwner;
		return true;
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
			//Release the spritesheet
			Destroy();
			return false;
		}

		return true;
	}

	void SpriteComponent::Render() const
	{
		auto transformComponent = m_pOwner->GetComponent<TransformComponent>();
		if (transformComponent.IsValid())
		{
			m_pSprite->SetPosition(transformComponent->GetX() + m_xOffset, transformComponent->GetY() + m_yOffset);
		}
		m_pSprite->Render();
	}

	void SpriteComponent::Destroy()
	{
		EXE_ASSERT(ResourceManager::GetInstance());

		if (m_pSpritesheetResource)
			ResourceManager::GetInstance()->ReleaseResource(m_pSpritesheetResource->GetResourceID());
		m_pSpritesheetResource = nullptr;
	}

	bool SpriteComponent::ParseSpritesheet(const rapidjson::Value& spritesheetData)
	{
		EXE_ASSERT(spritesheetData.IsString());
		m_pSpritesheetResource = GetResourceAs<SpritesheetResource>(spritesheetData.GetString(), true);

		if (!m_pSpritesheetResource)
		{
			EXELOG_ENGINE_WARN("Initialization for SpriteComponent failed: Spritesheet '{}' returned nullptr.", spritesheetData.GetString());
			return false;
		}

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

		m_pSprite = m_pSpritesheetResource->GetSprite(nameMember->value.GetString());

		if (!m_pSprite)
		{
			EXELOG_ENGINE_WARN("Failed to obtain sprite with name '{}'.", nameMember->value.GetString());
			return false;
		}

		// Everything below here can fail safely.

		auto xOffsetMember = spriteData.FindMember("xOffset");
		auto yOffsetMember = spriteData.FindMember("yOffset");
		auto xScaleMember = spriteData.FindMember("xScale");
		auto yScaleMember = spriteData.FindMember("yScale");

		float xScale = 1.0f;
		float yScale = 1.0f;

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
			xScale = xScaleMember->value.GetFloat();
		}
		if (yScaleMember != spriteData.MemberEnd())
		{
			EXE_ASSERT(yScaleMember->value.IsFloat());
			yScale = yScaleMember->value.GetFloat();
		}

		m_pSprite->SetScale(xScale, yScale);

		return true;
	}
}