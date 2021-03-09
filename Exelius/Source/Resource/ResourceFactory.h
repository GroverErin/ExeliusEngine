#pragma once
#include "Source/Resource/ResourceHelpers.h"

namespace Exelius
{
	class Resource;

	class ResourceFactory
	{
	public:
		virtual ~ResourceFactory() = default;
		virtual Resource* CreateResource(const ResourceID& resourceID) = 0;

	protected:
		virtual ResourceType::Type GetResourceType(const ResourceID& resourceID) const = 0;
	};
}