#include "EXEPCH.h"
#include "LuaScriptComponent.h"

#include "source/engine/gameobjects/GameObject.h"
#include "source/engine/resources/resourcetypes/TextFileResource.h"
#include "source/utility/io/File.h"

#include <sol/sol.hpp>

/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void LuaScriptComponent::InitializeScript(sol::state* pLuaState, GameObject gameObject)
	{
		EXE_ASSERT(pLuaState);

		if (!m_scriptResource.IsReferenceHeld())
			m_scriptResource.LoadNow(); // TODO: The script should be loaded already.

		TextFileResource* pScriptResource = m_scriptResource.GetAs<TextFileResource>();
		if (!pScriptResource)
		{
			EXE_LOG_CATEGORY_WARN("ScriptSystem", "Failed to initialize script '{}'", m_scriptResource.GetID().Get().c_str());
			return;
		}

		eastl::string fileNameNoExtenstion = File::GetFileName(pScriptResource->GetResourceID().Get());

		m_scriptData = pLuaState->script(pScriptResource->GetRawText().c_str(), pScriptResource->GetResourceID().Get().c_str());
		m_scriptData["gameObject"] = gameObject;

		EXE_ASSERT(m_scriptData.valid());
	}

	void LuaScriptComponent::SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("LuaScriptComponent");
		writer.StartObject(); // Start Lua Script Component.
		{
			if (m_scriptResource.IsReferenceHeld())
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
			EXE_ASSERT(!m_scriptResource.IsReferenceHeld()); // Deserializing into an existing component shouldn't be possible.
			m_scriptResource.SetResourceID(componentValue.FindMember("Lua Script")->value.GetString());
			m_scriptResource.LoadNow();
		}
	}
}
