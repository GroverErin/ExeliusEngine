#include "EXEPCH.h"
#include "ResourceHandle.h"
#include "ResourceManager.h"

namespace Exelius
{
	ResourceHandle::ResourceHandle()
		: m_resourceHeld(false)
	{
	}

	ResourceHandle::ResourceHandle(const ResourceID& resourceID, bool loadResource)
		: m_resourceID(resourceID)
		, m_resourceHeld(true)
	{
		if (loadResource)
			QueueLoad(true);
	}

	ResourceHandle::~ResourceHandle()
	{
		Release();
	}

	Resource* ResourceHandle::Get(bool forceLoad) const
	{
		return ResourceManager::GetInstance()->GetResource(m_resourceID, forceLoad);
	}

	void ResourceHandle::QueueLoad(bool signalLoaderThread, ResourceListenerPtr pListener)
	{
		ResourceManager::GetInstance()->QueueLoad(m_resourceID, signalLoaderThread, pListener);
	}

	void ResourceHandle::LoadNow(ResourceListenerPtr pListener)
	{
		ResourceManager::GetInstance()->LoadNow(m_resourceID);
	}

	void ResourceHandle::Release()
	{
		ResourceManager::GetInstance()->ReleaseResource(m_resourceID);
	}

	void ResourceHandle::SetResourceID(const ResourceID& idToSet)
	{
		if (m_resourceHeld)
			return;

		m_resourceID = idToSet;
		m_resourceHeld = true;
	}
}