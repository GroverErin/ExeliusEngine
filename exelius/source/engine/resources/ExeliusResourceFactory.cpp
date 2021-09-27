#include "EXEPCH.h"
#include "source/engine/resources/ExeliusResourceFactory.h"

#include "source/engine/resources/resourceTypes/ExeliusResourceTypes.h"

#include "source/engine/resources/resourceTypes/TextFileResource.h"
#include "source/engine/resources/resourceTypes/TextureResource.h"
#include "source/engine/resources/resourceTypes/SpritesheetResource.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	Resource* ExeliusResourceFactory::CreateResource(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		Log log("ResourceManager");

		Resource* pNewResource = nullptr;
		ResourceType::Type type = GetResourceType(resourceID);

		switch (type)
		{
		case ResourceType::kInvalid:
			{
				log.Warn("Resource cannot be created: Invalid or Unsupported Resource Type.");
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
		}

		return pNewResource;
	}

	ResourceType::Type ExeliusResourceFactory::GetResourceType(const ResourceID& resourceID) const
	{
		EXE_ASSERT(resourceID.IsValid());
		Log log("ResourceManager");

		eastl::string fileExtension = resourceID.Get().substr(resourceID.Get().find_last_of('.') + 1);

		if (fileExtension.empty())
		{
			log.Warn("Could not parse resource extention.");
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

		return ResourceType::kInvalid;
	}
}