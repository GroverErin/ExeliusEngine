#include "EXEPCH.h"
#include "ScriptingSystem.h"
#include "source/engine/scenesystem/Scene.h"
#include "source/engine/gameobjects/GameObject.h"

#include "source/engine/gameobjects/components/LuaScriptComponent.h"
#include "source/engine/resources/resourcetypes/TextFileResource.h"

#include "include/Input.h"

#include <sol/sol.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	ScriptingSystem::ScriptingSystem()
		: m_pLuaState(nullptr)
	{
		//
	}

	ScriptingSystem::~ScriptingSystem()
	{
		StopRuntimeScripting(nullptr);
	}

	void ScriptingSystem::InitializeRuntimeScripting(Scene* pOwningScene)
	{
		EXE_ASSERT(pOwningScene);
		m_pLuaState = EXELIUS_NEW(sol::state());
		EXE_ASSERT(m_pLuaState);
		m_pLuaState->open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::string);

		m_pLuaState->new_enum<KeyCode>("KeyCode", { {"A", KeyCode::A } });

		sol::table inputTable = m_pLuaState->create_named_table("Input");
		inputTable.set_function("IsKeyDown", IsKeyDown);
		inputTable.set_function("IsKeyPressed", IsKeyPressed);
		inputTable.set_function("IsKeyReleased", IsKeyReleased);

		auto view = pOwningScene->GetAllGameObjectsWith<LuaScriptComponent>();
		for (auto objectWithScript : view)
		{
			GameObject gameObject(objectWithScript, pOwningScene);
			LuaScriptComponent& luaScript = gameObject.GetComponent<LuaScriptComponent>();

			luaScript.InitializeScript(m_pLuaState);

			sol::function fn = luaScript.m_scriptData["OnInitialize"];

			if (fn)
			{
				sol::protected_function_result result = fn(luaScript.m_scriptData);
				if (!result.valid())
				{
					sol::error err = result;
					EXE_LOG_CATEGORY_ERROR("Lua", err.what());
				}
			}
		}
	}

	void ScriptingSystem::UpdateRuntimeScripting(Scene* pOwningScene)
	{
		EXE_ASSERT(pOwningScene);
		EXE_ASSERT(m_pLuaState);

		auto view = pOwningScene->GetAllGameObjectsWith<LuaScriptComponent>();
		for (auto objectWithScript : view)
		{
			GameObject gameObject(objectWithScript, pOwningScene);
			LuaScriptComponent& luaScript = gameObject.GetComponent<LuaScriptComponent>();

			sol::function fn = luaScript.m_scriptData["OnUpdate"];

			if (fn)
			{
				sol::protected_function_result result = fn(luaScript.m_scriptData);
				if (!result.valid())
				{
					sol::error err = result;
					EXE_LOG_CATEGORY_ERROR("Lua", err.what());
				}
			}
		}
	}

	void ScriptingSystem::StopRuntimeScripting(Scene* pOwningScene)
	{
		if (!pOwningScene)
		{
			EXELIUS_DELETE(m_pLuaState);
			return;
		}

		auto view = pOwningScene->GetAllGameObjectsWith<LuaScriptComponent>();
		for (auto objectWithScript : view)
		{
			GameObject gameObject(objectWithScript, pOwningScene);
			LuaScriptComponent& luaScript = gameObject.GetComponent<LuaScriptComponent>();

			sol::function fn = luaScript.m_scriptData["OnDestroy"];

			if (fn)
			{
				sol::protected_function_result result = fn(luaScript.m_scriptData);
				if (!result.valid())
				{
					sol::error err = result;
					EXE_LOG_CATEGORY_ERROR("Lua", err.what());
				}
			}

			// TODO: This is a hack.
			luaScript.m_scriptData.abandon();
		}

		EXELIUS_DELETE(m_pLuaState);
	}
}