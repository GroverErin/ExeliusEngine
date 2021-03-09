#include "EXEPCH.h"
#include "Source/Resource/ResourceManager.h"
#include "Source/Resource/ResourceFactory.h"

#include <fstream>
#include <zlib.h>

namespace Exelius
{
#pragma pack(1)
	struct ZipLocalHeader
	{
		static constexpr unsigned int kSignature = 0x04034b50;

		uint32_t sig;
		uint16_t version;
		uint16_t flag;
		uint16_t compression;	// Z_NO_COMPRESSION or Z_DEFLATED
		uint16_t modTime;
		uint16_t modDate;
		uint32_t crc32;
		uint32_t cSize;
		uint32_t ucSize;
		uint16_t fnameLen;	// Filename string follows header.
		uint16_t xtraLen;	// Extra field follows filename.
	};

	struct ZipDirHeader
	{
		static constexpr unsigned int kSignature = 0x06054b50;

		uint32_t sig;
		uint16_t nDisk;
		uint16_t nStartDisk;
		uint16_t totalDirEntries;
		uint32_t dirSize;
		uint32_t dirOffset;
		uint16_t cmntLen;
	};

	struct ZipDirFileHeader
	{
		static constexpr unsigned int kSignature = 0x02014b50;

		uint32_t sig;
		uint16_t verMade;
		uint16_t verNeeded;
		uint16_t flag;
		uint16_t compression;	// COMP_xxxx
		uint16_t modTime;
		uint16_t modDate;
		uint32_t crc32;
		uint32_t cSize;			// Compressed size
		uint32_t ucSize;		// Uncompressed size
		uint16_t fnameLen;		// Filename string follows header.
		uint16_t xtraLen;		// Extra field follows filename.
		uint16_t cmntLen;		// Comment field follows extra field.
		uint16_t diskStart;
		uint16_t intAttr;
		uint32_t extAttr;
		uint32_t hdrOffset;

		char* GetName() const { return (char*)(this + 1); }
		char* GetExtra() const { return GetName() + fnameLen; }
		char* GetComment() const { return GetExtra() + xtraLen; }
	};
#pragma pack()

	static constexpr ptrdiff_t kPtrDelta = -(static_cast<ptrdiff_t>(sizeof(ZipDirHeader)));


	ResourceManager::ResourceManager()
		: m_pResourceFactory(nullptr)
		, m_quitThread(false)
		, m_engineResourcePath("Invalid Engine Resource Path.")
		, m_clientResourcePath("Invalid Client Resource Path.")
		, m_clientAssetPackFile("Invalid Client Asset Pack File.")
		, m_useRawAssets(false)
	{
	}

	ResourceManager::~ResourceManager()
	{
		// Remove resources to be loaded.
		m_deferredQueueLock.lock();
		m_deferredQueue.clear();
		m_deferredQueueLock.unlock();

		// Tell the thread we are done.
		m_quitThread = true;
		SignalAndWaitForLoaderThread();
		m_loaderThread.join();

		// unload all the currently loaded resources.
		m_resourceDatabase.UnloadAll();

		// Don't delete, this lives on the Application.
		m_pResourceFactory = nullptr;


	}

	const ResourceID& ResourceManager::QueueLoad(const ResourceID& resourceID, bool signalLoaderThread)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXELOG_ENGINE_TRACE("Queueing: {}", resourceID.Get().c_str());

		// Check if the resource is already in the resource database.
		if (m_resourceDatabase.IsFound(resourceID))
		{
			EXELOG_ENGINE_TRACE("Attempted to Queue a Resource that is either already loaded or queued.");
			return resourceID;
		}

		// Else add it to the load queue and return its id.
		m_resourceDatabase.CreateEntry(resourceID);
		
		m_deferredQueueLock.lock();
		m_deferredQueue.emplace_back(resourceID);
		m_deferredQueueLock.unlock();

		if (signalLoaderThread)
			SignalLoaderThread();

