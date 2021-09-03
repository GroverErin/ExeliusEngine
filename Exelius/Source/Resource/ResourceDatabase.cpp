#include "EXEPCH.h"
#include "Source/Resource/ResourceDatabase.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	ResourceDatabase::~ResourceDatabase()
	{
		UnloadAll();
	}

	ResourceEntry* ResourceDatabase::GetEntry(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		if (IsFound(resourceID))
		{
			//m_resourceMap.at(resourceID).IncrementRefCount();
			return &m_resourceMap.at(resourceID);
		}

		return nullptr;
	}

	ResourceLoadStatus ResourceDatabase::GetLoadStatus(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		if (IsFound(resourceID))
			return m_resourceMap.at(resourceID).GetStatus();

		return ResourceLoadStatus::kInvalid;
	}

	void ResourceDatabase::SetLoadStatus(const ResourceID& resourceID, ResourceLoadStatus newStatus)
	{
		EXE_ASSERT(resourceID.IsValid());

		if (IsFound(resourceID))
			m_resourceMap.at(resourceID).SetStatus(newStatus);
	}

	void ResourceDatabase::CreateEntry(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		Log log("ResourceManager");

		if (IsFound(resourceID))
		{
			log.Info("Resource Entry for {} already exists.", resourceID.Get().c_str());
			return;
		}

		m_resourceMap.try_emplace(resourceID);
	}

	bool ResourceDatabase::IsFound(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		auto found = m_resourceMap.find(resourceID);
		if (found == m_resourceMap.end())
			return false;

		return true;
	}

	void ResourceDatabase::Unload(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());
		Log log("ResourceManager");
		log.Trace("Unloading Resource: {}", resourceID.Get().c_str());

		if (!IsFound(resourceID))
			return;

		log.Trace("Resource State: {}", m_resourceMap.at(resourceID).GetStatus());

		// TODO: .at fails here if the resource is not in the map yet...
		m_resourceMap.at(resourceID).SetStatus(ResourceLoadStatus::kUnloading);

		Resource* pResource = m_resourceMap.at(resourceID).GetResource();
		if (pResource)
		{
			pResource->Unload();
			m_resourceMap.at(resourceID).SetStatus(ResourceLoadStatus::kUnloaded);
		}

		m_resourceMap.erase(resourceID);
		log.Trace("Unloaded Resource.");
}

	void ResourceDatabase::UnloadAll()
	{
		Log log("ResourceManager");
		log.Trace("Unloading All Resources.");

		for (auto& resourcePair : m_resourceMap)
		{
			resourcePair.second.SetStatus(ResourceLoadStatus::kUnloading);
			Resource* pResource = resourcePair.second.GetResource();
			if (pResource)
			{
				pResource->Unload();
				resourcePair.second.SetStatus(ResourceLoadStatus::kUnloaded);
			}
		}

		m_resourceMap.clear();

		log.Trace("Unloaded All Resource.");
	}
}