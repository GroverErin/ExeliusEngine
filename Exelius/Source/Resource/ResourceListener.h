#pragma once
#include "ResourceHelpers.h"

#include <EASTL/shared_ptr.h>

namespace Exelius
{
	class ResourceListener
	{
	public:
		virtual ~ResourceListener() = default;

		//Return true if you flushed the resource.
		virtual bool OnResourceLoaded(const ResourceID& resourceID) = 0;
	};

	using ResourceListenerPtr = eastl::weak_ptr<ResourceListener>;
}