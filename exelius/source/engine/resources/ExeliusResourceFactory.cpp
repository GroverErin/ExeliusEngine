#include "EXEPCH.h"
#include "source/engine/resources/ExeliusResourceFactory.h"

#include "source/engine/resources/resourcetypes/ExeliusResourceTypes.h"

#include "source/engine/resources/resourcetypes/TextFileResource.h"
#include "source/engine/resources/resourcetypes/TextureResource.h"
#include "source/engine/resources/resourcetypes/SpritesheetResource.h"
#include "source/engine/resources/resourcetypes/FontResource.h"
#include "source/engine/resources/resourcetypes/AudioResource.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	Resource* ExeliusResourceFactory::CreateResource(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		Resource* pNewResource = nullptr;
		ResourceType::Type type = GetResourceType(resourceID);

		switch (type)
		{
		case ResourceType::kInvalid:
			{
				m_resourceManagerLog.Warn("Resource cannot be created: Invalid or Unsupported Resource Type.");
				break;
			}
		case ResourceType::kTextFile:
			{
				pNewResource = new TextFileResource(resourceID);
				break;
			}
		case ResourceType::kTexture:
			{
				pNewResource = new TextureResource(resourceID);
				break;
			}
		case ResourceType::kSpritesheet:
			{
				pNewResource = new SpritesheetResource(resourceID);
				break;
			}
		case ResourceType::kFont:
			{
				pNewResource = new FontResource(resourceID);
				break;
			}
		case ResourceType::kAudio:
			{
				pNewResource = new AudioResource(resourceID);
				break;
			}
		}

		return pNewResource;
	}

	ResourceType::Type ExeliusResourceFactory::GetResourceType(const ResourceID& resourceID) const
	{
		EXE_ASSERT(resourceID.IsValid());

		eastl::string fileExtension = resourceID.Get().substr(resourceID.Get().find_last_of('.') + 1);

		if (fileExtension.empty())
		{
			m_resourceManagerLog.Warn("Could not parse resource extention.");
			return ResourceType::kInvalid;
		}

		if (fileExtension == "txt" || fileExtension == "json")
		{
			return ResourceType::kTextFile;
		}
		else if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "bmp")
		{
			return ResourceType::kTexture;
		}
		else if (fileExtension == "spsh")
		{
			return ResourceType::kSpritesheet;
		}
		else if (fileExtension == "font")
		{
			return ResourceType::kFont;
		}
		else if (fileExtension == "wav" || fileExtension == "ogg" || fileExtension == "flac")
		{
			return ResourceType::kAudio;
		}
		
		return ResourceType::kInvalid;
	}
}
