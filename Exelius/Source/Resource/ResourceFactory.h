#pragma once
#include "Source/Resource/ResourceHelpers.h"

namespace Exelius
{
	class Resource;

	class ResourceFactory
	{
	public:
		virtual ~ResourceFactory() = default;
		virtual Resource* CreateResource(const ResourceID& resourceID, ResourceType::Type type) = 0;
		virtual ResourceType::Type GetResourceType(const char* resourcePath) const = 0;

	protected:
		static void SetResourceIdOnResource(Resource* pResource, const ResourceID& id);
	};
}