#include "EXEPCH.h"
#include "SpriteComponent.h"
#include "Source/Engine/Resources/ResourceRetrieval.h"

namespace Exelius
{
	SpriteComponent::~SpriteComponent()
	{
		EXE_ASSERT(ResourceManager::GetInstance());
		if (m_pSpritesheetResource)
			ResourceManager::GetInstance()->ReleaseResource(m_pSpritesheetResource->GetResourceID());
		m_pSpritesheetResource = nullptr;
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

		for (auto componentDataObj = jsonComponentData.MemberBegin(); componentDataObj != jsonComponentData.MemberEnd(); ++componentDataObj)
		{
			eastl::string objectName = componentDataObj->name.GetString();

			if (objectName == "Spritesheet")
			{
				EXE_ASSERT(componentDataObj->value.IsString());
				m_pSpritesheetResource = GetResourceAs<SpritesheetResource>(componentDataObj->value.GetString(), true);

				if (!m_pSpritesheetResource)
				{
					EXELOG_ENGINE_WARN("Initialization for SpriteComponent failed: Spritesheet '{}' returned nullptr.", componentDataObj->value.GetString());
					return false;
				}
			}
			else if (objectName == "Sprite")
			{
				EXELOG_ENGINE_FATAL("Ignoring '{}' for now.", objectName.c_str());
			}
			else // Default case:
			{
				EXELOG_ENGINE_WARN("Initialization for SpriteComponent failed: '{}' unhandled Data type found.", componentDataObj->name.GetString());
				return false;
			}
		}

		return true;
	}

	void SpriteComponent::Render() const
	{
		m_pSpritesheetResource->GetTextureResource()->GetTexture()->Render();
	}
}