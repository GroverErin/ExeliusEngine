#pragma once
#include "source/resource/ResourceHelpers.h"
#include "source/debug/Log.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Resource;

	/// <summary>
	/// ResourceEntry acts as a container for the data needed by
	/// the ResourceDatabase.
	/// 
	/// The resource system will populate the entry as the resource
	/// is loaded, retrieved, or unloaded.
	/// </summary>
	class ResourceEntry
	{
		/// <summary>
		/// Pointer to the resource subclass held by this entry.
		/// </summary>
		Resource* m_pResource;

		/// <summary>
		/// The current load status of this entry.
		/// </summary>
		ResourceLoadStatus m_status;

		/// <summary>
		/// The total number of held references to this entry.
		/// </summary>
		int m_refCount;

		/// <summary>
		/// The total number of locks placed on this entry.
		/// </summary>
		int m_lockCount;

		/// <summary>
		/// ResourceDatabase Log.
		/// </summary>
		Log m_resourceDatabaseLog;

	public:
		/// <summary>
		/// Create an empty ResourceEntry.
		/// The reference count is incremented because
		/// we know that if we are being created then there
		/// is a reference already.
		/// </summary>
		ResourceEntry();
		ResourceEntry(const ResourceEntry&) = delete;
		ResourceEntry(ResourceEntry&&) = delete;
		ResourceEntry& operator=(const ResourceEntry&) = delete;
		ResourceEntry& operator=(ResourceEntry&&) = delete;

		/// <summary>
		/// Safely destroy the resource held by this entry.
		/// A warning will output if there are still references to this entry.
		/// </summary>
		~ResourceEntry();

		/// <summary>
		/// Retrieve the resource that is held by this entry.
		/// The resource will only be returned in the case that
		/// has a kLoaded load state.
		/// 
		/// The reference count is incremented in the case that the
		/// resource is loaded OR loading.
		/// 
		/// @todo
		/// There may be a couple bugs here. See the .cpp file.
		/// </summary>
		/// <returns>The resource held by this entry if loaded, nullptr otherwise.</returns>
		Resource* GetResource();

		/// <summary>
		/// Sets the given resource on the resource entry.
		/// 
		/// @note
		/// This is only called by the resource manager.
		/// </summary>
		/// <param name="pResource">- The resource to set.</param>
		void SetResource(Resource* pResource);

		/// <summary>
		/// Increment the reference count of this entry.
		/// </summary>
		void IncrementRefCount();

		/// <summary>
		/// Decrement the reference count of this entry.
		/// This function will check the current number
		/// of references and locks.
		/// </summary>
		/// <returns>True if the ref count + lock count == 0, otherwise false.</returns>
		bool DecrementRefCount();

		/// <summary>
		/// Increment the lock count of this entry.
		/// </summary>
		void IncrementLockCount();

		/// <summary>
		/// Decrement the lock count of this entry.
		/// This function will check the current number
		/// of references and locks.
		/// </summary>
		/// <returns>True if the ref count + lock count == 0, otherwise false.</returns>
		bool DecrementLockCount();

		/// <summary>
		/// Check to see if this entry has any locks.
		/// </summary>
		/// <returns>True if there are locks, false otherwise.</returns>
		bool IsLocked() const { return m_lockCount > 0; }

		/// <summary>
		/// Get the current loading status of this resource.
		/// </summary>
		/// <returns>Resource current load status.</returns>
		ResourceLoadStatus GetStatus() const { return m_status; }

		/// <summary>
		/// Set the status of this resource.
		/// 
		/// This should only be set by the resource manager.
		/// </summary>
		/// <param name="status">- The status to set on this entry.</param>
		void SetStatus(ResourceLoadStatus status);

	private:
		/// <summary>
		/// Check if this entry is referenced or locked.
		/// </summary>
		/// <returns>True if the ref count + lock count == 0, otherwise false.</returns>
		bool IsReferenceOrLocked() const;
	};
}