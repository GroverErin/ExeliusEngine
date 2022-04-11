#include "EXEPCH.h"
#include "ScriptingSystem.h"
#include "source/engine/scenesystem/Scene.h"
#include "source/engine/gameobjects/GameObject.h"

#include "source/engine/gameobjects/components/LuaScriptComponent.h"

#include <sol/sol.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	ScriptingSystem::ScriptingSystem()
		: m_pluaState(nullptr)
	{
		//
	}

	ScriptingSystem::~ScriptingSystem()
	{
		StopRuntimeScripting();
	}

	void ScriptingSystem::InitializeRuntimeScripting(Scene* pOwningScene)
	{
		EXE_ASSERT(pOwningScene);
		m_pluaState = EXELIUS_NEW(sol::state());
		EXE_ASSERT(m_pluaState);
		m_pluaState->open_libraries(sol::lib::base, sol::lib::package);

		auto view = pOwningScene->GetAllGameObjectsWith<LuaScriptComponent>();
		for (auto objectWithScript : view)
		{
			GameObject gameObject(objectWithScript, pOwningScene);
			LuaScriptComponent& luaScript = gameObject.GetComponent<LuaScriptComponent>();

			if (luaScript.m_scriptResource.GetID().IsValid())
			{
				auto value = m_pluaState->script_file(luaScript.m_scriptResource.GetID().Get().c_str());
				EXE_ASSERT(value.valid());
			}
		}
	}

	void ScriptingSystem::UpdateRuntimeScripting(Scene* pOwningScene)
	{
		EXE_ASSERT(pOwningScene);
		EXE_ASSERT(m_pluaState);
	}

	void ScriptingSystem::StopRuntimeScripting()
	{
		EXELIUS_DELETE(m_pluaState);
	}
}