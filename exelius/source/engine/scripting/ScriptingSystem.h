#pragma once

namespace sol
{
	class state;
}

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Scene;
	class GameObject;

	class ScriptingSystem
	{
		sol::state* m_pLuaState;
	public:
		ScriptingSystem();
		~ScriptingSystem();

		void InitializeRuntimeScripting(Scene* pOwningScene);

		void UpdateRuntimeScripting(Scene* pOwningScene);

		void StopRuntimeScripting(Scene* pOwningScene);

		void TryAddRuntimeScript(GameObject owningGameObject);

	private:
		void SetGlobalInputTable();

		void SetGlobalTimeTable();

		void SetGlobalLogTable();

		void SetGlobalMathTable();

		void SetGlobalUtilities();

		void SetupLuaComponents();

		void SetupLuaGameObject(Scene* pOwningScene);

		void SetupLuaPhysics(Scene* pOwningScene);

		void InitializeScriptComponents(Scene* pOwningScene);
	};
}