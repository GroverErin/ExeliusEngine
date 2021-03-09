#include "EXEPCH.h"
#include "Source/Engine/Resources/ExeliusResourceFactory.h"

#include "Source/Engine/Resources/ExeliusResourceTypes.h"

#include "Source/Engine/Resources/TextFileResource.h"
#include "Source/Engine/Resources/TextureResource.h"

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
				EXELOG_ENGINE_WARN("Resource cannot be created: Invalid or Unsupported Resource Type.");
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
		}

		return pNewResource;
	}

	ResourceType::Type ExeliusResourceFactory::GetResourceType(const ResourceID& resourceID) const
	{
		EXE_ASSERT(resourceID.IsValid());

		eastl::string fileExtension = resourceID.Get().substr(resourceID.Get().find_last_of('.') + 1);

		if (fileExtension.empty())
		{
			EXELOG_ENGINE_WARN("Could not parse resource extention.");
			return ResourceType::kInvalid;
		}

		if (fileExtension == "txt")
		{
			return ResourceType::kTextFile;
		}
		else if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "bmp")
		{
			return ResourceType::kTexture;
		}

		return ResourceType::kInvalid;
	}
}