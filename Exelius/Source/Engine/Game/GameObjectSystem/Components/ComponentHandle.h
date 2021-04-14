#pragma once
#include "Source/Utility/Generic/Handle.h"
#include "Source/Engine/Game/GameObjectSystem/GameObjectSystem.h"

namespace Exelius
{
	template <class ComponentType>
	struct ComponentHandle
	{
		Handle m_internalHandle;

	public:
		ComponentHandle(Handle externalHandle)
			: m_internalHandle(externalHandle)
		{
			//
		}

		bool operator==(const ComponentHandle& other) const
		{
			return m_internalHandle == other.m_internalHandle;
		}

		bool operator!=(const ComponentHandle& other) const
		{
			return m_internalHandle != other.m_internalHandle;
		}

		ComponentType& Get()
		{
			return GameObjectSystem::GetInstance()->GetComponent<ComponentType>(m_internalHandle);
		}

		ComponentType* operator->()
		{
			return &GameObjectSystem::GetInstance()->GetComponent<ComponentType>(m_internalHandle);
		}

		operator bool() const
		{
			return GameObjectSystem::GetInstance()->IsValidComponent<ComponentType>(m_internalHandle);
		}
	};
}