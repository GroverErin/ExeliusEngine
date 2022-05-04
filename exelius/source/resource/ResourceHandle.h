#pragma once
#include "source/resource/ResourceHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Resource;
	class ResourceListener;
	using ResourceListenerPtr = WeakPtr<ResourceListener>; // "Forward Declaring" ResourceListenerPtr from ResourceListener.h

	/// <summary>
	/// ResourceHandle acts as the interface for acquiring and manipulating resources.
	/// Resources are volatile and may not exist at any one point of execution. So,
	/// the use of this handle class is a requirement.
	/// </summary>
	class ResourceHandle
	{
		/// <summary>
		/// The ID that this handle refers to.
		/// </summary>
		ResourceID m_resourceID;

		/// <summary>
		/// Does this resource handle currently "refer" to a resource.
		/// By refer, it is meant to say "Has this handle incremented
		/// the reference count for a resource?"
		/// </summary>
		bool m_resourceHeld;
	public:
		/// <summary>
		/// Default construct a resource handle. The default construction
		/// does not set an ID to refer to, nor does it acquire a resource.
		/// This needs to be done manually.
		/// </summary>
		ResourceHandle();

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
		ResourceHandle(const ResourceID& resourceID, bool loadResource = false);
		ResourceHandle(const ResourceHandle& other);
		ResourceHandle(ResourceHandle&& other) noexcept;
		ResourceHandle& operator=(const ResourceHandle& other);
		ResourceHandle& operator=(ResourceHandle&& other) noexcept;

		/// <summary>
		/// Destroying the ResourceHandle will automatically release the
		/// resource if it has not already been done manually.
		/// </summary>
		~ResourceHandle();

		/// <summary>
		/// Get the resource that is referred to by this handle.
		/// If the resource has not yet been acquired, it will attempt to do so here.
		/// </summary>
		/// <param name="forceLoad">- True will load the resource immediately, on the main thread, if not already loaded. Default is false.</param>
		/// <returns>The resource with the ID held by this ResourceHandle, or nullptr if the resource could not be retrieved.</returns>
		Resource* Get(bool forceLoad = false);

		/// <summary>
		/// Templatized.
		/// Get the resource that is referred to by this handle.
		/// If the resource has not yet been acquired, it will attempt to do so here.
		/// </summary>
		/// <typeparam name="Type">- The resource Subclass to be retrieved.</typeparam>
		/// <param name="forceLoad">- True will load the resource immediately, on the main thread, if not already loaded. Default is false.</param>
		/// <returns>The resource with the ID held by this ResourceHandle, or nullptr if the resource could not be retrieved.</returns>
		template <class Type>
		Type* GetAs(bool forceLoad = false)
		{
			return dynamic_cast<Type*>(Get(forceLoad));
		}

		bool CreateNew(const ResourceID& resourceID);

		void SaveResource();

		/// <summary>
		/// Queues the resource for the loader thread to load if
		/// the resource has not already been loaded. This function
		/// will acquire the resource automatically.
		/// </summary>
		/// <param name="signalLoaderThread">- Should signal the loader thread to begin. Default is false.</param>
		/// <param name="pListener">- An object that inherets from ResourceListener to be notified of on load completion.</param>
		void QueueLoad(bool signalLoaderThread = false, ResourceListenerPtr pListener = ResourceListenerPtr());

		/// <summary>
		/// Loads the resource immediate on the main thread. This is a blocking
		/// function, and may be slow. Use with caution. This function
		/// will acquire the resource automatically.
		/// </summary>
		/// <param name="pListener">- An object that inherets from ResourceListener to be notified of on load completion.</param>
		void LoadNow(ResourceListenerPtr pListener = ResourceListenerPtr());

		/// <summary>
		/// Releases the resource if it has been acquired.
		/// 'Release' in this case means to decrement the reference count
		/// of a resource, and unload the resource in the case that there
		/// is no longer any references to the resource.
		/// 
		/// This function is called by the destructor.
		/// </summary>
		void Release();

		/// <summary>
		/// Sets an ID on a resource. Calling this function on a
		/// handle that has already acquired, and not released, a
		/// resource will do nothing.
		/// 
		/// A resource can only be set if and only if this handle
		/// is not holding a reference to a resource already.
		/// </summary>
		/// <param name="idToSet">- The resource ID that will be set.</param>
		void SetResourceID(const ResourceID& idToSet);

		/// <summary>
		/// Retrieve the ID this handle refers to.
		/// </summary>
		/// <returns>The ID of this resource handle.</returns>
		const ResourceID& GetID() const { return m_resourceID; }

		/// <summary>
		/// Check if this handle is currently holding a resource.
		/// </summary>
		/// <returns>True if the handle holds a resource, false otherwise.</returns>
		bool IsReferenceHeld() const { return m_resourceHeld; }

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
		void LockResource();

		#undef UnlockResource // This is defined in WinBase.h

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
		void UnlockResource();

		/// <summary>
		/// Compares the given ResourceID to the ResourceHandle's ResourceID.
		/// </summary>
		/// <param name="right">- The ResourceID to compare.</param>
		/// <returns>True if matching, false otherwise.</returns>
		bool operator==(const ResourceHandle& right) { return (m_resourceID == right.m_resourceID); }

		/// <summary>
		/// Compares the given ResourceID to the ResourceHandle's ResourceID.
		/// </summary>
		/// <param name="right">- The ResourceID to compare.</param>
		/// <returns>True if not matching, false otherwise.</returns>
		bool operator!=(const ResourceHandle& right) { return (m_resourceID != right.m_resourceID); }

	private:
		/// <summary>
		/// Try to acquire the resource referenced by this handle.
		/// </summary>
		/// <returns>True if acquired, false otherwise.</returns>
		bool TryToAcquireResource();
	};
}