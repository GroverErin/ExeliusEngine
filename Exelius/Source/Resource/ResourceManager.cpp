#include "EXEPCH.h"
#include "Source/Resource/ResourceManager.h"

namespace Exelius
{
	ResourceManager::ResourceManager()
		: m_pLoaderThread(nullptr)
		, m_pProcessingQueue(nullptr)
		, m_pDeferredQueue(nullptr)
	{
	}

	ResourceManager::~ResourceManager()
	{
		// Clear the load queues.

		// Lock the loader thread.

		while (!m_pProcessingQueue->empty())
			m_pProcessingQueue->pop();
		while (!m_pDeferredQueue->empty())
			m_pDeferredQueue->pop();

		// Unlock the loader thread.

		// Forcefully unload all the resources.
		for (auto& resource : m_resourceMap)
		{
			resource.second->Unload();
		}
		m_resourceMap.clear();

		//Detroy the loader thread if multi-threaded.
		delete m_pLoaderThread;
	}

	void ResourceManager::Initialize()
	{
		// Get or create a loader thread if multi-threaded.
		m_pLoaderThread = new std::thread(&ResourceManager::ProcessResourceQueue, this);
	}

	ResourcePtr ResourceManager::FindAndReturn(std::string& resourceName)
	{
		unsigned int id = HashString(resourceName);
		for (auto& resource : m_resourceMap)
		{
			if (resource.second->GetID() == id)
				return resource.second;
		}
		return nullptr;
	}

	void ResourceManager::ProcessResourceQueue()
	{
		// While the thread is still active (Not shut down)
		while (true) //To be fixed
		{
			ValidateResources();

			while (m_pDeferredQueue->empty())
			{
				// wait
			}

			/// Capture the deferred queue into the processing queue (swap buffers)			<-- This could be handled better i think?>
			auto queueCapture = m_pProcessingQueue;
			m_pProcessingQueue = m_pDeferredQueue;
			m_pDeferredQueue = queueCapture;

			// Process the queue
			while (!m_pProcessingQueue->empty())
			{
				if (m_pProcessingQueue->front().second == Task::LOAD)
					m_pProcessingQueue->front().first->Load();
				else if (m_pProcessingQueue->front().second == Task::UNLOAD)
					m_pProcessingQueue->front().first->Unload();

				m_pProcessingQueue->pop();
			}
		}
	}

	void ResourceManager::ValidateResources()
	{
		// Scan the filesystem for the files we currently have *loaded*
			// If any of them have been updated, add them to the load queue.
	}
}