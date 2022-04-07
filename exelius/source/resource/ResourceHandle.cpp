#include "EXEPCH.h"
#include "source/resource/ResourceHandle.h"
#include "source/resource/ResourceLoader.h"
#include "source/resource/ResourceListener.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Default construct a resource handle. The default construction
	/// does not set an ID to refer to, nor does it acquire a resource.
	/// This needs to be done manually.
	/// </summary>
	ResourceHandle::ResourceHandle()
		: m_resourceHeld(false)
	{
		//
	}

	/// <summary>
	/// This constructor will refer to the given ID and will attempt to
	/// "acquire" a the resource with the given ID. This means that it
	/// will increment the reference count of the given resource if it
	/// already exists. Otherwise it will fail to do so. If it does fail
	/// it will reattempt to acquire it upon each Get or GetAs call.
	/// 
	/// Finally, the constructor can force a resource to be loaded if
	/// it is not loaded, which will automatically acquire the resource.
	/// The load will happen immediately on the main thread.
	/// </summary>
	/// <param name="resourceID">- The resource ID the handle is meant to refer to.</param>
	/// <param name="loadResource">- If the resource should be loaded. Default is false.</param>
	ResourceHandle::ResourceHandle(const ResourceID& resourceID, bool loadResource)
		: m_resourceID(resourceID)
		, m_resourceHeld(false)
	{
		// Check if the resource is already loaded.
		if (!TryToAcquireResource() && loadResource)
		{
			ResourceLoader::GetInstance()->LoadNow(resourceID);
		}
	}

	/// <summary>
	/// Destroying the ResourceHandle will automatically release the
	/// resource if it has not already been done manually.
	/// </summary>
	ResourceHandle::~ResourceHandle()
	{
		Release();
	}

	/// <summary>
	/// Get the resource that is referred to by this handle.
	/// If the resource has not yet been acquired, it will attempt to do so here.
	/// </summary>
	/// <param name="forceLoad">- True will load the resource immediately, on the main thread, if not already loaded. Default is false.</param>
	/// <returns>The resource with the ID held by this ResourceHandle, or nullptr if the resource could not be retrieved.</returns>
	Resource* ResourceHandle::Get(bool forceLoad)
	{
		if (!m_resourceHeld)
		{
			TryToAcquireResource();
		}

		if (!m_resourceID.IsValid())
			return nullptr;

		Resource* pResource = ResourceLoader::GetInstance()->GetResource(m_resourceID, forceLoad);

		if (pResource && !m_resourceHeld)
			m_resourceHeld = true; // Prevents double acquisition.

		return pResource;
	}

	bool ResourceHandle::CreateNew(const ResourceID& resourceID)
	{
		if (m_resourceHeld)
		{
			EXE_LOG_CATEGORY_WARN("ResourceHandle", "Unable to create new resource in used resource handle. Release the current resource first.");
			return false;
		}

		if (!resourceID.IsValid())
		{
			EXE_LOG_CATEGORY_WARN("ResourceHandle", "Unable to create new resource. ResourceID is invalid.");
			return false;
		}

		if (ResourceLoader::GetInstance()->IsResourceAcquirable(resourceID))
		{
			EXE_LOG_CATEGORY_WARN("ResourceHandle", "Unable to create new resource. Resource already exists.");
			return false;
		}

		ResourceLoader::GetInstance()->CreateNewResource(resourceID);

		SetResourceID(resourceID);
		if (!TryToAcquireResource())
		{
			EXE_LOG_CATEGORY_WARN("ResourceHandle", "Unable to acquire new resource.");
			return false;
		}

		// Here we were able to acquire the resource, but we have unintentionally
		// incremented the ref count twice. Once for when the resource is created, and
		// another for the TryToAcquireResource() call. There are probably a ton of ways to
		// solve this, but the easiest way for now, is to decrement the ref count once here.
		// We should still own another reference to this resource, so this should be fine.
		ResourceLoader::GetInstance()->ReleaseResource(m_resourceID);

		return true;
	}

	void ResourceHandle::SaveResource()
	{
		ResourceLoader::GetInstance()->SaveResource(m_resourceID);
	}

	/// <summary>
	/// Queues the resource for the loader thread to load if
	/// the resource has not already been loaded. This function
	/// will acquire the resource automatically.
	/// </summary>
	/// <param name="signalLoaderThread">- Should signal the loader thread to begin. Default is false.</param>
	/// <param name="pListener">- An object that inherets from ResourceListener to be notified of on load completion.</param>
	void ResourceHandle::QueueLoad(bool signalLoaderThread, ResourceListenerPtr pListener)
	{
		if (m_resourceHeld)
		{
			EXE_LOG_CATEGORY_INFO("ResourceHandle", "Resource with id '{}' cannot be loaded, this ResourceHandle already holds a resource.", m_resourceID.Get().c_str());
			return;
		}

		if (!m_resourceID.IsValid())
		{
			EXE_LOG_CATEGORY_INFO("ResourceHandle", "Resource cannot be Queued for load, resource ID is invalid or not set.");
			return;
		}

		if (ResourceLoader::GetInstance()->IsResourceAcquirable(m_resourceID))
		{
			if (TryToAcquireResource())
				return; // Return because we don't need to load.
		}

		ResourceLoader::GetInstance()->QueueLoad(m_resourceID, signalLoaderThread, pListener);
		m_resourceHeld = true;
	}

	/// <summary>
	/// Loads the resource immediate on the main thread. This is a blocking
	/// function, and may be slow. Use with caution. This function
	/// will acquire the resource automatically.
	/// </summary>
	/// <param name="pListener">- An object that inherets from ResourceListener to be notified of on load completion.</param>
	void ResourceHandle::LoadNow(ResourceListenerPtr pListener)
	{
		if (m_resourceHeld)
		{
			EXE_LOG_CATEGORY_INFO("ResourceHandle", "Resource with id '{}' cannot be loaded, this ResourceHandle already holds a resource.", m_resourceID.Get().c_str());
			return;
		}

		if (!m_resourceID.IsValid())
		{
			EXE_LOG_CATEGORY_INFO("ResourceHandle", "Resource cannot be loaded, resource ID is invalid or not set.");
			return;
		}

		if (ResourceLoader::GetInstance()->IsResourceAcquirable(m_resourceID))
		{
			if (TryToAcquireResource())
				return; // Return because we don't need to load.
		}

		ResourceLoader::GetInstance()->LoadNow(m_resourceID);
		m_resourceHeld = true;
	}

	/// <summary>
	/// Releases the resource if it has been acquired.
	/// 'Release' in this case means to decrement the reference count
	/// of a resource, and unload the resource in the case that there
	/// is no longer any references to the resource.
	/// 
	/// This function is called by the destructor.
	/// </summary>
	void ResourceHandle::Release()
	{
		if (!m_resourceHeld)
			return;

		ResourceLoader::GetInstance()->ReleaseResource(m_resourceID);
		m_resourceHeld = false;
	}

	/// <summary>
	/// Sets an ID on a resource. Calling this function on a
	/// handle that has already acquired, and not released, a
	/// resource will do nothing.
	/// 
	/// A resource can only be set if and only if this handle
	/// is not holding a reference to a resource already.
	/// </summary>
	/// <param name="idToSet">- The resource ID that will be set.</param>
	void ResourceHandle::SetResourceID(const ResourceID& idToSet)
	{
		if (m_resourceHeld)
			return;

		m_resourceID = idToSet;
	}

	/// <summary>
	/// Increments the lock count of the held resource, if possible.
	/// This will prevent a resource from being unloaded, even if there
	/// is no longer any references held to the resource. This handle
	/// can be destroyed after calling Lock and the resource will not
	/// deallocate, only calling Unlock with a handle that has the same
	/// ID will allow a resource to deallocate.
	/// @note
	/// Do not forget to call unlock on the resource, as this does NOT
	/// happen automatically.
	/// </summary>
	void ResourceHandle::LockResource()
	{
		ResourceLoader::GetInstance()->LockResource(m_resourceID);
	}

	/// <summary>
	/// Decrements the lock count of the held resource, if possible.
	/// This allows a resource to deallocate once there are no longer
	/// any held references to the resource, and there are no other
	/// locks in place on the resource.
	/// 
	/// @note
	/// Failing to unlock a resource may cause increased memory
	/// usage.
	/// </summary>
	void ResourceHandle::UnlockResource()
	{
		ResourceLoader::GetInstance()->UnlockResource(m_resourceID);
	}

	/// <summary>
	/// Try to acquire the resource referenced by this handle.
	/// </summary>
	/// <returns>True if acquired, false otherwise.</returns>
	bool ResourceHandle::TryToAcquireResource()
	{
		if (m_resourceHeld)
		{
			EXE_LOG_CATEGORY_INFO("ResourceHandle", "Resource with id '{}' cannot be acquired, this ResourceHandle already holds a resource.", m_resourceID.Get().c_str());
			return false;
		}

		if (!m_resourceID.IsValid())
		{
			return false; // We don't log here because it is a common case.
		}

		if (!ResourceLoader::GetInstance()->IsResourceAcquirable(m_resourceID))
		{
			EXE_LOG_CATEGORY_TRACE("ResourceHandle", "Resource cannot be acquired, resource not available.");
			return false;
		}

		ResourceLoader::GetInstance()->AcquireResource(m_resourceID);
		m_resourceHeld = true;
		return true;
	}
}