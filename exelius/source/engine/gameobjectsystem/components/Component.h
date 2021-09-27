#pragma once
#include "source/utility/string/StringIntern.h"

#include <rapidjson/document.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Defines the kType static variable. This is REQUIRED in order to allow a component to work.
	/// Crashes may occur otherwise.
	/// 
	/// This should be defined in the body of the Component Class Header. 
	/// 
	/// TransformComponent Example:
	///		class TransformComponent
	///			: public Component
	///		{
	///		public:
	///			// NOTE: Class/Component name *matches* parameter value. This is not a hard requirement, but is best practice.
	///			DEFINE_COMPONENT(TransformComponent);
	///		}
	/// 
	/// TODO: Make this not required?
	/// </summary>
	#define DEFINE_COMPONENT(COMPONENTTYPE) public: inline static const Type kType = #COMPONENTTYPE

	class GameObject;

	class Component
	{
	public:
		/// <summary>
		/// The Component Type. Is a StringIntern.
		/// </summary>
		using Type = StringIntern;

		/// <summary>
		/// Invalid definition of Type. Currently Unused. TODO: Can I remove this?
		/// </summary>
		inline static const Type kInvalidType = "InvalidComponent";

	protected:
		/// <summary>
		/// The Owner of this component.
		/// Used as a way to Get the GameObject from a component.
		/// </summary>
		GameObject* m_pOwner;

	public:
		/// <summary>
		/// Constructor - Initializes member variables.
		/// </summary>
		Component(GameObject* pOwner)
			: m_pOwner(pOwner)
		{
			EXE_ASSERT(pOwner);
		}

		/// <summary>
		/// Virtual Destructor
		/// </summary>
		virtual ~Component() = default;

		/// <summary>
		/// Empty Initialize the component.
		/// Empty Initialization should set any data in the component to a default or null state.
		/// </summary>
		/// <param name="pOwner">The GameObject that 'owns' this component.</param>
		/// <returns>True on success, false on failure.</returns>
		virtual bool Initialize()
		{
			return true;
		}

		/// <summary>
		/// Initialize the component.
		/// Initialization should set any data in the component to a state provided by the JSON value.
		/// 
		/// TODO:
		///		Ideally, I do not want to require the passing of rapidjson values. This requires users
		///		to know rapidjson API. Not Good.
		/// </summary>
		/// <param name="pOwner">The GameObject that 'owns' this component.</param>
		/// <param name="jsonComponentData">The GameObject that 'owns' this component.</param>
		/// <returns>True on success, false on failure.</returns>
		virtual bool Initialize([[maybe_unused]] const rapidjson::Value& jsonComponentData)
		{
			EXE_ASSERT(m_pOwner);
			return true;
		}

		/// <summary>
		/// Updates this component.
		/// </summary>
		virtual void Update() {}

		/// <summary>
		/// Renders this component.
		/// </summary>
		virtual void Render() const {}

		/// <summary>
		/// Get the GameObject that 'owns' this component.
		/// </summary>
		/// <returns></returns>
		const GameObject* GetOwner()
		{
			// m_pOwner should NEVER be null here.
			EXE_ASSERT(m_pOwner);
			return m_pOwner;
		}

		/// <summary>
		/// This function should be inhereted to release any resources before final destruction.
		/// </summary>
		virtual void Destroy() {}
	};
}