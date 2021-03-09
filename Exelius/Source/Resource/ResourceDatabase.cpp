#include "EXEPCH.h"
#include "Source/Resource/ResourceDatabase.h"

namespace Exelius
{
	ResourceEntry* ResourceDatabase::GetEntry(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		if (IsFound(resourceID))
		{
			m_resourceMap.at(resourceID)->IncrementRefCount();
			return m_resourceMap.at(resourceID);
		}

		return nullptr;
	}

	ResourceLoadStatus ResourceDatabase::GetLoadStatus(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		if (IsFound(resourceID))
			return m_resourceMap.at(resourceID)->GetStatus();

		return ResourceLoadStatus::kInvalid;
	}

	void ResourceDatabase::CreateEntry(const ResourceID& resourceID)
	{
		EXE_ASSERT(resourceID.IsValid());

		if (IsFound(resourceID))
		{
			EXELOG_ENGINE_INFO("Resourse Entry for {} already exists.", resourceID.Get().c_str());
			return;
		}

		m_resourceMap.emplace(resourceID, new ResourceEntry());
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

		if (IsFound(resourceID))
		{
			m_resourceMap.at(resourceID)->GetResource()->Unload();
		}
	}

	void ResourceDatabase::UnloadAll()
	{
		for (auto& resourcePair : m_resourceMap)
		{
			Resource* pResource = resourcePair.second->GetResource();
			if (pResource)
				pResource->Unload();
		}
	}
}