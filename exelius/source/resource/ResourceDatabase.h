#pragma once
#include "source/resource/ResourceHelpers.h"
#include "source/resource/ResourceEntry.h"
#include "source/debug/Log.h"

#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>

#include <mutex>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// The database containing all of the resource entries.
	/// This class is the inteface between the resource manager and the resource entries
	/// which house the resources.
	/// 
	/// These interactions between the resource manager and the database should
	/// be thread safe, as the manipulation of the resource entries happens on
	/// nearly all threads.
	/// </summary>
	class ResourceDatabase
	{
		/// <summary>
		/// The map of the resource entries keyed by the resource ID.
		/// </summary>
		eastl::unordered_map<ResourceID, ResourceEntry> m_resourceMap;

		/// <summary>
		/// Queue of resources to be unloaded.
		/// 
		/// Unloading is a double buffered system,
		/// to protect from resources that unload other resources.
		/// </summary>
		eastl::vector<ResourceID> m_unloadQueue;

		/// <summary>
		/// The mutex that guards the map from data race conditions.
		/// </summary>
		std::mutex m_mapLock;

		/// <summary>
		/// The mutex that guards the unload queue from data race conditions.
		/// </summary>
		std::mutex m_unloaderLock;

		/// <summary>
		/// ResourceDatabase Log.
		/// </summary>
		Log m_resourceDatabaseLog;
		
	public:
		ResourceDatabase();
		ResourceDatabase(const ResourceDatabase&) = delete;
		ResourceDatabase(ResourceDatabase&&) = delete;
		ResourceDatabase& operator=(const ResourceDatabase&) = delete;
		ResourceDatabase& operator=(ResourceDatabase&&) = delete;

		/// <summary>
		/// Thread Safe.
		/// Destructor will safely unload all the resources in the database.
		/// </summary>
		~ResourceDatabase();

		/// <summary>
		/// Deallocate any resourced currently in the unload queue.
		/// This happens once per frame.
		/// Resources can unload more resources, so this acts as a
		/// double buffered queue.
		/// </summary>
		void ProcessUnloadQueue();

		/// <summary>
		/// Thread Safe.
		/// Increments the reference count of the entry with the given ResourceID.
		/// </summary>
		/// <param name="resourceID">- The ID of the resource to increment.</param>
		void IncrementEntryRefCount(const ResourceID& resourceID);

		/// <summary>
		/// Thread Safe.
		/// Decrements the reference count of the entry with the given ResourceID.
		/// </summary>
		/// <param name="resourceID">- The ID of the resource to decrement.</param>
		/// <returns>True if the ref count + lock count == 0, otherwise false.</returns>
		bool DecrementEntryRefCount(const ResourceID& resourceID);

		/// <summary>
		/// Thread Safe.
		/// Increments the lock count of the entry with the given ResourceID.
		/// </summary>
		/// <param name="resourceID">- The ID of the resource to increment.</param>
		void IncrementEntryLockCount(const ResourceID& resourceID);

		/// <summary>
		/// Thread Safe.
		/// Decrements the lock count of the entry with the given ResourceID.
		/// </summary>
		/// <param name="resourceID">- The ID of the resource to decrement.</param>
		/// <returns>True if the ref count + lock count == 0, otherwise false.</returns>
		bool DecrementEntryLockCount(const ResourceID& resourceID);

		/// <summary>
		/// Thread Safe.
		/// Sets the given resource onto the resource entry.
		/// </summary>
		/// <param name="resourceID">- The resource ID to set the resource onto.</param>
		/// <param name="pResource">- The resource to set.</param>
		void SetEntryResource(const ResourceID& resourceID, Resource* pResource);

		/// <summary>
		/// Thread Safe.
		/// Gets the resource from a resource entry if it exists.
		/// </summary>
		/// <param name="resourceID">- The resource ID to get a resource from.</param>
		/// <returns>The resource from the given ID, nullptr if it did not exist.</returns>
		Resource* GetEntryResource(const ResourceID& resourceID);

		/// <summary>
		/// Thread Safe.
		/// Gets the current load status of the resource entry with the given ID.
		/// </summary>
		/// <param name="resourceID">- The resource ID for the ResourceEntry to check.</param>
		/// <returns>The current load status of the resource entry. Returns ResourceLoadStatus::kInvalid if the ResourceEntry does not exist.</returns>
		ResourceLoadStatus GetEntryLoadStatus(const ResourceID& resourceID);

		/// <summary>
		/// Thread Safe.
		/// Sets the current load status of the resource entry with the given ID.
		/// </summary>
		/// <param name="resourceID">- The resource ID for the ResourceEntry to set.</param>
		/// <param name="newStatus">- The load status to set.</param>
		void SetEntryLoadStatus(const ResourceID& resourceID, ResourceLoadStatus newStatus);

		/// <summary>
		/// Thread Safe.
		/// Creates an empty resource entry if it doesn't already exist.
		/// </summary>
		/// <param name="resourceID">- The resource to create.</param>
		/// <returns>True if the ResourceEntry was created, false if it already existed.</returns>
		bool CreateEntry(const ResourceID& resourceID);

		/// <summary>
		/// Thread Safe.
		/// Unloads a resource with the given ID.
		/// </summary>
		/// <param name="resourceID">- The resource to unload.</param>
		void UnloadEntry(const ResourceID& resourceID);

	private:
		/// <summary>
		/// Deallocate any resourced currently in the unload queue.
		/// This happens once per frame.
		/// Resources can unload more resources, so this acts as a
		/// double buffered queue.
		/// </summary>
		void InternalProcessUnloadQueue();

		/// <summary>
		/// Non-Thread-Safe.
		/// Gets the current load status of the resource entry with the given ID.
		/// </summary>
		/// <param name="resourceID">- The resource ID for the ResourceEntry to check.</param>
		/// <returns>The current load status of the resource entry. Returns ResourceLoadStatus::kInvalid if the ResourceEntry does not exist.</returns>
		ResourceLoadStatus InternalGetLoadStatus(const ResourceID& resourceID);

		/// <summary>
		/// Non-Thread-Safe.
		/// Sets the current load status of the resource entry with the given ID.
		/// </summary>
		/// <param name="resourceID">- The resource ID for the ResourceEntry to set.</param>
		/// <param name="newStatus">- The load status to set.</param>
		void InternalSetLoadStatus(const ResourceID& resourceID, ResourceLoadStatus newStatus);

		/// <summary>
		/// Non-Thread-Safe.
		/// Gets the resource from a resource entry if it exists.
		/// </summary>
		/// <param name="resourceID">- The resource ID to get a resource from.</param>
		/// <returns>The resource from the given ID, nullptr if it did not exist.</returns>
		Resource* InternalGetEntryResource(const ResourceID& resourceID);

		/// <summary>
		/// Non-Thread-Safe.
		/// Checks if a ResourceEntry with the given ID exists in the database.
		/// </summary>
		/// <param name="resourceID">- The resource ID for the ResourceEntry to check.</param>
		/// <returns>True if found, false otherwise.</returns>
		bool IsFound(const ResourceID& resourceID);

		/// <summary>
		/// Non-Thread-Safe.
		/// Gets a resource entry with the given ID if it exists.
		/// </summary>
		/// <param name="resourceID">- The resource ID for the ResourceEntry to get.</param>
		/// <returns>The resource entry with the given ID, nullptr if not found.</returns>
		ResourceEntry* GetEntry(const ResourceID& resourceID);

		/// <summary>
		/// Non-Thread-Safe.
		/// Unloads all the resources in the resource database.
		/// </summary>
		void UnloadAll();
	};
}