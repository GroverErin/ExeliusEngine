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

	class ScriptingSystem
	{
		sol::state* m_pLuaState;
	public:
		ScriptingSystem();
		~ScriptingSystem();

		void InitializeRuntimeScripting(Scene* pOwningScene);

		void UpdateRuntimeScripting(Scene* pOwningScene);

		void StopRuntimeScripting(Scene* pOwningScen);
	};
}