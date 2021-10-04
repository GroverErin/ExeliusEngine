#include "EXEPCH.h"
#include "source/resource/ResourceLoader.h"
#include "source/resource/ResourceListener.h"
#include "source/resource/ResourceFactory.h"
#include "source/resource/Resource.h"
#include "source/utility/io/File.h"
#include "source/utility/io/ZLIBStructs.h"

#include <zlib.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Constructor default initializes member data.
	/// </summary>
	ResourceLoader::ResourceLoader()
		: m_resourceLoaderLog("ResourceLoader")
		, m_pResourceFactory(nullptr)
		, m_quitThread(false)
		, m_successfulThreadShutdown(false)
		, m_engineResourcePath("Invalid Engine Resource Path.")
		, m_useRawAssets(false)
	{
		//
	}

	/// <summary>
	/// Destructor handles the safe shutdown and destruction of
	/// resources and the resource thread.
	/// </summary>
	ResourceLoader::~ResourceLoader()
	{
		// Remove resources to be loaded.
		m_deferredQueueLock.lock();
		m_deferredQueue.clear();
		m_deferredQueueLock.unlock();

		// Remove listeners waiting on resources.
		m_listenerMapLock.lock();
		m_deferredResourceListenersMap.clear();
		m_listenerMapLock.unlock();

		// Tell the thread we are done.
		m_quitThread = true;
		SignalAndWaitForLoaderThread();
		m_loaderThread.join();

		// Unload any assets that were added to this queue during
		// engine shutdown processes.
		ProcessUnloadQueue();

		// Don't delete, this lives on the Application/Engine.
		// I have decided that the destruction of the factory
		// makes more sense to happen in the same class that
		// created it. TODO: Consider making it a smart ptr.
		m_pResourceFactory = nullptr;
	}

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
	bool ResourceLoader::Initialize(ResourceFactory* pResourceFactory, const char* pEngineResourcePath, bool useRawAssets)
	{
		// Default resource factory MUST exist.
		EXE_ASSERT(pResourceFactory);
		m_pResourceFactory = pResourceFactory;

		if (pEngineResourcePath)
			m_engineResourcePath = pEngineResourcePath;

		m_useRawAssets = useRawAssets;

		// Should not contain data, but just in case.
		m_deferredQueueLock.lock();
		m_deferredQueue.clear();
		m_deferredQueueLock.unlock();

		m_listenerMapLock.lock();
		m_deferredResourceListenersMap.clear();
		m_listenerMapLock.unlock();

		// Spin up the loader thread.
		m_loaderThread = std::thread(&ResourceLoader::ProcessResourceQueue, this);

		return true;
	}

	/// <summary>
	/// Process any resources that have been marked for removal.
	/// This happens once per frame and should not be called by
	/// the client under any circumstances.
	/// 
	/// @todo
	/// See if there is a way to limit accessibility to this function.
	/// </summary>
	void ResourceLoader::ProcessUnloadQueue()
	{
		m_resourceDatabase.ProcessUnloadQueue();
	}

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
	void ResourceLoader::QueueLoad(const ResourceID& resourceID, bool signalLoaderThread, ResourceListenerPtr pListener)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXE_ASSERT(m_loaderThread.joinable());

		m_resourceLoaderLog.Trace("Queueing Resource: {}", resourceID.Get().c_str());

		// Attempt to create a resource entry.
		if (m_resourceDatabase.CreateEntry(resourceID))
		{
			m_resourceLoaderLog.Trace("Created new resource entry.");

			m_listenerMapLock.lock();
			m_deferredResourceListenersMap[resourceID].emplace_back(pListener);
			m_listenerMapLock.unlock();
		}
		else if (m_resourceDatabase.GetEntryLoadStatus(resourceID) == ResourceLoadStatus::kLoaded)
		{
			m_resourceLoaderLog.Trace("Resource already loaded.");

			// This may seem unnecessary, but it is a catch in case no listener was passed in.
			if (!pListener.expired())
				pListener.lock()->OnResourceLoaded(resourceID);

			return; // Bail here. We do NOT want to change the status of the resource.
		}
		else if (m_resourceDatabase.GetEntryLoadStatus(resourceID) == ResourceLoadStatus::kLoading)
		{
			m_resourceLoaderLog.Trace("Resource already queued.");
			m_resourceDatabase.IncrementEntryRefCount(resourceID);
			return; // Bail here. We do NOT want to change the status of the resource.
		}
		else
		{
			m_resourceLoaderLog.Warn("Resource was unloaded or unloading. SEE TODO in ResourceLoader.cpp.");
			/// TODO:
			///		It is possible that neither of the 2 branches were entered.
			///		This means that the Resource exists in the database && its
			///		LoadStatus is neither Loaded or Loading, possibly Unloading
			///		or Unloaded. This could cause an issue in the future if not
			///		carefully protected against. For example: The pListener will
			///		not be called in this case, which is already a bug. Hack fix
			///		is as follows.
			///	EDIT:
			///		I *think* that the resource would have it's ref count incremented
			///		already, but that may be something to think about here.
			m_listenerMapLock.lock();
			m_deferredResourceListenersMap[resourceID].emplace_back(pListener);
			m_listenerMapLock.unlock();
		}

		// If we get here, then the resource is now loading.
		m_resourceDatabase.SetEntryLoadStatus(resourceID, ResourceLoadStatus::kLoading);
		
		m_deferredQueueLock.lock();
		m_deferredQueue.emplace_back(resourceID);
		m_deferredQueueLock.unlock();

		if (signalLoaderThread)
			SignalLoaderThread();

		m_resourceLoaderLog.Trace("QueueLoad Complete.");
	}

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
	void ResourceLoader::LoadNow(const ResourceID& resourceID, ResourceListenerPtr pListener)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceLoaderLog.Trace("Loading Resource On Main Thread: {}", resourceID.Get().c_str());

		// Check if the resource is already in the resource database.
		if (m_resourceDatabase.CreateEntry(resourceID))
		{
			m_resourceLoaderLog.Trace("Created new resource entry.");

			m_listenerMapLock.lock();
			m_deferredResourceListenersMap[resourceID].emplace_back(pListener);
			m_listenerMapLock.unlock();
		}
		else if (m_resourceDatabase.GetEntryLoadStatus(resourceID) == ResourceLoadStatus::kLoaded)
		{
			m_resourceLoaderLog.Trace("Resource already loaded.");

			// This may seem unnecessary, but it is a catch in case no listener was passed in.
			if (!pListener.expired())
				pListener.lock()->OnResourceLoaded(resourceID);

			return;
		}
		else if (m_resourceDatabase.GetEntryLoadStatus(resourceID) == ResourceLoadStatus::kLoading)
		{
			m_resourceLoaderLog.Trace("Resource already queued.");
			// TODO: We may want to consider removing the queued load
			//		 then forcing the load here.
			return; // Bail here. We do NOT want to change the status of the resource.
		}

		m_resourceDatabase.SetEntryLoadStatus(resourceID, ResourceLoadStatus::kLoading);
		LoadResource(resourceID);

		if (!pListener.expired()) // This may seem unnecessary, but it is a catch in case no listener was passed in.
			pListener.lock()->OnResourceLoaded(resourceID);

		m_resourceLoaderLog.Trace("Load Complete.");
	}

	/// <summary>
	/// Checks the load status of a resource. If it is loading or loaded
	/// then it is a valid resource to be acquired.
	/// </summary>
	/// <param name="resourceID">- The resource to check.</param>
	/// <returns>True if the resource is acquirable, false otherwise.</returns>
	bool ResourceLoader::IsResourceAcquirable(const ResourceID& resourceID)
	{
		auto status = m_resourceDatabase.GetEntryLoadStatus(resourceID);

		if (status == ResourceLoadStatus::kLoading)
			return false;
		else if (status == ResourceLoadStatus::kLoaded)
			return true;

		return false;
	}

	/// <summary>
	/// Increments the reference count on the given resource.
	/// </summary>
	/// <param name="resourceID">- The resource to acquire.</param>
	void ResourceLoader::AcquireResource(const ResourceID& resourceID)
	{
		m_resourceDatabase.IncrementEntryRefCount(resourceID);
	}

	/// <summary>
	/// Decrements the reference count on the given resource.
	/// If there are no longer any references or locks on the
	/// given resource it will be unloaded when the unload queue
	/// is processed next.
	/// </summary>
	/// <param name="resourceID">- The resource to release.</param>
	void ResourceLoader::ReleaseResource(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceLoaderLog.Trace("Releasing Resource: {}", resourceID.Get().c_str());

		// Decrement the reference count of this resource.
		// If there is no longer any references to this resource, then unload it.
		if (m_resourceDatabase.DecrementEntryRefCount(resourceID))
			m_resourceDatabase.UnloadEntry(resourceID);

		m_resourceLoaderLog.Trace("Release Complete.");
	}

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
	void ResourceLoader::ReloadResource(const ResourceID& resourceID, bool forceLoad, ResourceListenerPtr pListener)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceLoaderLog.Trace("Reloading: {}", resourceID.Get().c_str());

		// Check if the resource is not already loaded.
		if (m_resourceDatabase.GetEntryLoadStatus(resourceID) != ResourceLoadStatus::kLoaded)
		{
			m_resourceLoaderLog.Warn("Attempted to Reload Resource that is not loaded. Queueing/Loading now.");

			if (forceLoad)
				LoadNow(resourceID, pListener);
			else
				QueueLoad(resourceID, true, pListener);
			return;
		}

		// TODO:
		//	This may be unnecesary and could potentially lead to bugs.
		//	This will heavily depend on the handling of resource lifetimes/accessors.
		m_resourceDatabase.UnloadEntry(resourceID);

		if (forceLoad)
			LoadNow(resourceID, pListener);
		else
			QueueLoad(resourceID, true, pListener);

		m_resourceLoaderLog.Trace("Reload Complete.");
	}

	/// <summary>
	/// Retrieve the given resource from the resource database.
	/// The resource can optionally be forced to load if it is
	/// not currently loaded.
	/// </summary>
	/// <param name="resourceID">- The resource to retrieve.</param>
	/// <param name="forceLoad">- If true, the resource will be loaded on the calling thread if not already loaded.</param>
	/// <returns>The resource with the given ResourceID. nullptr if not found and forceload == false, or if resource is still loading.</returns>
	Resource* ResourceLoader::GetResource(const ResourceID& resourceID, bool forceLoad)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceLoaderLog.Trace("Attempting to Retrieve Resource: {}", resourceID.Get().c_str());

		if (m_resourceDatabase.GetEntryLoadStatus(resourceID) == ResourceLoadStatus::kLoading)
		{
			m_resourceLoaderLog.Info("Resource Request Denied: Resource Still Loading.");
			return nullptr;
		}

		Resource* pResource = m_resourceDatabase.GetEntryResource(resourceID);

		if (!pResource && forceLoad)
		{
			m_resourceLoaderLog.Info("Forcing Resource Creation and Retrieving.");
			LoadNow(resourceID);
			return GetResource(resourceID, false); // Should be guaranteed, but false will prevent infinite recursion.
		}
		else if (!pResource)
		{
			m_resourceLoaderLog.Info("Resource not found.");
			return nullptr;
		}

		m_resourceLoaderLog.Trace("Resource Retrieved.");
		return pResource;
	}

	/// <summary>
	/// Increments the lock count of the given resource.
	/// This prevents a resource from being unloaded automatically.
	/// UnlockResource should always be called if LockResource has
	/// been called.
	/// </summary>
	/// <param name="resourceID">- The resource to lock.</param>
	void ResourceLoader::LockResource(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceLoaderLog.Trace("Locking Resource: {}", resourceID.Get().c_str());

		m_resourceDatabase.IncrementEntryLockCount(resourceID);
		m_resourceLoaderLog.Trace("Resource Locked.");
	}

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
	void ResourceLoader::UnlockResource(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceLoaderLog.Trace("Unlocking Resource: {}", resourceID.Get().c_str());

		// Decrement the reference count of this resource.
		// If there is no longer any references to this resource, then unload it.
		if (m_resourceDatabase.DecrementEntryLockCount(resourceID))
			m_resourceDatabase.UnloadEntry(resourceID);
	}

	/// <summary>
	/// Signal the resource loader thread to wake up and begin
	/// loading resources.
	/// </summary>
	void ResourceLoader::SignalLoaderThread()
	{
		EXE_ASSERT(m_loaderThread.joinable());
		m_resourceLoaderLog.Trace("Signaling Loader Thread.");

		m_signalThread.notify_one();
	}

	/// <summary>
	/// Signal the resource loader thread to wake up and begin
	/// loading resources. Sleeps the calling thread until the
	/// loader thread signals upon completion.
	/// </summary>
	void ResourceLoader::SignalAndWaitForLoaderThread()
	{
		SignalLoaderThread();

		std::mutex waitMutex;
		std::unique_lock<std::mutex> waitLock(waitMutex);

		m_resourceLoaderLog.Info("Waiting for response from LoaderThread.");
		m_signalThread.wait(waitLock, [this]()
		{
			return !m_quitThread || m_successfulThreadShutdown;
		});
	}

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
	void ResourceLoader::ProcessResourceQueue()
	{
		m_resourceLoaderLog.Info("Instantiating Resource Loader Thread.");
		std::mutex waitMutex;

		// TODO: Consider if this is the best container to use here.
		eastl::deque<ResourceID> processingQueue;
		ListenersMap processingResourceListenersMap;
		std::unique_lock<std::mutex> waitLock(waitMutex);

		// While the thread is still active (Not shut down)
		while (!m_quitThread)
		{
			// Wait until we are signaled to work.
			m_signalThread.wait(waitLock, [this]()
			{
				bool empty = true;

				m_deferredQueueLock.lock();
				empty = m_deferredQueue.empty();
				m_deferredQueueLock.unlock();

				return m_quitThread || !empty;
			});

			m_resourceLoaderLog.Trace("Loader Thread Recieved Signal");

			// Don't do any work if we're exiting.
			if (m_quitThread)
				break;

			/// Capture the deferred queue into the processing queue (swap buffers)
			m_deferredQueueLock.lock();
			processingQueue.swap(m_deferredQueue);
			m_deferredQueueLock.unlock();

			m_listenerMapLock.lock();
			processingResourceListenersMap.swap(m_deferredResourceListenersMap);
			m_listenerMapLock.unlock();

			// Process the queue
			while (!processingQueue.empty())
			{
				m_resourceLoaderLog.Trace("Loader Thread Loading: {}", processingQueue.front().Get().c_str());
				LoadResource(processingQueue.front());

				// Notify all the listeners that we are done loading.
				for (auto& listener : processingResourceListenersMap[processingQueue.front()])
				{
					if (listener.expired())
						continue;

					listener.lock()->OnResourceLoaded(processingQueue.front());
				}
				processingResourceListenersMap[processingQueue.front()].clear();

				m_resourceLoaderLog.Trace("Loader Thread Loading Complete.");
				processingQueue.pop_front();
			}

			// Done loading this pass, so signal the main thread in case it is waiting.
			m_signalThread.notify_one();
			m_resourceLoaderLog.Trace("Signaled Main Thread: Queue Finished");
		}

		// Let the main thread know we are fully exiting in case they are waiting.
		m_successfulThreadShutdown = true;
		m_resourceLoaderLog.Info("Signaled Main Thread: Thread Terminating.");
		m_signalThread.notify_one();
	}

	/// <summary>
	/// Load the given resource. Upon failure, ResourceDatabase::UnloadEntry
	/// will be called, removing the entry from the database.
	/// </summary>
	/// <param name="resourceID">- The resource to load.</param>
	void ResourceLoader::LoadResource(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceLoaderLog.Trace("Loading Resource Internally: {}", resourceID.Get().c_str());

		// TODO:
		//	Remove use of vector maybe?
		eastl::vector<std::byte> rawData = LoadRawData(resourceID);

		if (rawData.empty())
		{
			m_resourceLoaderLog.Warn("Raw file data was empty.");

			// TODO:
			//	This may present an issue with this resource having references to it.

			m_resourceDatabase.UnloadEntry(resourceID);
			return;
		}

		Resource* pResource = m_pResourceFactory->CreateResource(resourceID);
		if (!pResource)
		{
			m_resourceLoaderLog.Warn("Failed to create resource from resource factory.");
			m_resourceDatabase.UnloadEntry(resourceID);
			return;
		}

		if (pResource->Load(std::move(rawData)) != Resource::LoadResult::kFailed)
		{
			m_resourceDatabase.SetEntryResource(resourceID, pResource);
		}
		else
		{
			m_resourceLoaderLog.Warn("Failed to load resource from raw data.");
			m_resourceDatabase.UnloadEntry(resourceID);
			return;
		}

		m_resourceDatabase.SetEntryLoadStatus(resourceID, ResourceLoadStatus::kLoaded);

		m_resourceLoaderLog.Trace("Completed Loading Internally.");
	}

	/// <summary>
	/// Selector that chooses to load the raw data of an asset
	/// from the filesystem or from a compressed package.
	/// </summary>
	/// <param name="resourceID">- The resource to load.</param>
	/// <returns>The loaded raw data in a vector of bytes. The vector will be empty on failure.</returns>
	eastl::vector<std::byte> ResourceLoader::LoadRawData(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		m_resourceLoaderLog.Trace("Loading Resource Raw Data: {}", resourceID.Get().c_str());

		if (m_useRawAssets)
		{
			return LoadFromDisk(resourceID);
		}
		else
		{
			return LoadFromZip(resourceID);
		}
	}

	/// <summary>
	/// Load the given resource directly from the filesystem.
	/// </summary>
	/// <param name="resourceID">- The resource to load.</param>
	/// <returns>The loaded raw data in a vector of bytes. The vector will be empty on failure.</returns>
	eastl::vector<std::byte> ResourceLoader::LoadFromDisk(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		eastl::string strPath = resourceID.Get();

		File resourceFile;
		if (!resourceFile.Open(strPath, File::AccessPermission::kReadOnly, File::CreationType::kOpenFile))
		{
			m_resourceLoaderLog.Warn("Failed to open file: {}", strPath.c_str());
			return eastl::vector<std::byte>();
		}

		eastl::vector<std::byte> resourceData(resourceFile.GetSize());
		size_t readBytes = resourceFile.Read(resourceData);

		if (readBytes != resourceFile.GetSize())
		{
			m_resourceLoaderLog.Warn("Failed to read '{}'. Read {} of {} bytes.", strPath.c_str(), readBytes, resourceFile.GetSize());
			return eastl::vector<std::byte>();
		}

		return resourceData;
	}

	/// <summary>
	/// Load the given resource directly from a compressed file.
	/// @todo
	/// Currently unused.
	/// </summary>
	/// <param name="resourceID">- The resource to load.</param>
	/// <returns>The loaded raw data in a vector of bytes. The vector will be empty on failure.</returns>
	eastl::vector<std::byte> ResourceLoader::LoadFromZip(const ResourceID& resourceID)
	{
		eastl::string strPath = resourceID.Get();
		// TODO: Do this.

		return eastl::vector<std::byte>();
	}
}