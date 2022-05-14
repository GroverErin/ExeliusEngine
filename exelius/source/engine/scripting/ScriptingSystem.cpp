#include "EXEPCH.h"
#include "ScriptingSystem.h"
#include "source/engine/scenesystem/Scene.h"
#include "source/engine/physics/PhysicsSystem.h"
#include "source/engine/gameobjects/GameObject.h"

#include "source/engine/gameobjects/components/BoxColliderComponent.h"
#include "source/engine/gameobjects/components/CameraComponent.h"
#include "source/engine/gameobjects/components/CircleColliderComponent.h"
#include "source/engine/gameobjects/components/CircleRendererComponent.h"
#include "source/engine/gameobjects/components/GUIDComponent.h"
#include "source/engine/gameobjects/components/NameComponent.h"
#include "source/engine/gameobjects/components/RigidbodyComponent.h"
#include "source/engine/gameobjects/components/SpriteRendererComponent.h"
#include "source/engine/gameobjects/components/TransformComponent.h"
#include "source/engine/gameobjects/components/LuaScriptComponent.h"

#include "source/engine/resources/resourcetypes/TextFileResource.h"
#include "source/engine/resources/resourcetypes/TextureResource.h"
#include "source/render/Texture.h"

#include "include/Input.h"
#include "include/Time.h"

#include "source/utility/random/Random.h"
#include "source/utility/random/noise/PerlinNoise.h"
#include "source/utility/random/noise/SquirrelNoise.h"
#include "source/utility/math/Math.h"

#include <box2d/box2d.h>
#include <sol/sol.hpp>
#include <EASTL/vector.h>

#define ADD_LUA_KEYCODE(KEY) { #KEY, KeyCode::##KEY }

