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
		: m_useRawAssets(false)
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
		//for (auto& resource : m_resourceMap)
		//{
		//	Unload(resource.first);
		//}
		//m_resourceMap.clear();

		//m_loaderThread.join();
	}

	bool ResourceManager::Initialize([[maybe_unused]] ResourceFactory* pResourceFactory, [[maybe_unused]] const char* pEngineResourcePath, [[maybe_unused]] const char* pClientResourcePath, [[maybe_unused]] bool useRawAssets, [[maybe_unused]] const char* pInitialClientAssetPackage)
	{
		return true;
	}

	void ResourceManager::QueueLoad(const ResourceID& resourceID, bool signalLoaderThread)
	{
		// Check if resource exists...
		if (IsValidResource(resourceID))
		{
			// Resource exists, so increment the ref count.
			//m_resourceMap.at(resourceID).IncrementRefCount();
			return;
		}

		// Resource does not yet exist, create an empty entry for it as a placeholder for the data.
		ResourceEntry* pEntry = new ResourceEntry(resourceID);
		//m_resourceMap.emplace(resourceID, pEntry);

		// Add the resource.
		m_deferredQueueLock.lock();
		m_deferredQueue.emplace_back(pEntry);
		m_deferredQueueLock.unlock();

		// Wake up the loader thread if it is asleep.
		if (signalLoaderThread)
			SignalLoaderThread();
	}

	void ResourceManager::LoadNow(const ResourceID& resourceID)
	{
		// Check if resource exists...
		if (IsValidResource(resourceID))
		{
			// Resource exists, so increment the ref count.
			//m_resourceMap.at(resourceID).IncrementRefCount();
		}

		// Resource does not yet exist, create an empty entry for it as a placeholder for the data.
		ResourceEntry* pNewResourceEntry = new ResourceEntry(resourceID);
		//m_resourceMap.emplace(resourceID, pNewResourceEntry);


		LoadResource(pNewResourceEntry);
	}

	void ResourceManager::Unload(const ResourceID& resourceID)
	{
		// Check if resource exists...
		if (!IsValidResource(resourceID))
			return;

		// Resource exists, so decrement the ref count.
		//if (!m_resourceMap.at(resourceID).DecrementRefCount())
		//{
			//m_resourceMap.erase(resourceID);
		//}
	}

	void ResourceManager::SignalLoaderThread()
	{
		m_signalThread.notify_one();
	}

	void ResourceManager::SignalAndWaitForLoaderThread()
	{
		m_signalThread.notify_one();

		//Wait until thread is finished.

	}

	void ResourceManager::ReloadResource([[maybe_unused]] const ResourceID& resourceID, [[maybe_unused]] bool forceLoad)
	{
	}

	void ResourceManager::ReloadAllResources([[maybe_unused]] bool forceLoad)
	{
	}

	Resource* ResourceManager::GetResource(const ResourceID& resourceID, bool forceLoad)
	{
		// Check if resource exists...
		if (!IsValidResource(resourceID))
		{
			if (!forceLoad)
				return nullptr;

			LoadNow(resourceID);
		}

		return nullptr;//m_resourceMap.at(resourceID).GetResource();
	}

	void ResourceManager::LockResource(const ResourceID& resourceID)
	{
		// Check if resource exists...
		if (!IsValidResource(resourceID))
			return;

		//m_resourceMap.at(resourceID).IncrementRefCount();
	}

	void ResourceManager::UnlockResrce(const ResourceID& resourceID)
	{
		// Check if resource exists...
		if (!IsValidResource(resourceID))
			return;

		/*if (!m_resourceMap.at(resourceID).DecrementRefCount())
		{
			m_resourceMap.erase(resourceID);
		}*/
	}
	
	ResourceLoadStatus ResourceManager::GetResourceStatus(const ResourceID& resourceID)
	{
		// Check if resource exists...
		if (!IsValidResource(resourceID))
			return ResourceLoadStatus::kInvalid;

		return ResourceLoadStatus::kInvalid;//m_resourceMap.at(resourceID).GetStatus();
	}

	bool ResourceManager::IsValidResource([[maybe_unused]] const ResourceID& resourceID)
	{
		/*auto found = m_resourceMap.find(resourceID);
		if (found != m_resourceMap.end() && found->second.GetStatus() != ResourceLoadStatus::kInvalid)
			return false;*/
		return true;
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
				LoadResource(m_processingQueue.front());
				m_processingQueue.pop_front();
			}
			m_processingQueueLock.unlock();
		}
	}

	void ResourceManager::LoadResource(ResourceEntry* resourceEntry)
	{
		EXELOG_ENGINE_INFO("Loading Resource: %s", resourceEntry->GetID().c_str());
		const ResourceType::Type resourceType = m_pResourceFactory->GetResourceType(resourceEntry->GetID().c_str());

		eastl::vector<std::byte> rawData = LoadRawData(resourceEntry);

		Resource* pResource = m_pResourceFactory->CreateResource(resourceEntry->GetID(), resourceType);
		if (!pResource)
		{
			EXELOG_ENGINE_WARN("Failed to create resource from resource factory.");
		}

		if (pResource->Load(rawData.data(), rawData.size()))
		{
			//Keeping the raw data.
			pResource->SetRawData(rawData.data());
		}
		else
		{
			rawData.clear();
		}

		resourceEntry->StoreResourceIntoEntry(pResource);
	}

	eastl::vector<std::byte> ResourceManager::LoadRawData(ResourceEntry* resourceEntry)
	{
		if (m_useRawAssets)
		{
			return LoadFromDisk(resourceEntry);
		}
		else
		{
			return LoadFromZip(resourceEntry);
		}
	}

	eastl::vector<std::byte> ResourceManager::LoadFromDisk(ResourceEntry* resourceEntry)
	{
		std::string strPath = resourceEntry->GetID().c_str();
		std::transform(strPath.begin(), strPath.end(), strPath.begin(), [this](unsigned char c) -> unsigned char
			{
				return (unsigned char)std::tolower(c);
			});
		replace(strPath.begin(), strPath.end(), '\\', '/');

		std::ifstream inFile(strPath.c_str(), std::ios_base::binary | std::ios_base::in);
		if (inFile.fail())
		{
			EXELOG_ENGINE_WARN("Failed to open file: %s", strPath.c_str());
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

	eastl::vector<std::byte> ResourceManager::LoadFromZip(ResourceEntry* resourceEntry)
	{
		std::string strPath = resourceEntry->GetID().c_str();
		std::transform(strPath.begin(), strPath.end(), strPath.begin(), [this](unsigned char c) -> unsigned char
			{
				return (unsigned char)std::tolower(c);
			});
		replace(strPath.begin(), strPath.end(), '\\', '/');

		std::ifstream inFile(strPath.c_str(), std::ios_base::binary | std::ios_base::in);
		if (inFile.fail())
		{
			EXELOG_ENGINE_WARN("Failed to open file: %s", strPath.c_str());
			return eastl::vector<std::byte>();
		}

		ZipDirHeader dirHeader;
		memset(&dirHeader, 0, sizeof(dirHeader));

		inFile.seekg(kPtrDelta, std::ios_base::end);
		const auto dirHeaderOffset = inFile.tellg();
		inFile.read(reinterpret_cast<char*>(&dirHeader), sizeof(ZipDirHeader));
		if (dirHeader.sig != ZipDirHeader::kSignature)
		{
			EXELOG_ENGINE_WARN("Corrupted Zip file: %s", strPath.c_str());
			return eastl::vector<std::byte>();
		}

		// Now load the respective file? I need to think more about how I want this to operate.
		return eastl::vector<std::byte>();

	}
}