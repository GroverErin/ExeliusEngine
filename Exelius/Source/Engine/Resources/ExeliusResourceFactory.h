#pragma once
#include "Source/Resource/ResourceFactory.h"

namespace Exelius
{
	class ExeliusResourceFactory
		: public ResourceFactory
	{
	public:
		virtual Resource* CreateResource(const ResourceID& resourceID) final override;

	protected:
		virtual ResourceType::Type GetResourceType(const ResourceID& resourceID) const final override;
	};
}