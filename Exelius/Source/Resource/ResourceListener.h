#pragma once
#include "ResourceHelpers.h"

#include <EASTL/shared_ptr.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
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