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
			, m_refCount(0)
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
					EXELOG_ENGINE_WARN("Destroying resource '{}' that has REFCOUNT: {}, and LOCKCOUNT: {}", m_pResource->GetResourceID().Get().c_str(), m_refCount, m_lockCount);
				else
					EXELOG_ENGINE_WARN("Destroying nullptr resource that has REFCOUNT: {}, and LOCKCOUNT: {}", m_refCount, m_lockCount);
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
			return nullptr;
		}

		// This should be better, probably set by some friend function?
		void SetResource(Resource* pResource) { m_pResource = pResource; }

		void IncrementRefCount() { ++m_refCount; }

		bool DecrementRefCount()
		{
			--m_refCount;

			if (m_refCount < 0)
				m_refCount = 0;

			if (m_lockCount < 0)
				m_lockCount = 0;

			return !(m_refCount + m_lockCount <= 0);
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