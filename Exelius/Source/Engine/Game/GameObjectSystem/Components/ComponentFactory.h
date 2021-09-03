#pragma once
#include <rapidjson/document.h>

#include "Source/Engine/Game/GameObjectSystem/Components/Component.h"
#include "Source/Utility/Generic/Handle.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class GameObject;

	class ComponentFactory
	{
	public:
		virtual ~ComponentFactory() = default;
		virtual bool Initialize() = 0;
		virtual Handle CreateComponent(const Component::Type& componentName,  GameObject* pOwningObject, const rapidjson::Value& componentData) = 0;
	};
}