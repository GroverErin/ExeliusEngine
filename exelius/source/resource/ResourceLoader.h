#pragma once
#include "source/utility/generic/Singleton.h"
#include "source/resource/ResourceDatabase.h"
#include "source/debug/Log.h"

#include <EASTL/deque.h>
#include <EASTL/vector.h>

#define FORCE_SINGLE_THREADED_RESOURCE_LOADER 1

#if !FORCE_SINGLE_THREADED_RESOURCE_LOADER
	#include <thread>
	#include <condition_variable>
	#include <atomic>
	#include <mutex>
#endif // !FORCE_SINGLE_THREADED_RESOURCE_LOADER


/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class ResourceFactory;
	class ResourceListener;
	using ResourceListenerPtr = eastl::weak_ptr<ResourceListener>; // "Forward Declaring" ResourceListenerPtr from ResourceListener.h

	/// <summary>
	/// Loading of resources is managed by this class. Resources can
	/// be loaded in the main thread or on a separate dedicated thread.
	/// 
	/// The threaded loading is implemented via a Double Buffered System.
	/// 
	/// The resource loader then calls on the resource factory to
	/// create the specific type of resources based on criteria defined
	/// by the factory. For the engine's factory, this is done by
	/// extracting the file extention.
	/// @see ResourceFactory
	/// @see ExeliusResourceFactory
	/// 
	/// Lastly, the loaded resource is then stored as a reference counted
	/// object within a resource database. This allows for resources to
	/// have a lifetime determined by their use.
	/// @see ResourceDatabase
	/// 
	/// The resource loader should not necessarily need to be used by
	/// client applications directly. The interface ResourceHandle is
	/// provided and will handle this functionality in such a way that
	/// it is safe for the user as resources have such a volatile
	/// lifetime.
	/// @see ResourceHandle
	/// 
	/// @todo
	/// The resource loader needs some additional functionality:
	/// 1) The ability to load assets from a compressed file
	/// is not functional.
	/// 2) The use of engine and client resources is not in
	/// use.
	/// 3) The engine should be able to determine whether or
	/// not to load from a zip automatically via the data provided
	/// by the config file.
	/// 4) The resource loader does not make use of the
	/// config file currently.
	/// 5) The resource loader should be capable of being
	/// forced into single threaded mode.
	/// 6) The resource loader should allow for hot-reloading
	/// of assets in some form, whether that is automatic or manual.
	/// This could also be a user setting as well.
	/// 7) The loader should allow multiple listeners of a resource
	/// to be passed in to a single resource load call.
	/// </summary>
	class ResourceLoader
		: public Singleton<ResourceLoader>
	{
		/// <summary>
		/// A vector of objects listening for a notification on resource load.
		/// </summary>
		using ResourceListeners = eastl::vector<ResourceListenerPtr>;

		/// <summary>
		/// A map of listeners keyed by resource ID, allowing multiple listeners
		/// to listen for a single resource being loaded.
		/// </summary>
		using ListenersMap = eastl::unordered_map<ResourceID, ResourceListeners>;

		/// <summary>
		/// ResourceLoader Log.
		/// </summary>
		Log m_resourceLoaderLog;

		/// <summary>
		/// A map of listeners keyed by resource ID, allowing multiple listeners
		/// to listen for a single resource being loaded.
		/// </summary>
		ListenersMap m_deferredResourceListenersMap;

		#if !FORCE_SINGLE_THREADED_RESOURCE_LOADER
		/// <summary>
		/// A mutex used to protect the listeners from data race conditions.
		/// This allows the main thread and resource thread to swap maps
		/// safely.
		/// </summary>
		std::mutex m_listenerMapLock;
		#endif // !FORCE_SINGLE_THREADED_RESOURCE_LOADER

		/// <summary>
		/// The resource factory as defined by either the Engine or the Client.
		/// @see Application
		/// </summary>
		ResourceFactory* m_pResourceFactory;

		#if !FORCE_SINGLE_THREADED_RESOURCE_LOADER
		/// <summary>
		/// The dedicated resource loading thread.
		/// </summary>
		std::thread m_loaderThread;

		/// <summary>
		/// To shutdown the loader thread or continue operations.
		/// The thread will shutdown if this is true.
		/// </summary>
		std::atomic_bool m_quitThread;

		/// <summary>
		/// The loader thread will set this to true on successfully
		/// shutting down. This prevents a deadlock in the case
		/// that the thread shuts down prior to the main thread
		/// signaling and waiting for the shutdown to complete.
		/// </summary>
		std::atomic_bool m_successfulThreadShutdown;

		/// <summary>
		/// Signal for when the resource queue needs to be processed
		/// or to signal the main thread that the resource thread has
		/// completed processing.
		/// </summary>
		std::condition_variable m_signalThread;

		/// <summary>
		/// A mutex used to protect the queue from data race conditions.
		/// This allows the main thread and resource thread to swap queues
		/// safely.
		/// </summary>
		std::mutex m_deferredQueueLock;
		#endif // !FORCE_SINGLE_THREADED_RESOURCE_LOADER

		/// <summary>
		/// The resource database containing all the managed resources.
		/// Manages the lifetime of the resources automatically in
		/// conjunction with the ResourceHandle.
		/// @see ResourceDatabase
		/// @see ResourceHandle
		/// </summary>
		ResourceDatabase m_resourceDatabase;

		/// <summary>
		/// The queue containing the resources to be loaded.
		/// This will be swapped safely onto the resource
		/// thread.
		/// @todo
		/// Consider whether or not this is the best container
		/// to use here.
		/// </summary>
		eastl::deque<ResourceID> m_deferredQueue;

		/// <summary>
		/// The path containing the engine specific resources,
		/// for example: The engine logo for the splash screen.
		/// @todo
		/// Currently unused.
		/// </summary>
		eastl::string m_engineResourcePath;

		/// <summary>
		/// The path containing the client app specific resources,
		/// for example: A tilemap for a game.
		/// @todo
		/// Currently unused.
		/// </summary>
		eastl::string m_clientResourcePath;

		/// <summary>
		/// This will determine if the engine is expecting a compressed
		/// asset pack or raw assets. True means it will expect raw assets.
		/// </summary>
		bool m_useRawAssets;

	public:
		/// <summary>
		/// Constructor default initializes member data.
		/// </summary>
		ResourceLoader();
		ResourceLoader(const ResourceLoader&) = delete;
		ResourceLoader(ResourceLoader&&) = delete;
		ResourceLoader& operator=(const ResourceLoader&) = delete;
		ResourceLoader& operator=(ResourceLoader&&) = delete;

		/// <summary>
		/// Destructor handles the safe shutdown and destruction of
		/// resources and the resource thread.
		/// </summary>
		~ResourceLoader();

		/// <summary>
		/// Initialization sets the resource factory given by the application,
		/// sets the path to the engine and client resources, determines the
		/// way resources are retrieved and spins up the resource thread.
		/// The data provided for this function is retrieved from the config
		/// file in, and this function is called from, the Application.
		/// @see Application
		/// 
		/// @todo
		/// See if there is a way to limit accessibility to this function.
		/// </summary>
		/// <param name="pResourceFactory">- The desired resource factory to be used when creating resources.</param>
		/// <param name="pEngineResourcePath">- The path that the resource loader will look to load engine assets.</param>
		/// <param name="useRawAssets">- Whether or not the engine will be loading assets from a compressed file.</param>
		/// <returns>True if initialization was successful, false otherwise.</returns>
		bool Initialize(ResourceFactory* pResourceFactory, const char* pEngineResourcePath = nullptr, bool useRawAssets = false);

		/// <summary>
		/// Process any resources that have been marked for removal.
		/// This happens once per frame and should not be called by
		/// the client under any circumstances.
		/// 
		/// @todo
		/// See if there is a way to limit accessibility to this function.
		/// </summary>
		void ProcessUnloadQueue();

		/// <summary>
		/// Queue the resource to be loaded. This will defer the load to
		/// the resource thread if running in multithreaded mode. If a
		/// resource is already being loaded then the resource listener
		/// will be added to the list of listeners to be notified and
		/// nothing further will happen. If the resource is loaded already
		/// then the listener will be notified immediately.
		/// @todo
		/// Potential data race here, if the listener is added just after
		/// the loader thread proccesses the listener queue.
		/// 
		/// The loading time of a resource is not predictable, and thus
		/// the use of the ResourceListener class is highly reccomended
		/// here. (Less so in the single thread mode, but still recommended.)
		/// @see ResourceListener
		/// 
		/// @todo
		/// This could allow for a list of listeners to be passed in,
		/// so that multiple listeners can be notified of a resource being loaded.
		/// </summary>
		/// <param name="resourceID">- The filepath of the resource to load. This is a StringIntern for optimization.</param>
		/// <param name="signalLoaderThread">- True if the loader thread should be signaled to begin loading. Does nothing in single threaded mode.</param>
		/// <param name="pListener">- The listener to be notified when a resource has completed the load process.</param>
		void QueueLoad(const ResourceID& resourceID, bool signalLoaderThread = false, ResourceListenerPtr pListener = ResourceListenerPtr());

		/// <summary>
		/// Load the given resource immediately. This will happen on the
		/// calling thread and will be blocking on that thread until the
		/// resource has completed the load process. If a resource is
		/// already being loaded then the resource listener will be added
		/// to the list of listeners to be notified and nothing further
		/// will happen. If the resource is loaded already then the
		/// listener will be notified immediately.
		/// @todo
		/// Potential data race here, if the listener is added just after
		/// the loader thread proccesses the listener queue.
		/// @todo
		/// A resource that is already queued should be removed from the
		/// queue and forceloaded here.
		/// 
		/// The loading time of a resource is not predictable, and thus
		/// the use of the ResourceListener class is highly recomended
		/// here. (Less so in the single thread mode, but still recommended.)
		/// @see ResourceListener
		/// 
		/// @todo
		/// This could allow for a list of listeners to be passed in,
		/// so that multiple listeners can be notified of a resource being loaded.
		/// </summary>
		/// <param name="resourceID">- The filepath of the resource to load. This is a StringIntern for optimization</param>
		/// <param name="pListener">- The listener to be notified when a resource has completed the load process.</param>
		void LoadNow(const ResourceID& resourceID, ResourceListenerPtr pListener = ResourceListenerPtr());

		/// <summary>
		/// Checks the load status of a resource. If it is loading or loaded
		/// then it is a valid resource to be acquired.
		/// </summary>
		/// <param name="resourceID">- The resource to check.</param>
		/// <returns>True if the resource is acquirable, false otherwise.</returns>
		bool IsResourceAcquirable(const ResourceID& resourceID);

		/// <summary>
		/// Increments the reference count on the given resource.
		/// </summary>
		/// <param name="resourceID">- The resource to acquire.</param>
		void AcquireResource(const ResourceID& resourceID);

		/// <summary>
		/// Decrements the reference count on the given resource.
		/// If there are no longer any references or locks on the
		/// given resource it will be unloaded when the unload queue
		/// is processed next.
		/// </summary>
		/// <param name="resourceID">- The resource to release.</param>
		void ReleaseResource(const ResourceID& resourceID);

		/// <summary>
		/// If a resource is already loaded it will be unloaded and
		/// loaded again. Otherwise it will be loaded as normal.
		/// 
		/// @todo
		/// This function probably won't work and needs a bit of TLC.
		/// </summary>
		/// <param name="resourceID">- The resource to reload.</param>
		/// <param name="forceLoad">- If true, the resource will load immediately on the calling thread. Otherwise it will load when the loader thread does so.</param>
		/// <param name="pListener">- The listener to be notified of load completion.</param>
		void ReloadResource(const ResourceID& resourceID, bool forceLoad = false, ResourceListenerPtr pListener = ResourceListenerPtr());

		/// <summary>
		/// Retrieve the given resource from the resource database.
		/// The resource can optionally be forced to load if it is
		/// not currently loaded.
		/// </summary>
		/// <param name="resourceID">- The resource to retrieve.</param>
		/// <param name="forceLoad">- If true, the resource will be loaded on the calling thread if not already loaded.</param>
		/// <returns>The resource with the given ResourceID. nullptr if not found and forceload == false, or if resource is still loading.</returns>
		Resource* GetResource(const ResourceID& resourceID, bool forceLoad = false);

		/// <summary>
		/// Increments the lock count of the given resource.
		/// This prevents a resource from being unloaded automatically.
		/// UnlockResource should always be called if LockResource has
		/// been called.
		/// </summary>
		/// <param name="resourceID">- The resource to lock.</param>
		void LockResource(const ResourceID& resourceID);

		#undef UnlockResource // This is defined in WinBase.h

		/// <summary>
		/// Decrements the lock count of the given resource.
		/// This allows the engine to automatically unload a
		/// resource when the reference count reaches 0 and
		/// there are no other locks in place on the same
		/// resource.
		/// Failing to call UnlockResource on a Locked
		/// resource should be considered a bug.
		/// </summary>
		/// <param name="resourceID">- The resource to unlock.</param>
		void UnlockResource(const ResourceID& resourceID);

		/// <summary>
		/// Allows the resource system to switch between using raw and pack resources.
		/// </summary>
		/// <param name="useRawAssets">- If true, the system will use raw assets, false will use packed resources.</param>
		void SetUsingRawAssets(bool useRawAssets) { m_useRawAssets = useRawAssets; }

		/// <summary>
		/// Retrieve the path the system is using to load engine specific resources.
		/// </summary>
		/// <returns>The engine resource filepath.</returns>
		const eastl::string& GetEngineResourcePath() const { return m_engineResourcePath; }

		/// <summary>
		/// Retrieve the path the system is using to load client specific resources.
		/// </summary>
		/// <returns>The client resource filepath.</returns>
		const eastl::string& GetClientResourcePath() const { return m_clientResourcePath; }

		/// <summary>
		/// Sets the path to load client specific resources from.
		/// </summary>
		/// <param name="newClientResourcePath">- The new path to set.</param>
		void SetClientResourcePath(const eastl::string& newClientResourcePath) { m_clientResourcePath = newClientResourcePath; }

	private:
		/// <summary>
		/// Signal the resource loader thread to wake up and begin
		/// loading resources.
		/// </summary>
		void SignalLoaderThread();

		/// <summary>
		/// Signal the resource loader thread to wake up and begin
		/// loading resources. Sleeps the calling thread until the
		/// loader thread signals upon completion.
		/// </summary>
		void SignalAndWaitForLoaderThread();

		/// <summary>
		/// This is the instantiation function for the loader thread.
		/// 
		/// Double Buffered Resource Loading System:
		/// 
		/// The main thread will add resources and listners to a 
		/// couple of queues to be loaded and called, respectively.
		/// 
		/// When signaled, the loader thread will swap the deferred
		/// queues to be processed, allowing the main thread to continue
		/// to queue resources and listeners.
		/// 
		/// After the swap, the loader thread will load any queued
		/// resources from either raw asset files or a compressed
		/// asset package. Once a resource is loaded, the listeners
		/// attached to that resource will be called.
		/// 
		/// Once the loader thread has completed this process, it will
		/// signal to the main thread that it has completed, and sleep
		/// until the next signal to begin processing.
		/// 
		/// This thread will then have access to the following functions
		/// and thus these functions should be used with this in mind:
		///		ResourceLoader::LoadResource()
		/// 	ResourceLoader::LoadRawData()
		/// 	ResourceLoader::LoadFromDisk()
		/// 	ResourceLoader::LoadFromZip()
		/// 	ResourceDatabase::SetEntryLoadStatus()
		///		ResourceDatabase::SetEntryResource()
		///		ResourceDatabase::UnloadEntry()
		///		ResourceEntry::SetResource()
		/// 	ResourceEntry::SetStatus()
		/// 	ResourceFactory::CreateResource()*
		/// 	ResourceFactory::GetResourceType()*
		/// 	ResourceListener::OnResourceLoaded()*
		/// 	Resource::Load()*
		/// 
		/// *These can be inhereted from within client applications so
		/// special care needs to be made here.
		/// </summary>
		void ProcessResourceQueue();

		/// <summary>
		/// Load the given resource. Upon failure, ResourceDatabase::UnloadEntry
		/// will be called, removing the entry from the database.
		/// </summary>
		/// <param name="resourceID">- The resource to load.</param>
		void LoadResource(const ResourceID& resourceID);

		/// <summary>
		/// Selector that chooses to load the raw data of an asset
		/// from the filesystem or from a compressed package.
		/// </summary>
		/// <param name="resourceID">- The resource to load.</param>
		/// <returns>The loaded raw data in a vector of bytes. The vector will be empty on failure.</returns>
		eastl::vector<std::byte> LoadRawData(const ResourceID& resourceID);

		/// <summary>
		/// Load the given resource directly from the filesystem.
		/// </summary>
		/// <param name="resourceID">- The resource to load.</param>
		/// <returns>The loaded raw data in a vector of bytes. The vector will be empty on failure.</returns>
		eastl::vector<std::byte> LoadFromDisk(const ResourceID& resourceID);

		/// <summary>
		/// Load the given resource directly from a compressed file.
		/// @todo
		/// Currently unused.
		/// </summary>
		/// <param name="resourceID">- The resource to load.</param>
		/// <returns>The loaded raw data in a vector of bytes. The vector will be empty on failure.</returns>
		eastl::vector<std::byte> LoadFromZip(const ResourceID& resourceID);
	};
}