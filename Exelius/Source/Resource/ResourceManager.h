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

	/// <summary>
	/// ResourceManager resourceMngr = ResourceManager::GetInstance();
	/// int id = resourceMngr.QueueLoad("Scripts/Test.txt");
	/// Resource* pLoadedResource = resourceMngr.GetResource(id);
	/// </summary>
	class ResourceManager
		: public Singleton<ResourceManager>
	{
		// I do not want the Initialize function to be accessible by the client, but the engine needs to be able to call it.
		friend class Application;

		// Default factory used by the resource manager to create resources. This should be engine defined.
		ResourceFactory* m_pResourceFactory;

		// Thread stuff. This could possibly be made into a separate class?
		std::thread m_loaderThread;
		std::atomic_bool m_quitThread;
		std::condition_variable m_signalThread;
		std::mutex m_deferredQueueLock;

		// Contains the loaded/ing resources.
		ResourceDatabase m_resourceDatabase;

		// Resources to be loaded.
		eastl::deque<ResourceID> m_deferredQueue;

		// Maybe consider alternatives to these member vars.
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

		//----------------------------------------------------------------------------------------------------------------------------------
		// Resource Management
		//----------------------------------------------------------------------------------------------------------------------------------

		const ResourceID& QueueLoad(const ResourceID& resourceID, bool signalLoaderThread);
		const ResourceID& LoadNow(const ResourceID& resourceID);

		void Release(const ResourceID& resourceID);

		void SignalLoaderThread();
		void SignalAndWaitForLoaderThread();

		void ReloadResource(const ResourceID& resourceID, bool forceLoad = false);

		Resource* GetResource(const ResourceID& resourceID, bool forceLoad = false);

		void LockResource(const ResourceID& resourceID);
		void UnlockResrce(const ResourceID& resourceID);

		//----------------------------------------------------------------------------------------------------------------------------------
		// Getters and Setters.
		//----------------------------------------------------------------------------------------------------------------------------------

		bool SetAssetPackageFile(const char* pAssetPackageFilePath) { m_clientAssetPackFile = pAssetPackageFilePath; }

		void SetUsingRawAssets(bool useRawAssets) { m_useRawAssets = useRawAssets; }

		const eastl::string& GetEngineResourcePath() const { return m_engineResourcePath; }

		const eastl::string& GetClientResourcePath() const { return m_clientResourcePath; }
		void SetClientResourcePath(const eastl::string& newClientResourcePath) { m_clientResourcePath = newClientResourcePath; }

	private:
		bool Initialize(ResourceFactory* pResourceFactory, const char* pEngineResourcePath = nullptr, const char* pClientResourcePath = nullptr, bool useRawAssets = false, const char* pInitialClientAssetPackage = nullptr);

		void ProcessResourceQueueThreaded();

		void LoadResource(const ResourceID& resourceID);

		eastl::vector<std::byte> LoadRawData(const ResourceID& resourceID);

		eastl::vector<std::byte> LoadFromDisk(const ResourceID& resourceID);
		eastl::vector<std::byte> LoadFromZip(const ResourceID& resourceID);
	};
}