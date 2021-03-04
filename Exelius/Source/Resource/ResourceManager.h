#pragma once

#include "Source/Resource/Resource.h"

#include <EASTL/unordered_map.h>
#include <EASTL/deque.h>
#include <EASTL/vector.h>

#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>

namespace Exelius
{
	class ResourceEntry
	{
		Resource* m_pResource;
		int m_refCount;
		ResourceLoadStatus m_status;

		// Alias for resource id... currently this is BAD, literally just an eastl::string.
		ResourceID m_id;
	public:

		ResourceEntry(const ResourceID& resourceID)
			: m_pResource(nullptr)
			, m_refCount(1)
			, m_id(resourceID)
		{
			//
		}

		ResourceEntry(const ResourceEntry&) = delete;
		ResourceEntry(ResourceEntry&&) = delete;
		ResourceEntry& operator=(const ResourceEntry&) = delete;
		ResourceEntry& operator=(ResourceEntry&&) = delete;

		~ResourceEntry()
		{
			delete m_pResource;
		}

		Resource* GetResource() const { return m_pResource; }
		void IncrementRefCount() { ++m_refCount; }

		bool DecrementRefCount()
		{
			--m_refCount;
			if (m_refCount <= 0)
			{
				return false;
			}

			return true;
		}

		/// <summary>
		/// Get the unique ID (string hash) of this resource.
		/// </summary>
		/// <returns>GUID</returns>
		const ResourceID& GetID() const { return m_id; }

		/// <summary>
		/// Get the current loading status of this reource.
		/// </summary>
		/// <returns>Loading status.</returns>
		ResourceLoadStatus GetStatus() const { return m_status; }

		/// <summary>
		/// Set the status of this resource. This should only be able to be set by the resource loading management... I should probably not allow the client to adjust this.
		/// </summary>
		/// <param name="status"></param>
		void SetStatus(ResourceLoadStatus status) { m_status = status; }

		void StoreResourceIntoEntry(Resource* const pResource)
		{
			m_pResource = pResource;
		}
	};

	class ResourceFactory;

	/// <summary>
	/// 
	/// </summary>
	class ResourceManager
	{
		friend class Application;

		// The thread that does the actual work of the Resource Manager.
		std::thread m_loaderThread;

		std::atomic_bool m_quitThread;
		std::condition_variable m_signalThread;
		std::mutex m_waitMutex;
		std::mutex m_processingQueueLock;
		std::mutex m_deferredQueueLock;

		ResourceFactory* m_pResourceFactory;
		eastl::string m_engineResourcePath;
		eastl::string m_clientResourcePath;
		eastl::string m_clientAssetPackFile;
		bool m_useRawAssets;

		// All the resources owned by the Resource Manager stored by its
		// ID, created from a hash of its reletive path.
		//eastl::unordered_map<ResourceID, ResourceEntry> m_resourceMap;

		// Double buffered queues.
		eastl::deque<ResourceEntry*> m_processingQueue;
		eastl::deque<ResourceEntry*> m_deferredQueue;

	public:
		ResourceManager();
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
		~ResourceManager();

		void QueueLoad(const ResourceID& resourceID, bool signalLoaderThread = true);
		void LoadNow(const ResourceID& resourceID);

		void Unload(const ResourceID& resourceID);

		void SignalLoaderThread();
		void SignalAndWaitForLoaderThread();

		void ReloadResource(const ResourceID& resourceID, bool forceLoad = false);
		void ReloadAllResources(bool forceLoad = false);

		Resource* GetResource(const ResourceID& resourceID, bool forceLoad = false);

		void LockResource(const ResourceID& resourceID);
		void UnlockResrce(const ResourceID& resourceID);

		//----------------------------------------------------------------------------------------------------------------------------------
		// Getters and Setters.
		//----------------------------------------------------------------------------------------------------------------------------------

		/// <summary>
		/// Sets the currently used resource asset package by the client application.
		/// </summary>
		/// <param name="pAssetPackageFilePath"></param>
		/// <returns></returns>
		bool SetAssetPackageFile(const char* pAssetPackageFilePath) { m_clientAssetPackFile = pAssetPackageFilePath; }

		void SetUsingRawAssets(bool useRawAssets) { m_useRawAssets = useRawAssets; }

		const eastl::string& GetEngineResourcePath() const { return m_engineResourcePath; }

		const eastl::string& GetClientResourcePath() const { return m_clientResourcePath; }
		void SetClientResourcePath(const eastl::string& newClientResourcePath) { m_clientResourcePath = newClientResourcePath; }

		ResourceLoadStatus GetResourceStatus(const ResourceID& resourceID);

	private:
		/// <summary>
		/// Initialize the ResourceManager.
		/// </summary>
		/// <param name="pEngineResourcePath">The path to engine specific resources (Engine splash logo).</param>
		/// <param name="pClientResourcePath">The path to application specific resources.</param>
		/// <param name="useRawAssets">Set whether or not to use raw asset files or compressed files.</param>
		/// <param name="pInitialClientAssetPackage">File name and path to the clients initial asset package, relative to the client resource path.</param>
		/// <returns>True on success, false on failure.</returns>
		bool Initialize(ResourceFactory* pResourceFactory, const char* pEngineResourcePath = nullptr, const char* pClientResourcePath = nullptr, bool useRawAssets = false, const char* pInitialClientAssetPackage = nullptr);

		bool IsValidResource(const ResourceID& resourceID);

		void ProcessResourceQueueThreaded();

		void LoadResource(ResourceEntry* resourceEntry);

		eastl::vector<std::byte> LoadRawData(ResourceEntry* resourceEntry);

		eastl::vector<std::byte> LoadFromDisk(ResourceEntry* resourceEntry);
		eastl::vector<std::byte> LoadFromZip(ResourceEntry* resourceEntry);
	};
}