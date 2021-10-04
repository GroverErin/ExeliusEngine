#include "EXEPCH.h"
#include "source/resource/ResourceDatabase.h"
#include "source/resource/Resource.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	ResourceDatabase::ResourceDatabase()
		: m_resourceDatabaseLog("ResourceDatabase")
	{
		//
	}

	/// <summary>
	/// Thread Safe.
	/// Destructor will safely unload all the resources in the database.
	/// </summary>
	ResourceDatabase::~ResourceDatabase()
	{
		m_mapLock.lock();
		UnloadAll();
		m_mapLock.unlock();
	}

	/// <summary>
	/// Deallocate any resourced currently in the unload queue.
	/// This happens once per frame.
	/// Resources can unload more resources, so this acts as a
	/// double buffered queue.
	/// </summary>
	void ResourceDatabase::ProcessUnloadQueue()
	{
		while (!m_unloadQueue.empty())
			InternalProcessUnloadQueue();
	}

	/// <summary>
	/// Deallocate any resourced currently in the unload queue.
	/// This happens once per frame.
	/// Resources can unload more resources, so this acts as a
	/// double buffered queue.
	/// </summary>
	void ResourceDatabase::InternalProcessUnloadQueue()
	{
		// Create the second buffer.
		eastl::vector<ResourceID> m_activeUnloader;
		m_unloaderLock.lock();
		m_activeUnloader.swap(m_unloadQueue);
		m_unloaderLock.unlock();

		for (auto& resourceID : m_activeUnloader)
		{
			m_resourceDatabaseLog.Info("Unloading Resource: {}", resourceID.Get().c_str());
			if (!IsFound(resourceID))
				continue;

			// TODO: Verify that the ResourceEntry is in fact unloadable.

			Resource* pResource = InternalGetEntryResource(resourceID);
			InternalSetLoadStatus(resourceID, ResourceLoadStatus::kUnloading);
			if (pResource)
			{
				pResource->Unload();
				InternalSetLoadStatus(resourceID, ResourceLoadStatus::kUnloaded);
			}

			m_mapLock.lock();
			m_resourceMap.erase(resourceID);
			m_mapLock.unlock();

			m_resourceDatabaseLog.Info("Unloaded Resource '{}'", resourceID.Get().c_str());
		}
		m_activeUnloader.clear();
	}

	/// <summary>
	/// Thread Safe.
	/// Increments the reference count of the entry with the given ResourceID.
	/// </summary>
	/// <param name="resourceID">- The ID of the resource to increment.</param>
	void ResourceDatabase::IncrementEntryRefCount(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceDatabaseLog.Trace("Incrementing Reference Count for '{}'", resourceID.Get().c_str());

		m_mapLock.lock();
		ResourceEntry* pResourceEntry = GetEntry(resourceID);
		if (!pResourceEntry)
		{
			m_mapLock.unlock();
			m_resourceDatabaseLog.Info("Unable to increment reference count on ResourceEntry '{}'", resourceID.Get().c_str());
			return;
		}

		// Increment the reference count of this resource.
		pResourceEntry->IncrementRefCount();
		m_mapLock.unlock();
	}

	/// <summary>
	/// Thread Safe.
	/// Decrements the reference count of the entry with the given ResourceID.
	/// </summary>
	/// <param name="resourceID">- The ID of the resource to decrement.</param>
	/// <returns>True if the ref count + lock count == 0, otherwise false.</returns>
	bool ResourceDatabase::DecrementEntryRefCount(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceDatabaseLog.Trace("Decrementing Reference Count for '{}'", resourceID.Get().c_str());

		m_mapLock.lock();
		ResourceEntry* pResourceEntry = GetEntry(resourceID);
		if (!pResourceEntry)
		{
			m_mapLock.unlock();
			m_resourceDatabaseLog.Info("Unable to decrement reference count on ResourceEntry '{}'", resourceID.Get().c_str());
			return true; // Return true because there cannot be refs or locks on a non-existant entry.
		}

		// Decrement the reference count of this resource.
		bool entryUnheld = pResourceEntry->DecrementRefCount();
		m_mapLock.unlock();

		return entryUnheld;
	}

	/// <summary>
	/// Thread Safe.
	/// Increments the lock count of the entry with the given ResourceID.
	/// </summary>
	/// <param name="resourceID">- The ID of the resource to increment.</param>
	void ResourceDatabase::IncrementEntryLockCount(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceDatabaseLog.Trace("Incrementing Lock Count for '{}'", resourceID.Get().c_str());

		m_mapLock.lock();
		ResourceEntry* pResourceEntry = GetEntry(resourceID);
		if (!pResourceEntry)
		{
			m_mapLock.unlock();
			m_resourceDatabaseLog.Info("Unable to increment lock count on ResourceEntry '{}'", resourceID.Get().c_str());
			return;
		}

		// Increment the reference count of this resource.
		pResourceEntry->IncrementLockCount();
		m_mapLock.unlock();
	}

	/// <summary>
	/// Thread Safe.
	/// Decrements the lock count of the entry with the given ResourceID.
	/// </summary>
	/// <param name="resourceID">- The ID of the resource to decrement.</param>
	/// <returns>True if the ref count + lock count == 0, otherwise false.</returns>
	bool ResourceDatabase::DecrementEntryLockCount(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceDatabaseLog.Trace("Decrementing Lock Count for '{}'", resourceID.Get().c_str());

		m_mapLock.lock();
		ResourceEntry* pResourceEntry = GetEntry(resourceID);
		if (!pResourceEntry)
		{
			m_mapLock.unlock();
			m_resourceDatabaseLog.Info("Unable to decrement lock count on ResourceEntry '{}'", resourceID.Get().c_str());
			return true; // Return true because there cannot be refs or locks on a non-existant entry.
		}

		// Decrement the Lock count of this resource.
		bool entryUnheld = pResourceEntry->DecrementLockCount();
		m_mapLock.unlock();

		return entryUnheld;
	}

	/// <summary>
	/// Thread Safe.
	/// Sets the given resource onto the resource entry.
	/// </summary>
	/// <param name="resourceID">- The resource ID to set the resource onto.</param>
	/// <param name="pResource">- The resource to set.</param>
	void ResourceDatabase::SetEntryResource(const ResourceID& resourceID, Resource* pResource)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXE_ASSERT(pResource);

		m_mapLock.lock();
		GetEntry(resourceID)->SetResource(pResource);
		m_mapLock.unlock();
	}

	/// <summary>
	/// Thread Safe.
	/// Gets the resource from a resource entry if it exists.
	/// </summary>
	/// <param name="resourceID">- The resource ID to get a resource from.</param>
	/// <returns>The resource from the given ID, nullptr if it did not exist.</returns>
	Resource* ResourceDatabase::GetEntryResource(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		m_mapLock.lock();
		auto pResource = InternalGetEntryResource(resourceID);
		if (!pResource)
		{
			m_mapLock.unlock();
			return nullptr;
		}

		m_mapLock.unlock();
		return pResource;
	}

	/// <summary>
	/// Thread Safe.
	/// Gets the current load status of the resource entry with the given ID.
	/// </summary>
	/// <param name="resourceID">- The resource ID for the ResourceEntry to check.</param>
	/// <returns>The current load status of the resource entry. Returns ResourceLoadStatus::kInvalid if the ResourceEntry does not exist.</returns>
	ResourceLoadStatus ResourceDatabase::GetEntryLoadStatus(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		m_mapLock.lock();
		auto status = InternalGetLoadStatus(resourceID);
		m_mapLock.unlock();
		return status;
	}

	/// <summary>
	/// Thread Safe.
	/// Sets the current load status of the resource entry with the given ID.
	/// </summary>
	/// <param name="resourceID">- The resource ID for the ResourceEntry to set.</param>
	/// <param name="newStatus">- The load status to set.</param>
	void ResourceDatabase::SetEntryLoadStatus(const ResourceID& resourceID, ResourceLoadStatus newStatus)
	{
		EXE_ASSERT(resourceID.IsValid());

		m_mapLock.lock();
		InternalSetLoadStatus(resourceID, newStatus);
		m_mapLock.unlock();
	}

	/// <summary>
	/// Thread Safe.
	/// Creates an empty resource entry if it doesn't already exist.
	/// </summary>
	/// <param name="resourceID">- The resource to create.</param>
	/// <returns>True if the ResourceEntry was created, false if it already existed.</returns>
	bool ResourceDatabase::CreateEntry(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		m_mapLock.lock();
		if (IsFound(resourceID))
		{
			m_mapLock.unlock();
			m_resourceDatabaseLog.Info("Resource Entry for {} already exists.", resourceID.Get().c_str());
			return false;
		}

		m_resourceMap.try_emplace(resourceID);
		m_mapLock.unlock();

		return true;
	}

	/// <summary>
	/// Thread Safe.
	/// Unloads a resource with the given ID.
	/// </summary>
	/// <param name="resourceID">- The resource to unload.</param>
	void ResourceDatabase::UnloadEntry(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceDatabaseLog.Trace("Adding resource to unload queue: {}", resourceID.Get().c_str());

		m_unloaderLock.lock();
		// TODO:
		// Ideally I would like to set the status to unloading here,
		// the problem is how it triggers an "error state" by calling
		// GetResource on an "Unloading" asset during the shutdown
		// process. This will likely cause issues with resources
		// getting acquired while they sit in the unload queue.
		//InternalSetLoadStatus(resourceID, ResourceLoadStatus::kUnloading);
		m_unloadQueue.emplace_back(resourceID);
		m_unloaderLock.unlock();
	}

	/// <summary>
	/// Non-Thread-Safe.
	/// Gets the current load status of the resource entry with the given ID.
	/// </summary>
	/// <param name="resourceID">- The resource ID for the ResourceEntry to check.</param>
	/// <returns>The current load status of the resource entry. Returns ResourceLoadStatus::kInvalid if the ResourceEntry does not exist.</returns>
	ResourceLoadStatus ResourceDatabase::InternalGetLoadStatus(const ResourceID& resourceID)
	{
		if (IsFound(resourceID))
		{
			auto status = m_resourceMap.at(resourceID).GetStatus();
			return status;
		}
		return ResourceLoadStatus::kInvalid;
	}

	/// <summary>
	/// Non-Thread-Safe.
	/// Sets the current load status of the resource entry with the given ID.
	/// </summary>
	/// <param name="resourceID">- The resource ID for the ResourceEntry to set.</param>
	/// <param name="newStatus">- The load status to set.</param>
	void ResourceDatabase::InternalSetLoadStatus(const ResourceID& resourceID, ResourceLoadStatus newStatus)
	{
		if (IsFound(resourceID))
			m_resourceMap.at(resourceID).SetStatus(newStatus);
	}

	/// <summary>
	/// Non-Thread-Safe.
	/// Gets the resource from a resource entry if it exists.
	/// </summary>
	/// <param name="resourceID">- The resource ID to get a resource from.</param>
	/// <returns>The resource from the given ID, nullptr if it did not exist.</returns>
	Resource* ResourceDatabase::InternalGetEntryResource(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		ResourceEntry* pResourceEntry = GetEntry(resourceID);
		if (!pResourceEntry)
		{
			m_resourceDatabaseLog.Warn("Unable to get Resource from ResourceEntry '{}'", resourceID.Get().c_str());
			return nullptr;
		}

		Resource* pResource = pResourceEntry->GetResource();
		if (!pResource)
		{
			m_resourceDatabaseLog.Warn("Resource from ResourceEntry '{}' was nullptr.", resourceID.Get().c_str());
			return nullptr;
		}

		return pResource;
	}

	/// <summary>
	/// Non-Thread-Safe.
	/// Checks if a ResourceEntry with the given ID exists in the database.
	/// </summary>
	/// <param name="resourceID">- The resource ID for the ResourceEntry to check.</param>
	/// <returns>True if found, false otherwise.</returns>
	bool ResourceDatabase::IsFound(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		auto found = m_resourceMap.find(resourceID);
		if (found == m_resourceMap.end())
			return false;

		return true;
	}

	/// <summary>
	/// Non-Thread-Safe.
	/// Gets a resource entry with the given ID if it exists.
	/// </summary>
	/// <param name="resourceID">- The resource ID for the ResourceEntry to get.</param>
	/// <returns>The resource entry with the given ID, nullptr if not found.</returns>
	ResourceEntry* ResourceDatabase::GetEntry(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		if (IsFound(resourceID))
			return &m_resourceMap.at(resourceID);

		m_resourceDatabaseLog.Warn("Resource Entry '{}' does not exist in database.", resourceID.Get().c_str());
		return nullptr;
	}

	/// <summary>
	/// Non-Thread-Safe.
	/// Unloads all the resources in the resource database.
	/// </summary>
	void ResourceDatabase::UnloadAll()
	{
		m_resourceDatabaseLog.Trace("Beginning Unload All Resources.");

		if (!m_unloadQueue.empty())
		{
			m_resourceDatabaseLog.Trace("The unloading queue was not empty and should be.");
		}

		for (auto& resourcePair : m_resourceMap)
		{
			m_resourceDatabaseLog.Trace("Unloading Resource: {}", resourcePair.first.Get().c_str());
			Resource* pResource = resourcePair.second.GetResource();
			resourcePair.second.SetStatus(ResourceLoadStatus::kUnloading);
			if (pResource)
			{
				pResource->Unload();
				resourcePair.second.SetStatus(ResourceLoadStatus::kUnloaded);
			}
		}

		m_resourceMap.clear();

		m_resourceDatabaseLog.Info("Completed Unload All Resources.");
	}
}