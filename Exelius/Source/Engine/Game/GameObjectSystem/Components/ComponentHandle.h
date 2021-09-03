#pragma once
#include "Source/Utility/Generic/Handle.h"
#include "Source/Engine/Game/GameObjectSystem/GameObjectSystem.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated Component wrapper. It functions as a Component
	/// when the Get() function or operator->() is used.
	/// 
	/// This allows for a simple API,
	/// and safe use of volatile components.
	/// 
	/// NOTE/TODO:
	///		Asserting in this class in some functions might not be a wise option.
	/// </summary>
	template <class ComponentType>
	struct ComponentHandle
	{
		/// <summary>
		/// Handle that refers to a component in a component list,
		/// inside of the GameObjectSystem.
		/// </summary>
		Handle m_internalHandle;

	public:
		/// <summary>
		/// Default Constructor - Creates an *invalid* ComponentHandle.
		/// </summary>
		ComponentHandle()
			: m_internalHandle(0) // Invalid.
		{
			//
		}

		/// <summary>
		/// Constructor - Creates a ComponentHandle 
		/// </summary>
		/// <param name="externalHandle">The handle for a component.</param>
		ComponentHandle(Handle externalHandle)
			: m_internalHandle(externalHandle)
		{
			EXE_ASSERT(m_internalHandle.IsValid());
		}

		/// <summary>
		/// Overload for the operator==()
		/// Allows for fast Component Comparisons.
		/// </summary>
		/// <param name="other">The component to compare with.</param>
		/// <returns>True if components match, false if not matching.</returns>
		bool operator==(const ComponentHandle& other) const
		{
			EXE_ASSERT(m_internalHandle.IsValid());
			EXE_ASSERT(other.m_internalHandle.IsValid());

			return m_internalHandle == other.m_internalHandle;
		}

		/// <summary>
		/// Overload for the operator!=()
		/// Allows for fast Component Comparisons.
		/// </summary>
		/// <param name="other">The component to compare with.</param>
		/// <returns> True if not matching, False if components match.</returns>
		bool operator!=(const ComponentHandle& other) const
		{
			EXE_ASSERT(m_internalHandle.IsValid());
			EXE_ASSERT(other.m_internalHandle.IsValid());

			return m_internalHandle != other.m_internalHandle;
		}

		/// <summary>
		/// Get the component by reference.
		/// NOTE:
		///		This is UNSAFE to call if the component is invalid!
		///		Use the IsValid() function to validate.
		/// </summary>
		/// <returns>Component Reference.</returns>
		ComponentType& Get()
		{
			EXE_ASSERT(GameObjectSystem::GetInstance());
			EXE_ASSERT(m_internalHandle.IsValid());
			return GameObjectSystem::GetInstance()->GetComponent<ComponentType>(m_internalHandle);
		}

		/// <summary>
		/// Overload for the operator->()
		/// Get the component by pointer.
		/// NOTE:
		///		This is UNSAFE to call if the component is invalid!
		///		Use the IsValid() function to validate.
		/// </summary>
		/// <returns>Pointer to component, nullptr upon failure.</returns>
		ComponentType* operator->()
		{
			EXE_ASSERT(GameObjectSystem::GetInstance());
			EXE_ASSERT(m_internalHandle.IsValid());
			return &GameObjectSystem::GetInstance()->GetComponent<ComponentType>(m_internalHandle);
		}

		/// <summary>
		/// Checks if the Component referred to by this Handle is Valid.
		/// </summary>
		/// <returns>True if Valid, false if not.</returns>
		bool IsValid() const
		{
			EXE_ASSERT(GameObjectSystem::GetInstance());
			return GameObjectSystem::GetInstance()->IsValidComponent<ComponentType>(m_internalHandle);
		}

		/// <summary>
		/// Overload for the operator bool() const
		/// Checks if the Component referred to by this Handle is Valid.
		/// </summary>
		/// <returns>True if Valid, false if not.</returns>
		operator bool() const
		{
			return IsValid();
		}
	};
}