#pragma once

#include "Source/Utility/Macros.h"
#include "Source/Resource/Resources/Resource.h"

///Must include all resource types here.			<-- I don't think I like this.>
//#incude "Source/Resource/Resources/Texture.h"

#include <unordered_map>
#include <queue>
#include <thread>



//-------------------------------------------------------------------------------------------------
// Things not yet covered
//-------------------------------------------------------------------------------------------------
/*
	How to actively switch between loading from a compressed package and
		raw files.

	Saving and serialization of files.

	Ability to turn *off* multithreaded resource management.
*/


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
	/// The Resource manager handles both importing or loading of assets
	/// as well as the exporting or saving of assets. Resources can only
	/// be loaded once, and the reference count for them are incremented
	/// when "retrieved", or decremented when "freed". The resource life-
	/// time is managed this way, and are unloaded when there is not any
	/// remaining references to the resource.
	/// 
	/// The Resource Manager is able to load resources raw from the file
	/// system, with the benefit of checking occasionally for changes in
	/// the files, enabling hot-reloading of assets. Also, resources can
	/// be batched together into a single compressed package for release
	/// builds. (Not sure yet if the Resource Manager itself will handle
	/// this batching)
	/// 
	/// Possible Example of Use:
	/// 
	/// pActor->AddComponent<Texture>(ResourceManager::GetInstance()->
	///		GetResource<Texture>("TestPic.png"));
	/// 
	/// </summary>
	class ResourceManager
	{
		// The different tasks that the thread can perform on a resource
		enum class Task
		{
			LOAD,
			UNLOAD
		};

		// The thread that does the actual work of the Resource Manager.
		std::thread* m_pLoaderThread;

		// All the resources owned by the Resource Manager stored by its
		// ID, created from a hash of its reletive path.
		std::unordered_map<int, ResourcePtr> m_resourceMap;

		// Temporary double buffered queues. For Psuedocode.
		std::queue<std::pair<ResourcePtr, Task>>* m_pProcessingQueue;
		std::queue<std::pair<ResourcePtr, Task>>* m_pDeferredQueue;

	public:
		ResourceManager();
		~ResourceManager();

		/// <summary>
		/// Gets or Creates the Loader thread, the ability to get loader
		/// This can be useful for a future thread manager to "give" the
		/// resource manager a thread to use rather than an std::thread.
		/// </summary>
		void Initialize();

		/// <summary>
		/// Retrieves an already loaded resource, or adds a resource to
		/// the queue to be loaded if it isn't already loaded.
		/// </summary>
		template<class ResourceType>
		ResourcePtr GetResource(std::string& resourceName)
		{
			//Look to see if this resource has already been loaded.
			ResourcePtr foundResource = FindAndReturn(resourceName);
			if (foundResource)
				return foundResource;

			// Create a pointer to the resource.
			ResourcePtr newResource = CreateNewResource<ResourceType>(resourceName);

			// Check to see if this resource is already in a queue to be loaded and return it if so. (Now we just need to wait for it)
				// lock the loader thread
				// Check both queues
					// Return if found
				// Unlock loader thread


			// Lock the resource queue.

			// Add the resource.
			m_pDeferredQueue->emplace(newResource, Task::LOAD);

			// Unlock the queue.

			// Wake up the loader thread if it is asleep.

			return newResource;
		}

		/// <summary>
		/// Checks if a resource has been loaded and retrieves it, else
		/// it will immediately load it on the main thread. (Blocking)
		/// </summary>
		template<class ResourceType>
		ResourcePtr GetResourceNow(std::string& resourceName)
		{
			//Look to see if this resource has already been loaded.
			ResourcePtr foundResource = FindAndReturn(resourceName);
			if (foundResource)
				return foundResource;

			// Check to see if this resource is already in a queue to be loaded and remove it from that queue.
				// lock the loader thread
				// Check both queues
					// Remove if found
				// Unlock loader thread (maybe?)


			// Create a pointer to the resource.
			ResourcePtr newResource = CreateNewResource<ResourceType>(resourceName);

			newResource->Load();

			return newResource;
		}

	private:
		/// <summary>
		/// Search the resource map for a resource with the given name(ID)
		/// </summary>
		/// <param name="resourceName"></param>
		/// <returns></returns>
		ResourcePtr FindAndReturn(std::string& resourceName);

		/// <summary>
		/// Creates a new resource to be able to pass to the caller.
		/// </summary>
		/// <param name="resourceName"></param>
		/// <returns></returns>
		template<class ResourceType>
		ResourcePtr CreateNewResource(std::string& resourceName)
		{
			// Create a pointer to the resource.
			ResourceType newResource(resourceName);

			// Add this new resource to the resource map using it's ID.
			m_resourceMap.emplace(newResource->GetID(), newResource);

			return newResource;
		}

		/// <summary>
		/// This is the function passed into the loader thread. It will
		/// allow the loaderthread to process all of the read & writes.
		/// It should control when the thread will sleep and wake up to
		/// process the queue. It should be able to force the thread to
		/// sleep when a immediate file write or read occurs.
		/// </summary>
		void ProcessResourceQueue();

		/// <summary>
		/// This call is made on the load thread and will check through
		/// all the loaded resources to find any that aren't referenced		<-- This might be an issue?>
		/// and add them to the queue to be unloaded.
		/// </summary>
		void ValidateResources();
	};
}