#pragma once
#include "Source/Resource/ResourceManager.h"
#include "Source/Engine/Resources/ResourceTypes/TextFileResource.h"

namespace Exelius
{
	template <class ResourceType>
	ResourceType* GetResourceAs(const ResourceID& resourceID, bool forceLoad = false)
	{
		assert(ResourceManager::GetInstance());
		return static_cast<ResourceType*>(ResourceManager::GetInstance()->GetResource(resourceID, forceLoad));
	}
}