#pragma once

#include "Source/Resource/Resource.h"

#include "Source/Debugging/EngineLog.h"

#include <EASTL/unordered_map.h>

#include <mutex>

namespace Exelius
{
	// Contains the reference count of the resource, the load status of the resource, and the Raw data if it was kept.
	class ResourceEntry
	{
		friend class ResourceDatabase;

		Resource* m_pResource;
		ResourceLoadStatus m_status;
		int m_refCount;
		int m_lockCount;

	public:
		ResourceEntry()
			: m_pResource(nullptr)
			, m_status(ResourceLoadStatus::kInvalid)
			, m_refCount(1)
			, m_lockCount(0)
		{

		}
		ResourceEntry(const ResourceEntry&) = delete;
		ResourceEntry(ResourceEntry&&) = delete;
		ResourceEntry& operator=(const ResourceEntry&) = delete;
		ResourceEntry& operator=(ResourceEntry&&) = delete;

		~ResourceEntry()
		{
			if (m_refCount + m_lockCount > 0)
			{
				if (m_pResource)
					EXELOG_ENGINE_ERROR("Destroying resource '{}' that has REFCOUNT: {}, and LOCKCOUNT: {}", m_pResource->GetResourceID().Get().c_str(), m_refCount, m_lockCount);
				else
					EXELOG_ENGINE_ERROR("Destroying nullptr resource that has REFCOUNT: {}, and LOCKCOUNT: {}", m_refCount, m_lockCount);
			}

			delete m_pResource;
			m_pResource = nullptr;
		}

		// Maybe I should check for valid load states and return null value if not loaded?
		Resource* GetResource()
		{
			if (m_status == ResourceLoadStatus::kLoaded)
			{
				IncrementRefCount();
				return m_pResource;
			}
			else if (m_status == ResourceLoadStatus::kUnloading)
			{
				// TODO: This may cause bugs?
				// Do not increment as we are unloading this resource currently.
				return m_pResource;
			}
			else if (m_status == ResourceLoadStatus::kLoading)
			{
				// TODO: BUG: MAJOR SEVERITY
				// Here we have tried to get a resource that isn't loaded yet.
				// This is the FlushedWhileLoading case that Rez fought with.
				// It is likely that a Get call was made by a ResourceHandle,
				// which would decrement the RefCount on destruction, which
				// would reduce the refcount to 0 while the resource is still
				// loading. This can happen during a race condition (and is likely)
				// and will break the loading process of a resource.
				// In this specific case, Release was called by main thread during
				// render call on sprite component right when loader thread had created
				// the resource and was just about to call Lock. (thus, refcount = 0 and lockcount = 1)
				// This is a temporary fix that only fixes this specific case (ResourceHandle)
				// by incrementing the refcount, so that the refcount is not reduced to 0.
				// A potential solution is: unknown.
				IncrementRefCount();
			}
			return nullptr;
		}

		// This should be better, probably set by some friend function?
		void SetResource(Resource* pResource) { m_pResource = pResource; }

		void IncrementRefCount() { ++m_refCount; }

		/// <summary>
		/// Returns true if there is no longer any references or locks on this object.
		/// </summary>
		/// <returns></returns>
		bool DecrementRefCount()
		{
			--m_refCount;

			if (m_refCount < 0)
				m_refCount = 0;

			if (m_lockCount < 0)
				m_lockCount = 0;

			return (m_refCount + m_lockCount == 0);
		}

		void IncrementLockCount() { ++m_lockCount; }

		void DecrementLockCount() { --m_lockCount; }

		bool IsLocked() const { return m_lockCount > 0; }

		/// <summary>
		/// Get the current loading status of this reource.
		/// </summary>
		/// <returns>Loading status.</returns>
		ResourceLoadStatus GetStatus() const { return m_status; }

		/// <summary>
		/// Set the status of this resource. This should only be able to be set by the resource loading management... I should probably not allow the client to adjust this.
		/// </summary>
		/// <param name="status"></param>
		void SetStatus(ResourceLoadStatus status) { m_status = status; }

	};

	// The database for the resources.
	// This must be thread safe because the resource thread will be adding to this map and updating the status' of resources.
	class ResourceDatabase
	{
		eastl::unordered_map<ResourceID, ResourceEntry> m_resourceMap;
		std::mutex m_mapLock;
		
	public:
		ResourceDatabase() = default;
		ResourceDatabase(const ResourceDatabase&) = delete;
		ResourceDatabase(ResourceDatabase&&) = delete;
		ResourceDatabase& operator=(const ResourceDatabase&) = delete;
		ResourceDatabase& operator=(ResourceDatabase&&) = delete;
		~ResourceDatabase();

		ResourceEntry* GetEntry(const ResourceID& resourceID);
		ResourceLoadStatus GetLoadStatus(const ResourceID& resourceID);
		void SetLoadStatus(const ResourceID& resourceID, ResourceLoadStatus newStatus);
		void CreateEntry(const ResourceID& resourceID);
		bool IsFound(const ResourceID& resourceID);
		void Unload(const ResourceID& resourceID);
		void UnloadAll();
	};
}