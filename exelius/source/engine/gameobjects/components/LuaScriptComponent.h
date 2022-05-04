#pragma once
#include "source/engine/gameobjects/Component.h"
#include "source/resource/ResourceHandle.h"

#include <sol/table.hpp>
namespace sol
{
	class state;
}

/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class GameObject;

	struct LuaScriptComponent
		: public Component
	{
		ResourceHandle m_scriptResource;

		sol::table m_scriptData;

		void InitializeScript(sol::state* pLuaState, GameObject gameObject);

		virtual void SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer) final override;

		virtual void DeserializeComponent(const rapidjson::Value& componentValue) final override;
	};
}
