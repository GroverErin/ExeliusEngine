#include "EXEPCH.h"
#include "LuaScriptComponent.h"

/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void LuaScriptComponent::SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("LuaScriptComponent");
		writer.StartObject(); // Start Lua Script Component.
		{
			if (m_scriptResource.GetID().IsValid())
			{
				writer.Key("Lua Script");
				writer.String(m_scriptResource.GetID().Get().c_str(), (rapidjson::SizeType)(m_scriptResource.GetID().Get().size()));
			}
		}
		writer.EndObject(); // End Lua Script Component.
	}

	void LuaScriptComponent::DeserializeComponent(const rapidjson::Value& componentValue)
	{
		if (componentValue.FindMember("Lua Script") != componentValue.MemberEnd())
		{
			m_scriptResource.SetResourceID(componentValue.FindMember("Lua Script")->value.GetString());
			m_scriptResource.LoadNow();
		}
	}
}
