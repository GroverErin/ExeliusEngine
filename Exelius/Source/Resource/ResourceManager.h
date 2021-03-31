#pragma once

#include "Source/Utility/Singleton.h"
#include "Source/Resource/ResourceDatabase.h"

#include <EASTL/deque.h>
#include <EASTL/vector.h>

#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>

namespace Exelius
{
	class ResourceFactory;

	class ResourceManager
		: public Singleton<ResourceManager>
	{
		ResourceFactory* m_pResourceFactory;

		std::thread m_loaderThread;
		std::atomic_bool m_quitThread;
		std::condition_variable m_signalThread;
		std::mutex m_deferredQueueLock;

		ResourceDatabase m_resourceDatabase;

		eastl::deque<ResourceID> m_deferredQueue;

		eastl::string m_engineResourcePath;
		eastl::string m_clientResourcePath;
		eastl::string m_clientAssetPackFile;
		bool m_useRawAssets;

	public:
		ResourceManager();
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
		~ResourceManager();


		bool Initialize(ResourceFactory* pResourceFactory, const char* pEngineResourcePath = nullptr, bool useRawAssets = false);
		const ResourceID& QueueLoad(const ResourceID& resourceID, bool signalLoaderThread);
		const ResourceID& LoadNow(const ResourceID& resourceID);

		void ReleaseResource(const ResourceID& resourceID);

		void SignalLoaderThread();
		void SignalAndWaitForLoaderThread();

		void ReloadResource(const ResourceID& resourceID, bool forceLoad = false);

		Resource* GetResource(const ResourceID& resourceID, bool forceLoad = false);

		void LockResource(const ResourceID& resourceID);
#undef UnlockResource
		void UnlockResource(const ResourceID& resourceID);

		bool SetAssetPackageFile(const char* pAssetPackageFilePath) { m_clientAssetPackFile = pAssetPackageFilePath; }

		void SetUsingRawAssets(bool useRawAssets) { m_useRawAssets = useRawAssets; }

		const eastl::string& GetEngineResourcePath() const { return m_engineResourcePath; }

		const eastl::string& GetClientResourcePath() const { return m_clientResourcePath; }
		void SetClientResourcePath(const eastl::string& newClientResourcePath) { m_clientResourcePath = newClientResourcePath; }

	private:
		void ProcessResourceQueueThreaded();

		void LoadResource(const ResourceID& resourceID);

		eastl::vector<std::byte> LoadRawData(const ResourceID& resourceID);

		eastl::vector<std::byte> LoadFromDisk(const ResourceID& resourceID);
		eastl::vector<std::byte> LoadFromZip(const ResourceID& resourceID);
	};
}