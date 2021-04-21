#pragma once
#include "Source/Utility/Generic/Handle.h"
#include "Source/Utility/Generic/Macros.h"

#include <EASTL/vector.h>
#include <EASTL/deque.h>

namespace Exelius
{
	/// <summary>
	/// The ComponentListBase class allows for generalized storage of templated ComponentLists.
	/// 
	/// Components in the ComponentList class(es) are pooled. See: ComponentList below.
	/// </summary>
	class ComponentListBase
	{
	protected:
		/// <summary>
		/// Are the Components in this ComponentList updated every frame?
		/// </summary>
		bool m_isUpdated;

		/// <summary>
		/// Are the Components in this ComponentList rendered every frame?
		/// </summary>
		bool m_isRendered;

	public:
		/// <summary>
		/// Constructor - Sets the render and update booleans.
		/// </summary>
		/// <param name="isUpdated">True if updated, false if not.</param>
		/// <param name="isRendered">True if rendered, false if not.param>
		ComponentListBase(bool isUpdated = false, bool isRendered = false)
			: m_isUpdated(isUpdated)
			, m_isRendered(isRendered)
		{
			//
		}

		/// <summary>
		/// Virtual Destructor.
		/// </summary>
		virtual ~ComponentListBase() = default;

		/// <summary>
		/// Update the components in this list if
		/// this list is set to update them.
		/// </summary>
		virtual void UpdateComponents() = 0;

		/// <summary>
		/// Render the components in this list if
		/// this list is set to render them.
		/// </summary>
		virtual void RenderComponents() const = 0;

		/// <summary>
		/// Releases a component into the component pool.
		/// This does not destroy or reset a component.
		/// 
		/// TODO: I'd like for this to not be virtual.
		/// </summary>
		/// <param name="handle">The handle of the component to release.</param>
		virtual void ReleaseComponent(Handle handle) = 0;
	};

	/// <summary>
	/// Templated ComponentListBase subclass.
	/// This is a List of template specified component types.
	/// The lists of components by value allow for cache-friendly
	/// iteration. The hope is to have extremely fast Update and Render calls
	/// that remove as virtual function calls as possible.
	/// 
	/// It uses a system of Handles to maintain a Pool of Components.
	/// The handles allow for easy creation, removal, and validation
	/// of components.
	/// 
	/// TODO: Maybe make this a separate '.h' file?
	/// </summary>
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
			EXELOG_ENGINE_TRACE("ComponentList<{}> Reached.", ComponentType::kType.Get().c_str());

			// Need to explicitly shut components down.
			// This is what will release the resources owned by the components.
			for (auto& component : m_components)
			{
				component.Destroy();
			}
		}

		Handle EmplaceComponent()
		{
			// If there are free handles available..
			if (m_freeHandles.size() > 0)
			{
				// Get the first free handle and remove it from the free list.
				Handle handle = m_freeHandles.front();
				m_freeHandles.pop_front();

				handle.IncrementVersion();
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
				// Do not update inactive components.
				if (!m_handles[i].IsValid())
					continue;

				auto& component = m_components[i];
				
				// Ideally, there should be very little branching in this loop.
				// Maybe this could be a check that happens per-component? (In each components update...)
				// Another idea might be to invalidate component handles when a gameobject is disabled?
				//if (component.GetOwner()->IsEnabled())
				component.Update();
			}
		}

		virtual void RenderComponents() const final override
		{
			if (!m_isRendered)
				return;

			for (size_t i = 0; i < m_handles.size(); ++i)
			{
				// Do not render inactive components.
				if (!m_handles[i].IsValid())
					continue;

				auto& component = m_components[i];

				// Ideally, there should be very little branching in this loop.
				// Maybe this could be a check that happens per-component? (In each components render...)
				// Another idea might be to invalidate component handles when a gameobject is disabled?
				//if (component.GetOwner()->IsEnabled())
				component.Render();
			}
		}

		virtual void ReleaseComponent(Handle handle) final override
		{
			EXE_ASSERT(handle.IsValid());

			m_freeHandles.emplace_back(handle);
			m_handles[handle.GetId()].Invalidate();
		}
	};
}