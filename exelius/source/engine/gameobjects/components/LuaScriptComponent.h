#pragma once
#include "source/engine/gameobjects/Component.h"
#include "source/resource/ResourceHandle.h"

/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct LuaScriptComponent
		: public Component
	{
		ResourceHandle m_scriptResource;

		virtual void SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer) final override;

		virtual void DeserializeComponent(const rapidjson::Value& componentValue) final override;
	};
}
