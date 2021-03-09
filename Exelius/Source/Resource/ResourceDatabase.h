#pragma once

#include "Source/Resource/Resource.h"

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
			//
		}

		ResourceEntry(const ResourceEntry&) = delete;
		ResourceEntry(ResourceEntry&&) = delete;
		ResourceEntry& operator=(const ResourceEntry&) = delete;
		ResourceEntry& operator=(ResourceEntry&&) = delete;

		~ResourceEntry()
		{
			m_pResource->Unload();
			delete m_pResource;
			m_pResource = nullptr;
		}

		// Maybe I should check for valid load states and return null value if not loaded?
		Resource* GetResource() const { return m_pResource; }

		// This should be better, probably set by some friend function?
		void SetResource(Resource* pResource) { m_pResource = pResource; }

		void IncrementRefCount() { ++m_refCount; }

		bool DecrementRefCount()
		{
			--m_refCount;
			if (m_refCount + m_lockCount <= 0)
			{
				return false;
			}

			return true;
		}

		void IncrementLockCount() { ++m_refCount; }

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
		eastl::unordered_map<ResourceID, ResourceEntry*> m_resourceMap;
		std::mutex m_mapLock;
		
	public:
		ResourceEntry* GetEntry(const ResourceID& resourceID);
		ResourceLoadStatus GetLoadStatus(const ResourceID& resourceID);
		void CreateEntry(const ResourceID& resourceID);
		bool IsFound(const ResourceID& resourceID);
		void Unload(const ResourceID& resourceID);
		void UnloadAll();
	};
}