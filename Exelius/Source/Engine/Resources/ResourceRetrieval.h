#pragma once
#include "Source/Resource/ResourceManager.h"
#include "Source/Engine/Resources/ResourceTypes/TextFileResource.h"
#include "Source/Engine/Resources/ResourceTypes/SpritesheetResource.h"
#include "Source/Engine/Resources/ResourceTypes/TextureResource.h"

namespace Exelius
{
	/// <summary>
	/// Get a resource as the templated type.
	/// 
	/// Example (replace '{ }' with angle brackets):
	///		GetResourceAs{TextFileResource}(resourceID};
	/// </summary>
	template <class ResourceType>
	ResourceType* GetResourceAs(const ResourceID& resourceID, bool forceLoad = false)
	{
		assert(ResourceManager::GetInstance());
		return static_cast<ResourceType*>(ResourceManager::GetInstance()->GetResource(resourceID, forceLoad));
	}
}