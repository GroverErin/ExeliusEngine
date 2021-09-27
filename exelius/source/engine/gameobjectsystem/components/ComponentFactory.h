#pragma once
#include "source/engine/gameobjectsystem/components/Component.h"
#include "source/utility/generic/Handle.h"

#include <rapidjson/document.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class GameObject;

	/// <summary>
	/// The abstract factory that creates components. This should be
	/// subclassed in order for the Game Objects System to create
	/// custom components for GameObjects.
	/// 
	/// @note When subclassing the component factory, components will
	/// need to be registered in the Initialize function and have
	/// a Component::Type defined.
	/// 
	/// For an example on defining custom components
	/// @see TransformComponent
	/// 
	/// For an example of the use of ComponentFactory
	/// @see ExeliusComponentFactory
	/// </summary>
	class ComponentFactory
	{
	public:
		/// <summary>
		/// Destroy the component factory.
		/// </summary>
		virtual ~ComponentFactory() = default;

		/// <summary>
		/// Register all defined components. Components not registered
		/// will not be able to be used.
		/// 
		/// @todo
		/// Test what happens when attempting to use an unregistered
		/// component.
		/// </summary>
		/// <returns>True if all registration and custom initialize requirements succeed, false otherwise.</returns>
		virtual bool Initialize() = 0;

		/// <summary>
		/// Creates a component with the given type.
		/// Types are compared to the defined Type of the Component
		/// definitions. This means a couple of things:
		///		1) Failing to define a type is an error.
		///		2) Engine Components CAN be overwritten.
		/// 
		/// For clarification on 2.
		///		When the client application wishes to create
		///		a custom component factory, the client should
		///		call the ExeliusComponentFactory::CreateComponent
		///		function in the "default" case, so that any engine
		///		components would be created last, this allows
		///		for, as an example, the client to define a
		///		TransformComponent. This means the engine
		///		TranformComponent would never get created.
		/// 
		/// @todo
		/// Try to remove the use of rapidjson stuff here. It sucks.
		/// </summary>
		/// <param name="componentName">- The component to create.</param>
		/// <param name="pOwningObject">- The owner of the component to create.</param>
		/// <param name="componentData">- The raw JSON data for the component.</param>
		/// <returns>The handle to the created component. The handle will be invalid if an error occured.</returns>
		virtual Handle CreateComponent(const Component::Type& componentName,  GameObject* pOwningObject, const rapidjson::Value& componentData) = 0;
	};
}