		return resourceID;
	}

	const ResourceID& ResourceManager::LoadNow(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXELOG_ENGINE_TRACE("Loading Resource On Main Thread: {}", resourceID.Get().c_str());

		// Check if the resource is already loaded.
		if (m_resourceDatabase.GetLoadStatus(resourceID) == ResourceLoadStatus::kLoaded)
		{
			EXELOG_ENGINE_TRACE("Attempted to Load a Resource that is already loaded.");
			return resourceID;
		}

		// Second check may present a race condition.
		else if (m_resourceDatabase.GetLoadStatus(resourceID) == ResourceLoadStatus::kLoading)
		{
			EXELOG_ENGINE_TRACE("Attempted to Load a Resource that is queued. TODO: Interrupt the thread and Load it immediately.");
			return resourceID;
		}

		// Else load and return it.
		m_resourceDatabase.CreateEntry(resourceID);

		LoadResource(resourceID);

		return resourceID;
	}

	void ResourceManager::Release(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXELOG_ENGINE_TRACE("Releasing: {}", resourceID.Get().c_str());

		ResourceEntry* pResourceEntry = m_resourceDatabase.GetEntry(resourceID);

		if (!pResourceEntry)
			return;
		
		// Decrement the reference count of this resource.
		// If there is no longer any references to this resource, then unload it.
		if (!pResourceEntry->DecrementRefCount())
			m_resourceDatabase.Unload(resourceID);
	}

	void ResourceManager::SignalLoaderThread()
	{
		m_signalThread.notify_one();
	}

	void ResourceManager::SignalAndWaitForLoaderThread()
	{
		SignalLoaderThread();

		// Not sure if this will work?
		std::mutex m_waitMutex;
		std::unique_lock<std::mutex> waitLock(m_waitMutex);

		m_signalThread.wait(waitLock);
	}

	void ResourceManager::ReloadResource(const ResourceID& resourceID, bool forceLoad)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXELOG_ENGINE_TRACE("Reloading: {}", resourceID.Get().c_str());

		// Check if the resource is not already loaded.
		if (m_resourceDatabase.GetLoadStatus(resourceID) != ResourceLoadStatus::kLoaded)
		{
			EXELOG_ENGINE_TRACE("Attempted to Reload a Resource that is not loaded.");
			return;
		}

		m_resourceDatabase.Unload(resourceID);

		if (forceLoad)
			LoadNow(resourceID);
		else
			QueueLoad(resourceID, true);
	}

	Resource* ResourceManager::GetResource(const ResourceID& resourceID, bool forceLoad)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXELOG_ENGINE_TRACE("Retrieving Resource: {}", resourceID.Get().c_str());

		ResourceEntry* pResourceEntry = m_resourceDatabase.GetEntry(resourceID);

		if (!pResourceEntry && forceLoad)
		{
			ResourceID id = LoadNow(resourceID);
			return GetResource(id, false); // Should be guaranteed, but calling false will prevent infinite recursion.
		}
		else if (!pResourceEntry)
		{
			return nullptr;
		}

		return pResourceEntry->GetResource();
	}

	void ResourceManager::LockResource(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXELOG_ENGINE_TRACE("Locking Resource: {}", resourceID.Get().c_str());

		ResourceEntry* pResourceEntry = m_resourceDatabase.GetEntry(resourceID);

		if (!pResourceEntry)
			return;

		pResourceEntry->IncrementLockCount();
	}

	void ResourceManager::UnlockResrce(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXELOG_ENGINE_TRACE("Unlocking Resource: {}", resourceID.Get().c_str());

		ResourceEntry* pResourceEntry = m_resourceDatabase.GetEntry(resourceID);

		if (!pResourceEntry)
			return;

		pResourceEntry->DecrementLockCount();
	}

	bool ResourceManager::Initialize(ResourceFactory* pResourceFactory, const char* pEngineResourcePath, const char* pClientResourcePath, bool useRawAssets, const char* pInitialClientAssetPackage)
	{
		m_deferredQueue.clear();

		m_pResourceFactory = pResourceFactory;
		m_engineResourcePath = pEngineResourcePath;
		m_clientResourcePath = pClientResourcePath;
		m_useRawAssets = useRawAssets;
		m_clientAssetPackFile = pInitialClientAssetPackage;

		m_loaderThread = std::thread(&ResourceManager::ProcessResourceQueueThreaded, this);

		return true;
	}

	void ResourceManager::ProcessResourceQueueThreaded()
	{
		std::mutex waitMutex;
		eastl::deque<ResourceID> processingQueue;
		std::unique_lock<std::mutex> waitLock(waitMutex);

		// While the thread is still active (Not shut down)
		while (!m_quitThread)
		{
			// Wait until we are signaled to work.
			m_signalThread.wait(waitLock);

			// Don't do any work if we're exiting.
			if (m_quitThread)
				continue;

			/// Capture the deferred queue into the processing queue (swap buffers)
			m_deferredQueueLock.lock();
			processingQueue.swap(m_deferredQueue);
			m_deferredQueueLock.unlock();

			// Process the queue
			while (!processingQueue.empty())
			{
				LoadResource(processingQueue.front());
				processingQueue.pop_front();
			}

			// Done loading this pass, so signal the main thread in case it is waiting.
			m_signalThread.notify_one();
		}

		// Let the main thread know we are fully exiting in case they are waiting.
		m_signalThread.notify_one();
	}

	void ResourceManager::LoadResource(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXELOG_ENGINE_TRACE("Loading Resource Internally: {}", resourceID.Get().c_str());

		eastl::vector<std::byte> rawData = LoadRawData(resourceID);

		if (rawData.empty())
		{
			EXELOG_ENGINE_WARN("Raw file data was empty.");
			return;
		}

		Resource* pResource = m_pResourceFactory->CreateResource(resourceID);
		if (!pResource)
		{
			EXELOG_ENGINE_WARN("Failed to create resource from resource factory.");
		}

		if (pResource->Load(std::move(rawData)) != Resource::LoadResult::kFailed)
		{
			m_resourceDatabase.GetEntry(resourceID)->SetResource(pResource);
		}
	}

	eastl::vector<std::byte> ResourceManager::LoadRawData(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		EXELOG_ENGINE_TRACE("Loading Resource Raw Data: {}", resourceID.Get().c_str());

		if (m_useRawAssets)
		{
			return LoadFromDisk(resourceID);
		}
		else
		{
			return LoadFromZip(resourceID);
		}
	}

	eastl::vector<std::byte> ResourceManager::LoadFromDisk(const ResourceID& resourceID)
	{
		eastl::string strPath = resourceID.Get();
		eastl::transform(strPath.begin(), strPath.end(), strPath.begin(), [this](unsigned char c) -> unsigned char
			{
				return (unsigned char)std::tolower(c);
			});
		eastl::replace(strPath.begin(), strPath.end(), '\\', '/');

		std::ifstream inFile;
		inFile.open(strPath.c_str(), std::ios_base::binary | std::ios_base::in);
		if (!inFile.is_open())
		{
			EXELOG_ENGINE_WARN("Failed to open file: {}", strPath.c_str());

			if (inFile.bad())
				EXELOG_ENGINE_WARN("I/O error while reading");
			else if (inFile.eof())
				EXELOG_ENGINE_WARN("End of file reached successfully");
			else if (inFile.fail())
				EXELOG_ENGINE_WARN("Non-integer data encountered");
			
			return eastl::vector<std::byte>();
		}

		// find the size of the file in bytes;
		inFile.seekg(0, std::ios::end);
		std::streampos size = inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		// Read contents into vector of bytes
		eastl::vector<std::byte> data(static_cast<size_t>(size));
		inFile.read(reinterpret_cast<char*>(data.data()), size);

		inFile.close();

		return data;
	}

	eastl::vector<std::byte> ResourceManager::LoadFromZip(const ResourceID& resourceID)
	{
		eastl::string strPath = resourceID.Get().c_str();
		eastl::transform(strPath.begin(), strPath.end(), strPath.begin(), [this](unsigned char c) -> unsigned char
			{
				return (unsigned char)std::tolower(c);
			});
		eastl::replace(strPath.begin(), strPath.end(), '\\', '/');

		//std::ifstream inFile(strPath.c_str(), std::ios_base::binary | std::ios_base::in);
		//if (inFile.fail())
		//{
		//	EXELOG_ENGINE_WARN("Failed to open file: {}", strPath.c_str());
		//	return eastl::vector<std::byte>();
		//}

		//ZipDirHeader dirHeader;
		//memset(&dirHeader, 0, sizeof(dirHeader));

		//inFile.seekg(kPtrDelta, std::ios_base::end);
		//const auto dirHeaderOffset = inFile.tellg();
		//inFile.read(reinterpret_cast<char*>(&dirHeader), sizeof(ZipDirHeader));
		//if (dirHeader.sig != ZipDirHeader::kSignature)
		//{
		//	EXELOG_ENGINE_WARN("Corrupted Zip file: {}", strPath.c_str());
		//	return eastl::vector<std::byte>();
		//}

		//// Now load the respective file? I need to think more about how I want this to operate.
		return eastl::vector<std::byte>();
	}
}