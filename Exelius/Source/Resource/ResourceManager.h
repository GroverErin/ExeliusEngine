#pragma once

#include "Source/Resource/Resources/Resource.h"

#include <unordered_map>
#include <deque>

#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>

namespace Exelius
{
	/// <summary>
	/// Manager of all resources used by Exelius and Client Applications.
	/// This system can run as a single threaded or multithreaded system.
	/// When multithreaded, the system uses a double buffered queue when
	/// performing read and write operations. When the system is using a
	/// single thread (operating on the main thread) the read and writes
	/// queue is differed to a point in the main loop to be handled.
	/// 
	/// Resources can only be loaded once, and the reference count for
	/// them are incremented when "retrieved", or decremented when "freed".
	/// The resource lifetime is managed this way, and are unloaded when
	/// there is not any remaining references to the resource.
	/// 
	/// The Resource Manager is able to load resources raw from the file
	/// system, with the benefit of checking on demand for changes in
	/// the files, enabling hot-reloading of assets. Also, resources can
	/// be batched together into a single compressed package for release
	/// builds. (Not sure yet if the Resource Manager itself will handle
	/// this batching)
	/// 
	/// Possible Example of Use:
	/// 
	/// pActor->AddComponent<Texture>(static_cast<TextureResource>
	///		(ResourceManager::GetInstance()->GetResource("TestPic.png")));
	/// 
	/// </summary>
	class ResourceManager
	{
		friend class Application;
		// The different tasks that the thread can perform on a resource
		enum class Task
		{
			LOAD,
			UNLOAD
		};

		// The thread that does the actual work of the Resource Manager.
		std::thread m_loaderThread;

		std::atomic_bool m_quitThread;
		std::condition_variable m_signalThread;
		std::mutex m_waitMutex;
		std::mutex m_processingQueueLock;
		std::mutex m_deferredQueueLock;

		std::string m_engineResourcePath;
		std::string m_clientResourcePath;
		bool m_useRawAssets;
		bool m_isMultiThreaded;

		// All the resources owned by the Resource Manager stored by its
		// ID, created from a hash of its reletive path.
		std::unordered_map<unsigned int, ResourcePtr> m_resourceMap;

		// Double buffered queues.
		std::deque<std::pair<ResourcePtr, Task>> m_processingQueue;
		std::deque<std::pair<ResourcePtr, Task>> m_deferredQueue;

	public:
		ResourceManager();
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
		~ResourceManager();

		/// <summary>
		/// Gets or Creates the Loader thread, the ability to get loader
		/// This can be useful for a future thread manager to "give" the
		/// resource manager a thread to use rather than an std::thread.
		/// </summary>
		void Initialize(const char* engineResourcePath, const char* clientResourcePath
			, bool usingRawFiles = false, bool isMultiThreaded = true);

		/// <summary>
		/// Get the directory path for the resources used by the engine specifically.
		/// </summary>
		/// <returns>Path.</returns>
		const std::string& GetEngineResourcePath() const { return m_engineResourcePath; }

		/// <summary>
		/// Get the directory path for the resources used by the client application.
		/// </summary>
		/// <returns>Path.</returns>
		const std::string& GetClientResourcePath() const { return m_clientResourcePath; }

		/// <summary>
		/// Set the path to search for specified resources.
		/// Only the client resource path can be changed at runtime.
		/// The engine should be set via config options.
		/// </summary>
		/// <param name="newClientResourcePath"></param>
		void SetClientResourcePath(const std::string& newClientResourcePath) { m_clientResourcePath = newClientResourcePath; }

		/// <summary>
		/// Is the Resource Manager retrieving assets from raw files or packed/zipped files?
		/// </summary>
		/// <returns></returns>
		bool IsUsingRawAssets() const { return m_useRawAssets; }

		/// <summary>
		/// Set the Resource Manager to use or not use raw files.
		/// </summary>
		/// <param name="useRawAssets"></param>
		void SetUsingRawAssets(bool useRawAssets) { m_useRawAssets = useRawAssets; }

		/// <summary>
		/// Get the threading state of the resource manager.
		/// </summary>
		/// <returns>True if threaded, false if not.</returns>
		bool IsMultiThreaded() const { return m_isMultiThreaded; }

		/// <summary>
		/// Either turn on or off threading for the resource manager. I may not want this to be able to change at runtime.
		/// This has no effect if you set the manager thread state to
		/// match its current state.
		/// </summary>
		/// <param name="isThreaded">True to turn on, false to turn off threading.</param>
		void SetMultiThreaded(bool isThreaded);

		/// <summary>
		/// Retrieves an already loaded resource, or adds a resource to
		/// the queue to be loaded if it isn't already loaded.
		/// </summary>
		ResourcePtr GetResource(unsigned int resourceID);

		/// <summary>
		/// Retrieves an already loaded resource, or adds a resource to
		/// the queue to be loaded if it isn't already loaded.
		/// </summary>
		ResourcePtr GetResource(const std::string& resourceName);

		/// <summary>
		/// Checks if a resource has been loaded and retrieves it, else
		/// it will immediately load it on the main thread. (Blocking)
		/// </summary>
		ResourcePtr GetResourceNow(unsigned int resourceID);

		/// <summary>
		/// Checks if a resource has been loaded and retrieves it, else
		/// it will immediately load it on the main thread. (Blocking)
		/// </summary>
		ResourcePtr GetResourceNow(const std::string& resourceName);

		/// <summary>
		/// If using *Raw* files from the directory, this will check if
		/// any loaded resources are out-of-date when compared to the 
		/// resources in the directory and add them to the load queue.
		/// </summary>
		void ReloadResources();

		/// <summary>
		/// Unloads the given resource. This should be called by the resource destructor.
		/// This will allow the resource map to be cleaned up when there are no longer any
		/// references to a resource. Should not generally be called by a client.
		/// </summary>
		/// <param name="resourceID"></param>
		void UnloadResource(unsigned int resourceID);

		/// <summary>
		/// Unloads the given resource. This should be called by the resource destructor.
		/// This will allow the resource map to be cleaned up when there are no longer any
		/// references to a resource. Should not generally be called by a client. (Blocking)
		/// </summary>
		/// <param name="resourceID"></param>
		void UnloadResourceNow(unsigned int resourceID);

	private:
		/// <summary>
		/// Search the resource map for a resource with the given ID
		/// </summary>
		/// <param name="resourceName"></param>
		/// <returns></returns>
		ResourcePtr FindAndReturn(unsigned int resourceID);

		/// <summary>
		/// Creates a new resource to be able to pass to the caller.
		/// </summary>
		/// <param name="resourceName"></param>
		/// <returns></returns>
		ResourcePtr CreateNewResource(unsigned int resourceID);

		/// <summary>
		/// This is used only in the case that multithreading is turned off.
		/// This will process all the resource calls from the previous frame.
		/// This is a blocking process and it will prevent the appliation from
		/// continueing until the queue has been emptied.
		/// </summary>
		void ProcessResourceQueue();

		/// <summary>
		/// This is the function passed into the loader thread. It will
		/// allow the loaderthread to process all of the read & writes.
		/// It should control when the thread will sleep and wake up to
		/// process the queue. It should be able to force the thread to
		/// sleep when a immediate file write or read occurs.
		/// </summary>
		void ProcessResourceQueueThreaded();
	};
}