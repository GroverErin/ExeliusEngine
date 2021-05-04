#pragma once
#include "ResourceHelpers.h"
#include "ResourceListener.h"
#include "Source/Utility/Generic/Handle.h"

namespace Exelius
{
	class Resource;

	class ResourceHandle
	{
		ResourceID m_resourceID;
		Handle m_internalHandle;
		bool m_resourceHeld;

	public:
		ResourceHandle();
		ResourceHandle(const ResourceID& resourceID, bool loadResource = false);
		ResourceHandle(const ResourceHandle&) = default;
		ResourceHandle(ResourceHandle&&) = delete;
		ResourceHandle& operator=(const ResourceHandle&) = delete;
		ResourceHandle& operator=(ResourceHandle&&) = delete;
		~ResourceHandle();

		Resource* Get(bool forceLoad = false) const;

		template <class Type>
		Type* GetAs(bool forceLoad = false) const
		{
			return static_cast<Type*>(Get(forceLoad));
		}

		void QueueLoad(bool signalLoaderThread, ResourceListenerPtr pListener = ResourceListenerPtr());
		void LoadNow(ResourceListenerPtr pListener = ResourceListenerPtr());
		void Release();

		void SetResourceID(const ResourceID& idToSet);
		const ResourceID& GetID() const { return m_resourceID; }
		bool IsReferenceHeld() const { return m_resourceHeld; }

		bool operator==(const ResourceHandle& right) { return (m_resourceID == right.m_resourceID); }
		bool operator!=(const ResourceHandle& right) { return (m_resourceID != right.m_resourceID); }
	};
}