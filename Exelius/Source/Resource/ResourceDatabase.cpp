#include "EXEPCH.h"
#include "Source/Resource/ResourceDatabase.h"

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

		if (IsFound(resourceID))
		{
			EXELOG_ENGINE_INFO("Resource Entry for {} already exists.", resourceID.Get().c_str());
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
		EXELOG_ENGINE_TRACE("Unloading Resource: {}", resourceID.Get().c_str());

		if (!IsFound(resourceID))
			return;

		EXELOG_ENGINE_TRACE("Resource State: {}", m_resourceMap.at(resourceID).GetStatus());

		m_resourceMap.at(resourceID).SetStatus(ResourceLoadStatus::kUnloading);

		Resource* pResource = m_resourceMap.at(resourceID).GetResource();
		if (pResource)
		{
			pResource->Unload();
			m_resourceMap.at(resourceID).SetStatus(ResourceLoadStatus::kUnloaded);
		}

		m_resourceMap.erase(resourceID);
		EXELOG_ENGINE_TRACE("Unloaded Resource.");
}

	void ResourceDatabase::UnloadAll()
	{
		EXELOG_ENGINE_TRACE("Unloading All Resources.");

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

		EXELOG_ENGINE_TRACE("Unloaded All Resource.");
	}
}