#include "EXEPCH.h"
#include "SpriteComponent.h"
#include "Source/OS/Interface/Graphics/Texture.h"
#include "Source/OS/Interface/Graphics/Sprite.h"

#include "Source/Engine/Application.h"
#include "Source/Engine/GameObjectSystem/GameObject.h"
#include "Source/Engine/GameObjectSystem/Components/ComponentTypes/TransformComponent.h"
#include "Source/Resource/ResourceHandle.h"
#include "Source/Engine/Resources/ResourceTypes/SpritesheetResource.h"

#include "Source/Render/RenderManager.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	bool SpriteComponent::Initialize(GameObject* pOwner, const rapidjson::Value& jsonComponentData)
	{
		EXE_ASSERT(pOwner);
		Log log("GameObjectSystem");
		m_pOwner = pOwner;

		// Get the spritesheet associated with this sprite.
		auto spritesheetData = jsonComponentData.FindMember("Spritesheet");

		if (spritesheetData == jsonComponentData.MemberEnd())
		{
			log.Warn("Initialization for SpriteComponent failed: No Spritesheet data found.");
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
			log.Warn("Initialization for SpriteComponent failed: No Sprite data found.");
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
		//EXE_ASSERT(m_pOwner);
		Log log("GameObjectSystem");
		if (!m_pOwner)
		{
			log.Fatal("Owner of SpriteComponent was nullptr. This should NEVER happen.");
			return;
		}

		// Get the transform component sibling.
		auto transformComponent = m_pOwner->GetComponent<TransformComponent>();

		ResourceHandle spriteSheet(m_spriteSheetID);

		auto* pSheet = spriteSheet.GetAs<SpritesheetResource>();

		if (!pSheet)
			return;

		if (!transformComponent.IsValid())
			return;

		RenderCommand command;
		command.m_renderLayer = RenderCommand::RenderLayer::World;
		command.m_texture = pSheet->GetTextureResource();
		command.m_spriteFrame = pSheet->GetSprite(m_spriteID);
		command.m_scaleFactor = { m_xScale, m_yScale };
		command.m_position = { transformComponent->GetX() + m_xOffset, transformComponent->GetY() + m_yOffset };
		RenderManager::GetInstance()->PushRenderCommand(command);
	}

	void SpriteComponent::Destroy()
	{
		ResourceHandle spriteSheet(m_spriteSheetID);
		spriteSheet.UnlockResource();
	}

	bool SpriteComponent::ParseSpritesheet(const rapidjson::Value& spritesheetData)
	{
		EXE_ASSERT(spritesheetData.IsString());

		m_spriteSheetID = spritesheetData.GetString();
		EXE_ASSERT(m_spriteSheetID.IsValid());

		ResourceHandle spriteSheet(m_spriteSheetID);
		//EXE_ASSERT(spriteSheet.IsReferenceHeld());

		//spriteSheet.SetResourceID(m_spriteSheetID);

		spriteSheet.QueueLoad(true);

		// We lock here until this component is destroyed.
		spriteSheet.LockResource();

		return true;
	}

	bool SpriteComponent::ParseSprite(const rapidjson::Value& spriteData)
	{
		EXE_ASSERT(spriteData.IsObject());
		Log log("GameObjectSystem");

		// We are inside of the "Sprite" value.
		// Get the name of the sprite and get it.

		auto nameMember = spriteData.FindMember("name");

		if (nameMember == spriteData.MemberEnd())
		{
			log.Warn("Initialization for SpriteComponent failed: Sprite has no 'name' value.");
			return false;
		}

		EXE_ASSERT(nameMember->value.IsString());

		//m_pSprite = m_pSpritesheetResource->GetSprite(nameMember->value.GetString());

		m_spriteID = nameMember->value.GetString();

		if (!m_spriteID.IsValid())
		{
			log.Warn("Failed to obtain sprite with name '{}'.", nameMember->value.GetString());
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