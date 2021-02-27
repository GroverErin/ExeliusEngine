#include "EXEPCH.h"
#include "Source/Resource/ResourceManager.h"

namespace Exelius
{
	ResourceManager::ResourceManager()
		: m_useRawAssets(false)
		, m_isMultiThreaded(true)
	{
	}

	ResourceManager::~ResourceManager()
	{
		m_quitThread = true;
		m_signalThread.notify_all();

		// Clear the load queues.
		m_processingQueueLock.lock();
		while (!m_processingQueue.empty())
			m_processingQueue.pop_back();
		m_processingQueueLock.unlock();

		m_deferredQueueLock.lock();
		while (!m_deferredQueue.empty())
			m_deferredQueue.pop_back();
		m_deferredQueueLock.unlock();

		// Forcefully unload all the resources.
		for (auto& resource : m_resourceMap)
		{
			resource.second->Unload();
		}
		m_resourceMap.clear();

		//Detroy the loader thread if multi-threaded.
		if (m_isMultiThreaded)
			m_loaderThread.join();
	}

	void ResourceManager::Initialize(const char* engineResourcePath, const char* clientResourcePath, bool usingRawFiles, bool isMultiThreaded)
	{
		// Set the paths for resources. This should be configurable.
		m_engineResourcePath = engineResourcePath;
		m_clientResourcePath = clientResourcePath;

		// Set using Raw assets vs. packaged assets.
		m_useRawAssets = usingRawFiles;

		// Get or create a loader thread if multi-threaded.
		if (isMultiThreaded)
			m_loaderThread = std::thread(&ResourceManager::ProcessResourceQueueThreaded, this);

		m_isMultiThreaded = isMultiThreaded;
	}

	void ResourceManager::SetMultiThreaded(bool isThreaded)
	{
		// If turning threading off...
		if (m_isMultiThreaded && !isThreaded)
		{
			m_quitThread = true;
			m_signalThread.notify_all();
			m_loaderThread.join();
		}

		// If turning threading on...
		if (!m_isMultiThreaded && isThreaded)
		{
			m_quitThread = false;
			m_loaderThread = std::thread(&ResourceManager::ProcessResourceQueueThreaded, this);
		}

		m_isMultiThreaded = isThreaded;
	}

	ResourcePtr ResourceManager::GetResource(unsigned int resourceID)
	{
		//Look to see if this resource has already been loaded or queued.
		ResourcePtr foundResource = FindAndReturn(resourceID);
		if (foundResource)
			return foundResource;

		// Create a pointer to the resource.
		ResourcePtr newResource = CreateNewResource(resourceID);

		// Add the resource.
		m_deferredQueueLock.lock();
		m_deferredQueue.emplace_back(newResource, Task::LOAD);
		m_deferredQueueLock.unlock();

		// Wake up the loader thread if it is asleep.
		m_signalThread.notify_all();

		return newResource;
	}

	ResourcePtr ResourceManager::GetResource(const std::string& resourceName)
	{
		// Immediately hash the string name to get the ID.
		unsigned int resourceID = HashString(resourceName);

		auto& resource = GetResource(resourceID);
		resource->SetFilePath(resourceName);

		return resource;
	}

	ResourcePtr ResourceManager::GetResourceNow(unsigned int resourceID)
	{
		//Look to see if this resource has already been loaded or queued.
		ResourcePtr foundResource = FindAndReturn(resourceID);

		// If the resource was found, check to see if it is not yet loaded (In queue).
		if (foundResource && foundResource->GetStatus() == Resource::Status::kLoaded)
		{
			return foundResource;
		}
		else if (foundResource)
		{
			// Lock the queues, because we may need to remove something from them.
			m_processingQueueLock.lock();
			m_deferredQueueLock.lock();

			// Remove it from any queues.
			auto& found = std::find(m_deferredQueue.begin(), m_deferredQueue.end(), foundResource);
			if (found != m_deferredQueue.end())
			{
				m_deferredQueue.erase(found);
			}

			auto& found = std::find(m_processingQueue.begin(), m_processingQueue.end(), foundResource);
			if (found != m_processingQueue.end())
			{
				m_processingQueue.erase(found);
			}

			m_processingQueueLock.unlock();
			m_deferredQueueLock.unlock();

			foundResource->Load();
			return foundResource;
		}

		// Create a pointer to the resource.
		ResourcePtr newResource = CreateNewResource(resourceID);

		newResource->Load();

		return newResource;
	}

	ResourcePtr ResourceManager::GetResourceNow(const std::string& resourceName)
	{
		// Immediately hash the string name to get the ID.
		unsigned int resourceID = HashString(resourceName);

		return GetResourceNow(resourceID);
	}

	void ResourceManager::ReloadResources()
	{
	}

	void ResourceManager::UnloadResource(unsigned int resourceID)
	{
	}

	void ResourceManager::UnloadResourceNow(unsigned int resourceID)
	{
	}

	ResourcePtr ResourceManager::FindAndReturn(unsigned int resourceID)
	{
		auto& resource = m_resourceMap.find(resourceID);
		if (resource != m_resourceMap.end())
			return resource->second;
		return nullptr;
	}

	ResourcePtr ResourceManager::CreateNewResource(unsigned int resourceID)
	{
		ResourcePtr newResource = std::make_shared<Resource>(resourceID);

		// Add this new resource to the resource map using it's ID.
		m_resourceMap.emplace(resourceID, newResource);

		return newResource;
	}

	void ResourceManager::ProcessResourceQueue()
	{
		if (m_isMultiThreaded)
		{
			EXELOG_ENGINE_WARN("The Resource Manager is Threaded! Inappropriate call to the non-threaded processing function.");
			return;
		}

		/// Capture the deferred queue into the processing queue (swap buffers)
		m_processingQueue.swap(m_deferredQueue);

		// Process the queue
		while (!m_processingQueue.empty())
		{
			if (m_processingQueue.front().second == Task::LOAD)
			{
				m_processingQueue.front().first->Load();
			}
			else if (m_processingQueue.front().second == Task::UNLOAD)
			{
				m_processingQueue.front().first->Unload();
			}

			m_processingQueue.pop_front();
		}
	}

	void ResourceManager::ProcessResourceQueueThreaded()
	{
		// While the thread is still active (Not shut down)
		while (!m_quitThread)
		{
			// Wait if we have no work to do.
			std::unique_lock<std::mutex> waitLock(m_waitMutex);
			m_signalThread.wait(waitLock, [this]()
				{
					bool empty = true;
					m_deferredQueueLock.lock();
					empty = m_deferredQueue.empty();
					m_deferredQueueLock.unlock();

					return m_quitThread || !empty;
				});

			// Don't do any work if we're exiting.
			if (m_quitThread)
				continue;

			/// Capture the deferred queue into the processing queue (swap buffers)
			m_deferredQueueLock.lock();
			m_processingQueueLock.lock();
			m_processingQueue.swap(m_deferredQueue);
			m_deferredQueueLock.unlock();
			// Process the queue
			while (!m_processingQueue.empty())
			{
				if (m_processingQueue.front().second == Task::LOAD)
					m_processingQueue.front().first->Load();
				else if (m_processingQueue.front().second == Task::UNLOAD)
					m_processingQueue.front().first->Unload();

				m_processingQueue.pop_front();
			}
			m_processingQueueLock.unlock();
		}
	}
}