#pragma once
#include "Source/Utility/Generic/Handle.h"
#include "Source/Utility/Generic/Macros.h"

#include <EASTL/vector.h>
#include <EASTL/deque.h>

namespace Exelius
{
	class ComponentListBase
	{
	protected:
		bool m_isUpdated;
		bool m_isRendered;

	public:
		ComponentListBase(bool isUpdated = false, bool isRendered = false)
			: m_isUpdated(isUpdated)
			, m_isRendered(isRendered)
		{
			//
		}
		~ComponentListBase() = default;

		virtual void UpdateComponents() = 0;
		virtual void RenderComponents() const = 0;

		virtual void DestroyComponent(Handle handle) = 0;
	};

	template <class ComponentType>
	class ComponentList
		: public ComponentListBase
	{
		eastl::vector<ComponentType> m_components;
		eastl::vector<Handle> m_handles;
		eastl::deque<Handle> m_freeHandles;

	public:

		ComponentList(bool isUpdated = false, bool isRendered = false)
			: ComponentListBase(isUpdated, isRendered)
		{
			//
		}

		virtual ~ComponentList()
		{
		}

		Handle EmplaceComponent()
		{
			// If there are free handles available..
			if (m_freeHandles.size() > 0)
			{
				// Get the first free handle and remove it from the free list.
				Handle handle = m_freeHandles.front();
				m_freeHandles.pop_front();

				handle.Increment();
				m_handles[handle.GetId()] = handle;
				EXE_ASSERT(IsValidComponent(handle));

				return handle;
			}

			// Get the next available index.
			size_t index = m_components.size();

			// Create a new component (at the above index).
			m_components.emplace_back();

			// Create a new handle for that component.
			// This handle will refer to the component in the component list (regardless of vector resizing).
			Handle& handle = m_handles.emplace_back(index);

			handle.SetVersion(1);
			return handle;
		}

		ComponentType& GetComponent(Handle handle)
		{
			EXE_ASSERT(IsValidComponent(handle));

			return m_components[handle.GetId()];
		}

		bool IsValidComponent(Handle handle)
		{
			if (m_handles[handle.GetId()].GetVersion() == handle.GetVersion())
				return true;

			return false;
		}

		virtual void UpdateComponents() final override
		{
			if (!m_isUpdated)
				return;

			for (size_t i = 0; i < m_handles.size(); ++i)
			{
				if (m_handles[i].IsValid()) // Checks if version is not 0
					m_components[i].Update();
			}
		}

		virtual void RenderComponents() const final override
		{
			if (!m_isRendered)
				return;

			for (size_t i = 0; i < m_handles.size(); ++i)
			{
				if (m_handles[i].IsValid()) // Checks if version is not 0
					m_components[i].Render();
			}
		}

		virtual void DestroyComponent(Handle handle) final override
		{
			m_freeHandles.emplace_back(handle);
			m_handles[handle].Invalidate();
		}
	};
}