#define ADD_ADDCOMPONENT(COMPONENT) "Add"#COMPONENT, &GameObject::AddComponent<##COMPONENT>
#define ADD_REMOVECOMPONENT(COMPONENT) "Remove"#COMPONENT, &GameObject::RemoveComponent<##COMPONENT>
#define ADD_GETCOMPONENT(COMPONENT) "Get"#COMPONENT, [](GameObject& gameObject, sol::this_state state){ sol::state_view luaState(state); if (gameObject.HasComponent<##COMPONENT>()) return sol::make_object(luaState, &gameObject.GetComponent<##COMPONENT>()); else return sol::make_object(luaState, sol::nil); }

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
		m_pLuaState->open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::string, sol::lib::math);

		SetGlobalInputTable();
		SetGlobalTimeTable();
		SetGlobalLogTable();
		SetGlobalMathTable();
		SetGlobalUtilities();
		SetupLuaComponents();
		SetupLuaGameObject(pOwningScene);
		SetupLuaPhysics(pOwningScene);

		InitializeScriptComponents(pOwningScene);
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

			if (!luaScript.m_didInitialize)
			{
				luaScript.InitializeScript(m_pLuaState, gameObject);

				if (!luaScript.m_didInitialize)
					continue;

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

			sol::protected_function fn = luaScript.m_scriptData["OnUpdate"];

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

		if (!m_pLuaState)
			return;

		auto view = pOwningScene->GetAllGameObjectsWith<LuaScriptComponent>();
		for (auto objectWithScript : view)
		{
			GameObject gameObject(objectWithScript, pOwningScene);
			DestroyGameObject(gameObject);
		}

		m_pLuaState->collect_garbage();

		EXELIUS_DELETE(m_pLuaState);

		// Reset the timescale. In case it was set via scripts.
		Time.SetTimeScale();
	}

	void ScriptingSystem::TryAddRuntimeScript(GameObject owningGameObject)
	{
		if (!m_pLuaState)
			return; // We aren't running the simulation so, we can skip this.

		LuaScriptComponent& luaScript = owningGameObject.GetComponent<LuaScriptComponent>();

		luaScript.InitializeScript(m_pLuaState, owningGameObject);

		if (!luaScript.m_didInitialize)
			return;

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

	void ScriptingSystem::DestroyGameObject(GameObject gameObjectToDestroy)
	{
		LuaScriptComponent& luaScript = gameObjectToDestroy.GetComponent<LuaScriptComponent>();

		if (!luaScript.m_didInitialize)
		{
			luaScript.InitializeScript(m_pLuaState, gameObjectToDestroy);

			if (!luaScript.m_didInitialize)
				return;

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

		sol::protected_function  fn = luaScript.m_scriptData["OnDestroy"];

		if (fn)
		{
			sol::protected_function_result result = fn(luaScript.m_scriptData);
			if (!result.valid())
			{
				sol::error err = result;
				EXE_LOG_CATEGORY_ERROR("Lua", err.what());
			}
		}

		// TODO: This is a hack. It prevents a crash that I am not sure about yet.
		luaScript.m_scriptData.abandon();
	}

	void ScriptingSystem::SetGlobalInputTable()
	{
		sol::table inputTable = m_pLuaState->create_named_table("Input");

		m_pLuaState->new_enum<KeyCode>("KeyCode",
			{
				ADD_LUA_KEYCODE(Num0),
				ADD_LUA_KEYCODE(Num1),
				ADD_LUA_KEYCODE(Num2),
				ADD_LUA_KEYCODE(Num3),
				ADD_LUA_KEYCODE(Num4),
				ADD_LUA_KEYCODE(Num5),
				ADD_LUA_KEYCODE(Num6),
				ADD_LUA_KEYCODE(Num7),
				ADD_LUA_KEYCODE(Num8),
				ADD_LUA_KEYCODE(Num9),

				ADD_LUA_KEYCODE(A),
				ADD_LUA_KEYCODE(B),
				ADD_LUA_KEYCODE(C),
				ADD_LUA_KEYCODE(D),
				ADD_LUA_KEYCODE(E),
				ADD_LUA_KEYCODE(F),
				ADD_LUA_KEYCODE(G),
				ADD_LUA_KEYCODE(H),
				ADD_LUA_KEYCODE(I),
				ADD_LUA_KEYCODE(J),
				ADD_LUA_KEYCODE(K),
				ADD_LUA_KEYCODE(L),
				ADD_LUA_KEYCODE(M),
				ADD_LUA_KEYCODE(N),
				ADD_LUA_KEYCODE(O),
				ADD_LUA_KEYCODE(P),
				ADD_LUA_KEYCODE(Q),
				ADD_LUA_KEYCODE(R),
				ADD_LUA_KEYCODE(S),
				ADD_LUA_KEYCODE(T),
				ADD_LUA_KEYCODE(U),
				ADD_LUA_KEYCODE(V),
				ADD_LUA_KEYCODE(W),
				ADD_LUA_KEYCODE(X),
				ADD_LUA_KEYCODE(Y),
				ADD_LUA_KEYCODE(Z),

				ADD_LUA_KEYCODE(Escape),
				ADD_LUA_KEYCODE(Space),
				ADD_LUA_KEYCODE(Quote),
				ADD_LUA_KEYCODE(LBracket),
				ADD_LUA_KEYCODE(RBracket),

				ADD_LUA_KEYCODE(LControl),
				ADD_LUA_KEYCODE(LShift),
				ADD_LUA_KEYCODE(LAlt),
				ADD_LUA_KEYCODE(LSystem),
				ADD_LUA_KEYCODE(RControl),
				ADD_LUA_KEYCODE(RShift),
				ADD_LUA_KEYCODE(RAlt),
				ADD_LUA_KEYCODE(RSystem),
				ADD_LUA_KEYCODE(Menu),

				ADD_LUA_KEYCODE(Tab),
				ADD_LUA_KEYCODE(Backspace),
				ADD_LUA_KEYCODE(Enter),
				ADD_LUA_KEYCODE(Comma),
				ADD_LUA_KEYCODE(Period),
				ADD_LUA_KEYCODE(Slash),
				ADD_LUA_KEYCODE(Backslash),
				ADD_LUA_KEYCODE(Tilde),
				ADD_LUA_KEYCODE(Equal),
				ADD_LUA_KEYCODE(Hyphen),
				ADD_LUA_KEYCODE(Semicolon),

				ADD_LUA_KEYCODE(PageUp),
				ADD_LUA_KEYCODE(PageDown),

				ADD_LUA_KEYCODE(End),
				ADD_LUA_KEYCODE(Home),
				ADD_LUA_KEYCODE(Insert),
				ADD_LUA_KEYCODE(Delete),

				ADD_LUA_KEYCODE(Divide),
				ADD_LUA_KEYCODE(Multiply),
				ADD_LUA_KEYCODE(Add),
				ADD_LUA_KEYCODE(Subtract),

				ADD_LUA_KEYCODE(Left),
				ADD_LUA_KEYCODE(Right),
				ADD_LUA_KEYCODE(Up),
				ADD_LUA_KEYCODE(Down),

				ADD_LUA_KEYCODE(Numpad0),
				ADD_LUA_KEYCODE(Numpad1),
				ADD_LUA_KEYCODE(Numpad2),
				ADD_LUA_KEYCODE(Numpad3),
				ADD_LUA_KEYCODE(Numpad4),
				ADD_LUA_KEYCODE(Numpad5),
				ADD_LUA_KEYCODE(Numpad6),
				ADD_LUA_KEYCODE(Numpad7),
				ADD_LUA_KEYCODE(Numpad8),
				ADD_LUA_KEYCODE(Numpad9),

				ADD_LUA_KEYCODE(F1),
				ADD_LUA_KEYCODE(F2),
				ADD_LUA_KEYCODE(F3),
				ADD_LUA_KEYCODE(F4),
				ADD_LUA_KEYCODE(F5),
				ADD_LUA_KEYCODE(F6),
				ADD_LUA_KEYCODE(F7),
				ADD_LUA_KEYCODE(F8),
				ADD_LUA_KEYCODE(F9),
				ADD_LUA_KEYCODE(F10),
				ADD_LUA_KEYCODE(F11),
				ADD_LUA_KEYCODE(F12),
				ADD_LUA_KEYCODE(F13),
				ADD_LUA_KEYCODE(F14),
				ADD_LUA_KEYCODE(F15),
				ADD_LUA_KEYCODE(Pause)
			});

		sol::table inputMetaTable = m_pLuaState->create_table_with();
		inputMetaTable.set_function("IsKeyDown", IsKeyDown);
		inputMetaTable.set_function("IsKeyPressed", IsKeyPressed);
		inputMetaTable.set_function("IsKeyReleased", IsKeyReleased);

		inputMetaTable[sol::meta_function::new_index] = [](lua_State* pLuaState) {return luaL_error(pLuaState, "Modifying 'Input' table is restricted."); };
		inputMetaTable[sol::meta_function::index] = inputMetaTable;

		inputTable[sol::metatable_key] = inputMetaTable;
	}

	void ScriptingSystem::SetGlobalTimeTable()
	{
		auto type = m_pLuaState->new_usertype<GameTime::TimeScaler>("TimeScaler");
		type[sol::meta_function::multiplication] = [](float left, GameTime::TimeScaler& timeScaler)
		{
			return timeScaler * left;
		};

		type[sol::meta_function::addition] = [](float left, GameTime::TimeScaler& timeScaler)
		{
			return timeScaler + left;
		};

		type[sol::meta_function::subtraction] = [](float left, GameTime::TimeScaler& timeScaler)
		{
			return timeScaler - left;
		};

		sol::table timeTable = m_pLuaState->create_named_table("Time");

		sol::table timeMetaTable = m_pLuaState->create_table_with();

		timeMetaTable["DeltaTime"] = &Time.DeltaTime;
		timeMetaTable["DeltaTimeUnscaled"] = &Time.DeltaTimeUnscaled;
		timeMetaTable["ElapsedTime"] = &Time.ElapsedTime;
		timeMetaTable.set_function("SetTimeScale", Time.SetTimeScale);

		timeMetaTable[sol::meta_function::new_index] = [](lua_State* pLuaState) {return luaL_error(pLuaState, "Modifying 'Time' table is restricted."); };
		timeMetaTable[sol::meta_function::index] = timeMetaTable;

		timeTable[sol::metatable_key] = timeMetaTable;
	}

	static eastl::string GetLuaPrintMessage(lua_State* pLuaState)
	{
		eastl::string message;

		int n = lua_gettop(pLuaState);  /* number of arguments */
		int i;
		for (i = 1; i <= n; i++) {  /* for each argument */
			size_t l;
			const char* s = luaL_tolstring(pLuaState, i, &l);  /* convert it to string */
			if (i > 1)  /* not the first element? */
				message += "\t"; /* add a tab before it */
			message += s; /* append it */
			lua_pop(pLuaState, 1);  /* pop result */
		}

		return message;
	}
	
	static int LuaTraceLog(lua_State* pLuaState)
	{
		const eastl::string& message = GetLuaPrintMessage(pLuaState);
		EXE_LOG_CATEGORY_TRACE("Lua", message.c_str());
		return 0;
	}

	static int LuaWarnLog(lua_State* pLuaState)
	{
		const eastl::string& message = GetLuaPrintMessage(pLuaState);
		EXE_LOG_CATEGORY_WARN("Lua", message.c_str());
		return 0;
	}

	static int LuaInfoLog(lua_State* pLuaState)
	{
		const eastl::string& message = GetLuaPrintMessage(pLuaState);
		EXE_LOG_CATEGORY_INFO("Lua", message.c_str());
		return 0;
	}

	static int LuaErrorLog(lua_State* pLuaState)
	{
		const eastl::string& message = GetLuaPrintMessage(pLuaState);
		EXE_LOG_CATEGORY_ERROR("Lua", message.c_str());
		return 0;
	}

	static int LuaFatalLog(lua_State* pLuaState)
	{
		const eastl::string& message = GetLuaPrintMessage(pLuaState);
		EXE_LOG_CATEGORY_FATAL("Lua", message.c_str());
		return 0;
	}

	void ScriptingSystem::SetGlobalLogTable()
	{
		m_pLuaState->set_function("print", LuaTraceLog);
		m_pLuaState->set_function("warn", LuaWarnLog);
		sol::table logTable = m_pLuaState->create_named_table("Log");

		sol::table logMetaTable = m_pLuaState->create_table_with();

		logMetaTable.set_function("Trace", LuaTraceLog);
		logMetaTable.set_function("Info", LuaInfoLog);
		logMetaTable.set_function("Warn", LuaWarnLog);
		logMetaTable.set_function("Error", LuaErrorLog);
		logMetaTable.set_function("Fatal", LuaFatalLog);

		logMetaTable[sol::meta_function::new_index] = [](lua_State* pLuaState) {return luaL_error(pLuaState, "Modifying 'Log' table is restricted."); };
		logMetaTable[sol::meta_function::index] = logMetaTable;

		logTable[sol::metatable_key] = logMetaTable;
	}
	
	void ScriptingSystem::SetGlobalMathTable()
	{
		sol::table mathTable = m_pLuaState->create_named_table("ExeliusMath"); // Not sure if just "Math" will work.
		sol::table mathMetaTable = m_pLuaState->create_table_with();

		mathMetaTable.set_function("Lerp", [](float min, float max, float weight) { return Lerp(min, max, weight); });
		mathMetaTable.set_function("SmootherStep", [](float x) { return SmootherStep(x); });
		mathMetaTable.set_function("Normalize", [](float x, float min, float max) { return Normalize(x, min, max); });

		mathMetaTable[sol::meta_function::new_index] = [](lua_State* pLuaState) {return luaL_error(pLuaState, "Modifying 'ExeliusMath' table is restricted."); };
		mathMetaTable[sol::meta_function::index] = mathMetaTable;

		mathTable[sol::metatable_key] = mathMetaTable;
	}

	void ScriptingSystem::SetGlobalUtilities()
	{
		m_pLuaState->new_usertype<GUID>("GUID");

		m_pLuaState->new_usertype<glm::vec2>("Vec2",
			"x", &glm::vec2::x,
			"y", &glm::vec2::y
			);

		m_pLuaState->new_usertype<glm::vec3>("Vec3",
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z
			);

		m_pLuaState->new_usertype<glm::vec4>("Vec4",
			"x", &glm::vec4::x,
			"y", &glm::vec4::y,
			"z", &glm::vec4::z,
			"w", &glm::vec4::w
			);

		m_pLuaState->new_usertype<Color>("Color", sol::constructors<Color(), Color(uint8_t), Color(uint8_t, uint8_t, uint8_t), Color(uint8_t, uint8_t, uint8_t, uint8_t), Color(const glm::vec4&)>(),
			"r", &Color::r,
			"g", &Color::g,
			"b", &Color::b,
			"a", &Color::a
			);

		auto resourceIDType = m_pLuaState->new_usertype<ResourceID>("ResourceID", sol::constructors<ResourceID(), ResourceID(const char*)>(),
			"Get", [](ResourceID& id) { return id.Get().c_str(); },
			"IsValid", &ResourceID::IsValid
			);

		resourceIDType[sol::meta_function::to_string] = [](ResourceID& id) { return id.Get().c_str(); };

		m_pLuaState->new_usertype<Texture>("Texture", sol::constructors<Texture(uint32_t, uint32_t)>(),
			"Bind", [](Texture& texture, uint32_t slot) { texture.Bind(slot); },
			"GetWidth", [](Texture& texture) { return texture.GetWidth(); },
			"GetHeight", [](Texture& texture) { return texture.GetHeight(); },
			"GetRendererID", [](Texture& texture) { return texture.GetRendererID(); },
			"SetData", [](Texture& texture, sol::table data, uint32_t size)
			{
				if (size != texture.GetWidth() * texture.GetHeight())
				{
					EXE_LOG_CATEGORY_ERROR("Lua", "Data size does not match Texture size!");
					return;
				}

				eastl::vector<uint32_t> hexColorData(size, 0xFFFFFFFF);
				for (auto it = data.begin(); it != data.end(); ++it)
				{
					auto firstType = (*it).first.get_type();
					if (firstType != sol::type::number)
					{
						EXE_LOG_CATEGORY_ERROR("Lua", "Texture:SetData(): Invalid key found in data. Data must be in the form of an array.");
						return;
					}

					if (!(*it).second.is<Color>())
					{
						EXE_LOG_CATEGORY_ERROR("Lua", "Texture:SetData(): Invalid value found in data. Values must be of 'Color' type.");
						return; // Invalid key entry.
					}

					if ((*it).first.as<size_t>() >= size)
					{
						EXE_LOG_CATEGORY_ERROR("Lua", "Texture:SetData(): Indices larger than the array size. Make sure to start at 0.");
						return; // Invalid key entry.
					}

					hexColorData[(*it).first.as<size_t>()] = (*it).second.as<Color>().GetHex();
				}
				texture.SetData(hexColorData.data(), size * 4);
			},
			"IsLoaded", [](Texture& texture) { return texture.IsLoaded(); }
		);

		m_pLuaState->new_usertype<TextFileResource>("TextFileResource");
		m_pLuaState->new_usertype<TextureResource>("TextureResource",
			"GetTexture", [](TextureResource& resource) { return resource.GetTexture(); },
			"SetTexture", [](TextureResource& resource, sol::object texture)
			{
				if (texture.is<Texture>())
				{
					Texture* pTexture = EXELIUS_NEW(Texture(texture.as<Texture>()));
					resource.SetTexture(pTexture);
				}
			}
		);

		m_pLuaState->new_usertype<ResourceHandle>("ResourceHandle", sol::constructors<ResourceHandle(), ResourceHandle(ResourceID), ResourceHandle(ResourceID, bool)>(),
			"Release", [](ResourceHandle& handle) { return handle.Release(); },
			"SetResourceID", [](ResourceHandle& handle, ResourceID& resourceID) { handle.SetResourceID(resourceID); },
			"IsReferenceHeld", [](ResourceHandle& handle) { return handle.IsReferenceHeld(); },
			"LoadNow", [](ResourceHandle& handle) { return handle.LoadNow(); },
			"LockResource", [](ResourceHandle& handle) { return handle.LockResource(); },
			"UnlockResource", [](ResourceHandle& handle) { return handle.UnlockResource(); },
			"CreateResource", [](ResourceHandle& handle, ResourceID& resourceID) { return handle.CreateNew(resourceID); },
			"GetAsTextureResource", [](ResourceHandle& handle) { return handle.GetAs<TextureResource>(); },
			"GetAsTextFileResource", [](ResourceHandle& handle) { return handle.GetAs<TextFileResource>(); }
		);

		m_pLuaState->new_usertype<Random>("Random", sol::constructors<Random(), Random(int), Random(int, int)>(),
			"Rand", [](Random& rand) { return (unsigned long)rand.Rand(); }, // We truncate the value here because lua can't handle a long long. Absolutely NOT ideal.
			"FRand", [](Random& rand) { return rand.FRand(); },
			"IRandomRange", [](Random& rand, int min, int max) { return rand.IRandomRange(min, max); },
			"FRandomRange", [](Random& rand, float min, float max) { return rand.FRandomRange(min, max); }
			);

		auto perlinNoiseType = m_pLuaState->new_usertype<PerlinNoise>("PerlinNoise");
		perlinNoiseType.set_function("GetSeed", &PerlinNoise::GetSeed);
		perlinNoiseType.set_function("SetSeed", [](PerlinNoise& noise, unsigned int seed) { noise.SetSeed(seed); });
		perlinNoiseType.set_function("GetNoise", sol::overload(
			[](PerlinNoise& noise, float x, float y, float maxX, float maxY, unsigned int noiseInputRange) { return noise.GetNoise(x, y, maxX, maxY, noiseInputRange); },
			[](PerlinNoise& noise, float x, float y, float maxX, float maxY, unsigned int noiseInputRange, unsigned int seedOverride) { return noise.GetNoise(x, y, maxX, maxY, noiseInputRange, seedOverride); },
			[](PerlinNoise& noise, float x, float y) { return noise.GetNoise(x, y); },
			[](PerlinNoise& noise, float x, float y, unsigned int seedOverride) { return noise.GetNoise(x, y, seedOverride); }
		));
		perlinNoiseType.set_function("GetNormalizedNoise", sol::overload(
			[](PerlinNoise& noise, float x, float y) { return noise.GetNormalizedNoise(x, y); },
			[](PerlinNoise& noise, float x, float y, unsigned int seedOverride) { return noise.GetNormalizedNoise(x, y, seedOverride); }
		));
		perlinNoiseType.set_function("GetAverageNoise", sol::overload(
			[](PerlinNoise& noise, float x, float y, float maxX, float maxY, unsigned int noiseInputRange, unsigned int numOctaves, float persistance) { return noise.GetAverageNoise(x, y, maxX, maxY, noiseInputRange, numOctaves, persistance); },
			[](PerlinNoise& noise, float x, float y, float maxX, float maxY, unsigned int noiseInputRange, unsigned int numOctaves, float persistance, unsigned int seedOverride) { return noise.GetAverageNoise(x, y, maxX, maxY, noiseInputRange, numOctaves, persistance, seedOverride); }
		));
		
		m_pLuaState->new_usertype<SquirrelNoise>("SquirrelNoise",
			"GetSeed", &SquirrelNoise::GetSeed,
			"SetSeed", [](SquirrelNoise& noise, unsigned int seed) { noise.SetSeed(seed); },
			"Get1DNoise", sol::overload(
				[](SquirrelNoise& noise, int x, unsigned int seedOverride) { return noise.Get1DNoise(x, seedOverride); },
				[](SquirrelNoise& noise, int x) { return noise.Get1DNoise(x); }
			),
			"GetUniform1DNoise", sol::overload(
				[](SquirrelNoise& noise, int x, unsigned int seedOverride) { return noise.GetUniform1DNoise(x, seedOverride); },
				[](SquirrelNoise& noise, int x) { return noise.GetUniform1DNoise(x); }
			),
			"GetSignedUniform1DNoise", sol::overload(
				[](SquirrelNoise& noise, int x, unsigned int seedOverride) { return noise.GetSignedUniform1DNoise(x, seedOverride); },
				[](SquirrelNoise& noise, int x) { return noise.GetSignedUniform1DNoise(x); }
			),
			"Get2DNoise", sol::overload(
				[](SquirrelNoise& noise, int x, int y, unsigned int seedOverride) { return noise.Get2DNoise(x, y, seedOverride); },
				[](SquirrelNoise& noise, int x, int y) { return noise.Get2DNoise(x, y); }
			),
			"GetUniform2DNoise", sol::overload(
				[](SquirrelNoise& noise, int x, int y, unsigned int seedOverride) { return noise.GetUniform2DNoise(x, y, seedOverride); },
				[](SquirrelNoise& noise, int x, int y) { return noise.GetUniform2DNoise(x, y); }
			),
			"GetSignedUniform2DNoise", sol::overload(
				[](SquirrelNoise& noise, int x, int y, unsigned int seedOverride) { return noise.GetSignedUniform2DNoise(x, y, seedOverride); },
				[](SquirrelNoise& noise, int x, int y) { return noise.GetSignedUniform2DNoise(x, y); }
			),
			"Get3DNoise", sol::overload(
				[](SquirrelNoise& noise, int x, int y, int z, unsigned int seedOverride) { return noise.Get3DNoise(x, y, z, seedOverride); },
				[](SquirrelNoise& noise, int x, int y, int z) { return noise.Get3DNoise(x, y, z); }
			),
			"GetUniform3DNoise", sol::overload(
				[](SquirrelNoise& noise, int x, int y, int z, unsigned int seedOverride) { return noise.GetUniform3DNoise(x, y, z, seedOverride); },
				[](SquirrelNoise& noise, int x, int y, int z) { return noise.GetUniform3DNoise(x, y, z); }
			),
			"GetSignedUniform3DNoise", sol::overload(
				[](SquirrelNoise& noise, int x, int y, int z, unsigned int seedOverride) { return noise.GetSignedUniform3DNoise(x, y, z, seedOverride); },
				[](SquirrelNoise& noise, int x, int y, int z) { return noise.GetSignedUniform3DNoise(x, y, z); }
			)
			);
	}
	
	void ScriptingSystem::SetupLuaComponents()
	{
		m_pLuaState->new_usertype<BoxColliderComponent>("BoxColliderComponent",
			"offset", &BoxColliderComponent::m_offset,
			"size", &BoxColliderComponent::m_size,
			"density", &BoxColliderComponent::m_density,
			"friction", &BoxColliderComponent::m_friction,
			"restitution", &BoxColliderComponent::m_restitution,
			"restitutionThreshold", &BoxColliderComponent::m_restitutionThreshold
			);

		m_pLuaState->new_usertype<CameraComponent>("CameraComponent",
			"isActive", &CameraComponent::m_isActive,
			"isFixedAspectRatio", &CameraComponent::m_isFixedAspectRatio,
			"viewportRect", &CameraComponent::m_viewportRect
			);

		m_pLuaState->new_usertype<CircleColliderComponent>("CircleColliderComponent",
			"offset", &CircleColliderComponent::m_offset,
			"radius", &CircleColliderComponent::m_radius,
			"density", &CircleColliderComponent::m_density,
			"friction", &CircleColliderComponent::m_friction,
			"restitution", &CircleColliderComponent::m_restitution,
			"restitutionThreshold", &CircleColliderComponent::m_restitutionThreshold
			);

		m_pLuaState->new_usertype<CircleRendererComponent>("CircleRendererComponent",
			"color", &CircleRendererComponent::m_color,
			"fade", &CircleRendererComponent::m_fade,
			"thickness", &CircleRendererComponent::m_thickness
			);

		m_pLuaState->new_usertype<LuaScriptComponent>("LuaScriptComponent",
			"script", &LuaScriptComponent::m_scriptData
			);

		m_pLuaState->new_usertype<RigidbodyComponent>("RigidbodyComponent",
			"bodyType", &RigidbodyComponent::m_bodyType,
			"gravityScale", &RigidbodyComponent::m_gravityScale,
			"isFixedRotation", &RigidbodyComponent::m_isFixedRotation,
			"SetPosition", [](RigidbodyComponent& component, const glm::vec2& position)
			{
				if (component.m_pRuntimeBody == nullptr)
				{
					EXE_LOG_CATEGORY_ERROR("Lua", "Invalid Rigidbody!");
					return;
				}

				b2Body* body = (b2Body*)component.m_pRuntimeBody;
				body->SetTransform(b2Vec2(position.x, position.y), body->GetAngle());
			});

		m_pLuaState->new_usertype<SpriteRendererComponent>("SpriteRendererComponent",
			"color", &SpriteRendererComponent::m_color,
			"textureResource", &SpriteRendererComponent::m_textureResource,
			"textureTilingMultiplier", &SpriteRendererComponent::m_textureTilingMultiplier
			);

		m_pLuaState->new_usertype<TransformComponent>("TransformComponent",
			"translation", &TransformComponent::m_translation,
			"rotation", &TransformComponent::m_rotation,
			"scale", &TransformComponent::m_scale
			);
	}

	static void DestroyScriptObject(GameObject& gameObject, Scene* pOwningScene)
	{
		EXE_ASSERT(pOwningScene);
		if (!gameObject.HasComponent<LuaScriptComponent>())
		{
			pOwningScene->DestroyGameObject(gameObject);
			return; // Bail if we don't have a script.
		}

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

		pOwningScene->DestroyGameObject(gameObject);
	}

	void ScriptingSystem::SetupLuaGameObject(Scene* pOwningScene)
	{
		m_pLuaState->new_usertype<GameObject>("GameObject",
			"Instance", [pOwningScene](GameObject& thisGameObject, sol::this_state state)
			{
				sol::state_view luaState(state);
				eastl::string instanceName = thisGameObject.GetName() + " (Instance)";

				GameObject instance = pOwningScene->DuplicateGameObject(thisGameObject, instanceName);
				if (instance)
					return sol::make_object(luaState, instance);
				return sol::make_object(luaState, sol::nil);
			},
			"Destroy", [pOwningScene](GameObject& thisGameObject) { DestroyScriptObject(thisGameObject, pOwningScene); },
			"GetName", [pOwningScene](GameObject& thisGameObject) { return thisGameObject.GetName().c_str(); },
			"GetGUID", [pOwningScene](GameObject& thisGameObject) { return thisGameObject.GetGUID(); },

			ADD_ADDCOMPONENT(BoxColliderComponent),
			"AddBoxColliderComponent", [](GameObject& thisGameObject)
			{
				if (!thisGameObject.HasComponent<RigidbodyComponent>())
					thisGameObject.AddComponent<RigidbodyComponent>();
				return thisGameObject.AddComponent<BoxColliderComponent>();
			},
			ADD_ADDCOMPONENT(CameraComponent),
				"AddCircleColliderComponent", [](GameObject& thisGameObject)
			{
				if (!thisGameObject.HasComponent<RigidbodyComponent>())
					thisGameObject.AddComponent<RigidbodyComponent>();
				return thisGameObject.AddComponent<CircleColliderComponent>();
			},
			ADD_ADDCOMPONENT(CircleRendererComponent),
			ADD_ADDCOMPONENT(GUIDComponent),
			ADD_ADDCOMPONENT(LuaScriptComponent),
			ADD_ADDCOMPONENT(NameComponent),
			ADD_ADDCOMPONENT(RigidbodyComponent),
			ADD_ADDCOMPONENT(SpriteRendererComponent),
			ADD_ADDCOMPONENT(TransformComponent),

			ADD_REMOVECOMPONENT(BoxColliderComponent),
			ADD_REMOVECOMPONENT(CameraComponent),
			ADD_REMOVECOMPONENT(CircleColliderComponent),
			ADD_REMOVECOMPONENT(CircleRendererComponent),
			ADD_REMOVECOMPONENT(GUIDComponent),
			ADD_REMOVECOMPONENT(LuaScriptComponent),
			ADD_REMOVECOMPONENT(NameComponent),
			ADD_REMOVECOMPONENT(RigidbodyComponent),
			ADD_REMOVECOMPONENT(SpriteRendererComponent),
			ADD_REMOVECOMPONENT(TransformComponent),

			ADD_GETCOMPONENT(BoxColliderComponent),
			ADD_GETCOMPONENT(CameraComponent),
			ADD_GETCOMPONENT(CircleColliderComponent),
			ADD_GETCOMPONENT(CircleRendererComponent),
			ADD_GETCOMPONENT(GUIDComponent),
			"GetLuaScriptComponent", [](GameObject& gameObject, sol::this_state state)
			{
				sol::state_view luaState(state);
				if (gameObject.HasComponent<LuaScriptComponent>())
					return sol::make_object(luaState, gameObject.GetComponent<LuaScriptComponent>().m_scriptData);
				else
					return sol::make_object(luaState, sol::nil);
			},
			ADD_GETCOMPONENT(NameComponent),
			ADD_GETCOMPONENT(RigidbodyComponent),
			ADD_GETCOMPONENT(SpriteRendererComponent),
			ADD_GETCOMPONENT(TransformComponent)
			);

		m_pLuaState->set_function("CreateGameObject", [pOwningScene](const char* name)
			{
				return pOwningScene->CreateGameObject(name);
			});
		m_pLuaState->set_function("InstantiateGameObject", [pOwningScene](const char* name, sol::this_state state)
			{
				sol::state_view luaState(state);
				GameObject instance = pOwningScene->DeserializeGameObject(name);
				if (instance)
					return sol::make_object(luaState, instance);
				return sol::make_object(luaState, sol::nil);
			});
		m_pLuaState->set_function("DestroyGameObject", [pOwningScene](GameObject& gameObject) { DestroyScriptObject(gameObject, pOwningScene); });
	}
	
	void ScriptingSystem::SetupLuaPhysics(Scene* pOwningScene)
	{
		m_pLuaState->new_usertype<CollisionData>("CollisionData",
			"thisGameObject", &CollisionData::m_thisGameObject,
			"otherGameObject", &CollisionData::m_otherGameObject,
			"isCollisionEnter", &CollisionData::m_isCollisionEnter
			);

		auto collisionFunction = [pOwningScene](CollisionData collisionData)
		{
			EXE_ASSERT(pOwningScene);
			if (!collisionData.m_thisGameObject.HasComponent<LuaScriptComponent>())
				return; // Bail if we don't have a script.

			LuaScriptComponent& luaScript = collisionData.m_thisGameObject.GetComponent<LuaScriptComponent>();

			if (collisionData.m_isCollisionEnter)
			{
				sol::function fn = luaScript.m_scriptData["OnCollisionEnter"];

				if (fn)
				{
					sol::protected_function_result result = fn(luaScript.m_scriptData, collisionData);
					if (!result.valid())
					{
						sol::error err = result;
						EXE_LOG_CATEGORY_ERROR("Lua", err.what());
					}
				}
			}
			else
			{
				sol::function fn = luaScript.m_scriptData["OnCollisionExit"];

				if (fn)
				{
					sol::protected_function_result result = fn(luaScript.m_scriptData, collisionData);
					if (!result.valid())
					{
						sol::error err = result;
						EXE_LOG_CATEGORY_ERROR("Lua", err.what());
					}
				}
			}
		};

		pOwningScene->GetPhysicsSystem().AddCollisionCallback(collisionFunction);
	}
	
	void ScriptingSystem::InitializeScriptComponents(Scene* pOwningScene)
	{
		auto view = pOwningScene->GetAllGameObjectsWith<LuaScriptComponent>();
		for (auto objectWithScript : view)
		{
			GameObject gameObject(objectWithScript, pOwningScene);
			LuaScriptComponent& luaScript = gameObject.GetComponent<LuaScriptComponent>();

			luaScript.InitializeScript(m_pLuaState, gameObject);

			if (!luaScript.m_didInitialize)
				continue;

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
}