#include "EXEPCH.h"

#include "source/resource/ResourceEntry.h"
#include "source/resource/Resource.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Create an empty ResourceEntry.
	/// The reference count is incremented because
	/// we know that if we are being created then there
	/// is a reference already.
	/// </summary>
	ResourceEntry::ResourceEntry()
		: m_pResource(nullptr)
		, m_status(ResourceLoadStatus::kInvalid)
		, m_refCount(1)
		, m_lockCount(0)
		, m_resourceDatabaseLog("ResourceDatabase")
	{
		//
	}

	/// <summary>
	/// Safely destroy the resource held by this entry.
	/// A warning will output if there are still references to this entry.
	/// </summary>
	ResourceEntry::~ResourceEntry()
	{
		if (m_refCount + m_lockCount > 0)
		{
			if (m_pResource)
				m_resourceDatabaseLog.Warn("Destroying resource '{}' that has REFCOUNT: {}, and LOCKCOUNT: {}", m_pResource->GetResourceID().Get().c_str(), m_refCount, m_lockCount);
			else
				m_resourceDatabaseLog.Warn("Destroying nullptr resource that has REFCOUNT: {}, and LOCKCOUNT: {}", m_refCount, m_lockCount);
		}

		if (m_pResource)
			m_resourceDatabaseLog.Info("Destroying resource '{}'.", m_pResource->GetResourceID().Get().c_str());
		else
			m_resourceDatabaseLog.Info("Destroying nullptr resource.");

		delete m_pResource;
		m_pResource = nullptr;
	}

	/// <summary>
	/// Retrieve the resource that is held by this entry.
	/// The resource will only be returned in the case that
	/// has a kLoaded load state.
	/// 
	/// The reference count is incremented in the case that the
	/// resource is loaded OR loading.
	/// 
	/// @todo
	/// There may be a couple bugs here.
	/// </summary>
	/// <returns>The resource held by this entry if loaded, nullptr otherwise.</returns>
	Resource* ResourceEntry::GetResource()
	{
		if (m_status == ResourceLoadStatus::kLoaded)
		{
			return m_pResource;
		}
		else if (m_status == ResourceLoadStatus::kUnloading)
		{
			// If GetResource is called during an unloading operation,
			// be sure to make sure all calls to GetResource are made
			// before setting the status to unloading (unless it is the
			// intended purpose to catch this kind of error!) otherwise
			// this branch will trigger unintentionally.
			m_resourceDatabaseLog.Fatal("Attempting to get a resource that is unloading.");
		}
		else if (m_status == ResourceLoadStatus::kLoading)
		{
			// TODO:
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
			//IncrementRefCount();
			m_resourceDatabaseLog.Fatal("Attempting to get a resource that is loading.");
		}
		return nullptr;
	}

	/// <summary>
	/// Sets the given resource on the resource entry.
	/// 
	/// @note
	/// This is only called by the resource manager.
	/// </summary>
	/// <param name="pResource">- The resource to set.</param>
	void ResourceEntry::SetResource(Resource* pResource)
	{
		EXE_ASSERT(pResource);
		m_pResource = pResource;
	}

	/// <summary>
	/// Increment the reference count of this entry.
	/// </summary>
	void ResourceEntry::IncrementRefCount()
	{
		++m_refCount;
		m_resourceDatabaseLog.Trace("New Ref Count: {}", m_refCount);
	}

	/// <summary>
	/// Decrement the reference count of this entry.
	/// This function will check the current number
	/// of references and locks.
	/// </summary>
	/// <returns>True if the ref count + lock count == 0, otherwise false.</returns>
	bool ResourceEntry::DecrementRefCount()
	{
		--m_refCount;

		if (m_refCount < 0)
		{
			m_resourceDatabaseLog.Warn("ResourceEntry Ref Count below 0. Resource is being Over Released.");
			m_refCount = 0;
		}

		m_resourceDatabaseLog.Trace("New Ref Count: {}", m_refCount);
		return IsReferenceOrLocked();
	}

	/// <summary>
	/// Increment the lock count of this entry.
	/// </summary>
	void ResourceEntry::IncrementLockCount()
	{
		++m_lockCount;
		m_resourceDatabaseLog.Trace("New Lock Count: {}", m_lockCount);
	}

	/// <summary>
	/// Decrement the lock count of this entry.
	/// This function will check the current number
	/// of references and locks.
	/// </summary>
	/// <returns>True if the ref count + lock count == 0, otherwise false.</returns>
	bool ResourceEntry::DecrementLockCount()
	{
		--m_lockCount;

		if (m_lockCount < 0)
		{
			m_resourceDatabaseLog.Warn("ResourceEntry Lock Count below 0. Resource is being Over Unlocked.");
			m_lockCount = 0;
		}

		m_resourceDatabaseLog.Trace("New Lock Count: {}", m_lockCount);
		return IsReferenceOrLocked();
	}

	/// <summary>
	/// Set the status of this resource.
	/// 
	/// This should only be set by the resource manager.
	/// </summary>
	/// <param name="status">- The status to set on this entry.</param>
	void ResourceEntry::SetStatus(ResourceLoadStatus status)
	{
		EXE_ASSERT(status != ResourceLoadStatus::kInvalid);
		m_status = status;
	}

	/// <summary>
	/// Check if this entry is referenced or locked.
	/// </summary>
	/// <returns>True if referenced or locked, false otherwise.</returns>
	bool ResourceEntry::IsReferenceOrLocked() const
	{
		return (m_refCount + m_lockCount == 0);
	}
}