#include "EXEPCH.h"
#include "Scene.h"

#include "source/engine/physics/PhysicsSystem.h"
#include "source/engine/scripting/ScriptingSystem.h"
#include "source/engine/gameobjects/GameObject.h"
#include "source/engine/renderer/Renderer2D.h"

#include "source/engine/gameobjects/components/RigidbodyComponent.h"
#include "source/engine/gameobjects/components/BoxColliderComponent.h"
#include "source/engine/gameobjects/components/TransformComponent.h"
#include "source/engine/gameobjects/components/SpriteRendererComponent.h"
#include "source/engine/gameobjects/components/NameComponent.h"
#include "source/engine/gameobjects/components/GUIDComponent.h"
#include "source/engine/gameobjects/components/CameraComponent.h"
#include "source/engine/gameobjects/components/CircleColliderComponent.h"
#include "source/engine/gameobjects/components/CircleRendererComponent.h"
#include "source/engine/gameobjects/components/LuaScriptComponent.h"

#include "source/engine/resources/resourcetypes/TextFileResource.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

//extern "C"
//{
//	#include <lua.h>
//	#include <lualib.h>
//	#include <lauxlib.h>
//	#include <luaconf.h>
//}

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	template<typename Component>
	static void CopyComponent(entt::registry& destinationRegistry, entt::registry& sourceRegistry, const eastl::unordered_map<GUID, entt::entity>& enttMap)
	{
		auto view = sourceRegistry.view<Component>();
		for (auto gameObject : view)
		{
			GUID id = sourceRegistry.get<GUIDComponent>(gameObject).m_GUID;
			EXE_ASSERT(enttMap.find(id) != enttMap.end());
			entt::entity dstEnttID = enttMap.at(id);

			Component& component = sourceRegistry.get<Component>(gameObject);
			destinationRegistry.emplace_or_replace<Component>(dstEnttID, component);
		}
	}

	static void CopyAllComponents(entt::registry& destinationRegistry, entt::registry& sourceRegistry, const eastl::unordered_map<GUID, entt::entity>& enttMap)
	{
		// Copy components, except Name and GUID components, as those are done already.
		// This sucks, as it blocks the ability for clients to define custom components.
		// I do not think there is currently a way to do this well with ENTT. :(
		// Due to this, I am looking at going the Unity route and making custom components
		// just be Lua Scripts.
		CopyComponent<TransformComponent>(destinationRegistry, sourceRegistry, enttMap);
		CopyComponent<SpriteRendererComponent>(destinationRegistry, sourceRegistry, enttMap);
		CopyComponent<CircleRendererComponent>(destinationRegistry, sourceRegistry, enttMap);
		CopyComponent<CameraComponent>(destinationRegistry, sourceRegistry, enttMap);
		CopyComponent<RigidbodyComponent>(destinationRegistry, sourceRegistry, enttMap);
		CopyComponent<BoxColliderComponent>(destinationRegistry, sourceRegistry, enttMap);
		CopyComponent<CircleColliderComponent>(destinationRegistry, sourceRegistry, enttMap);
		CopyComponent<LuaScriptComponent>(destinationRegistry, sourceRegistry, enttMap);
	}

	template<typename Component>
	static void CopyExistingComponent(GameObject destinationGameObject, GameObject sourceGameObject)
	{
		if (sourceGameObject.HasComponent<Component>())
			destinationGameObject.AddOrReplaceComponent<Component>(sourceGameObject.GetComponent<Component>());
	}

	static void CopyExistingComponents(GameObject destinationGameObject, GameObject sourceGameObject)
	{
		// Once again, this sucks...
		CopyExistingComponent<TransformComponent>(destinationGameObject, sourceGameObject);
		CopyExistingComponent<SpriteRendererComponent>(destinationGameObject, sourceGameObject);
		CopyExistingComponent<CircleRendererComponent>(destinationGameObject, sourceGameObject);
		CopyExistingComponent<CameraComponent>(destinationGameObject, sourceGameObject);
		CopyExistingComponent<RigidbodyComponent>(destinationGameObject, sourceGameObject);
		CopyExistingComponent<BoxColliderComponent>(destinationGameObject, sourceGameObject);
		CopyExistingComponent<CircleColliderComponent>(destinationGameObject, sourceGameObject);
		CopyExistingComponent<LuaScriptComponent>(destinationGameObject, sourceGameObject);
	}

	Scene::Scene()
		: m_pPhysicsSystem(nullptr)
		, m_pScriptingSystem(nullptr)
		, m_viewportWidth(0)
		, m_viewportHeight(0)
	{
		m_pPhysicsSystem = EXELIUS_NEW(PhysicsSystem(this));
		m_pScriptingSystem = EXELIUS_NEW(ScriptingSystem());
	}

	Scene::~Scene()
	{
		OnRuntimeStop();
		if (m_sceneResource.IsReferenceHeld())
			m_sceneResource.Release();

		EXELIUS_DELETE(m_pPhysicsSystem);
		EXELIUS_DELETE(m_pScriptingSystem);
	}

	SharedPtr<Scene> Scene::Copy(SharedPtr<Scene> other)
	{
		SharedPtr<Scene> newScene = MakeShared<Scene>();

		newScene->m_viewportWidth = other->m_viewportWidth;
		newScene->m_viewportHeight = other->m_viewportHeight;

		auto& srcSceneRegistry = other->m_registry;
		auto& dstSceneRegistry = newScene->m_registry;
		eastl::unordered_map<GUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<GUIDComponent>();
		for (auto e : idView)
		{
			GUID id = srcSceneRegistry.get<GUIDComponent>(e).m_GUID;
			const eastl::string& name = srcSceneRegistry.get<NameComponent>(e).m_name;
			GameObject newGameObject = newScene->CreateGameObjectWithGUID(id, name);
			enttMap[id] = (entt::entity)newGameObject;
		}

		CopyAllComponents(dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	GameObject Scene::CreateGameObject(const eastl::string& name)
	{
		return CreateGameObjectWithGUID(GUID(), name);
	}

	GameObject Scene::CreateGameObjectWithGUID(GUID id, const eastl::string& name)
	{
		GameObject gameObject(m_registry.create(), this);
		gameObject.AddComponent<GUIDComponent>(id);
		gameObject.AddComponent<TransformComponent>();
		NameComponent& nameComponent = gameObject.AddComponent<NameComponent>();

		if (name.empty())
			nameComponent.m_name = "GameObject";
		else
			nameComponent.m_name = name;

		return gameObject;
	}

	GameObject Scene::DuplicateGameObject(GameObject gameObject, const eastl::string& name)
	{
		EXE_ASSERT(m_pPhysicsSystem);
		EXE_ASSERT(m_pScriptingSystem);

		eastl::string newName = name;
		if (newName.empty())
			newName = gameObject.GetName();
		GameObject newGameObject = CreateGameObject(newName);

		CopyExistingComponents(newGameObject, gameObject);

		m_pPhysicsSystem->TryAddRuntimeBody(newGameObject);
		m_pPhysicsSystem->TryAddRuntimeBoxCollider(newGameObject);
		m_pPhysicsSystem->TryAddRuntimeCircleCollider(newGameObject);
		m_pScriptingSystem->TryAddRuntimeScript(newGameObject);

		return newGameObject;
	}

	void Scene::DestroyGameObject(GameObject gameObject)
	{
		m_registry.destroy(gameObject);
	}

	void Scene::OnRuntimeStart()
	{
		EXE_ASSERT(m_pPhysicsSystem);
		EXE_ASSERT(m_pScriptingSystem);

		m_pPhysicsSystem->InitializeRuntimePhysics();
		m_pScriptingSystem->InitializeRuntimeScripting(this);
	}

	void Scene::OnRuntimeUpdate()
	{
		EXE_ASSERT(m_pPhysicsSystem);
		EXE_ASSERT(m_pScriptingSystem);

		m_pScriptingSystem->UpdateRuntimeScripting(this);
		m_pPhysicsSystem->UpdateRuntimePhysics();

		RenderSceneForActiveCameras();
	}

	void Scene::OnRuntimeStop()
	{
		EXE_ASSERT(m_pPhysicsSystem);
		EXE_ASSERT(m_pScriptingSystem);

		m_pScriptingSystem->StopRuntimeScripting(this);
		m_pPhysicsSystem->StopRuntimePhysics();
	}

	void Scene::OnUpdateEditor(EditorCamera& camera)
	{
		Renderer2D::GetInstance()->Begin2DScene(camera);
		{
			SubmitSprites();
			SubmitCircles();
		}
		Renderer2D::GetInstance()->End2DScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_registry.view<CameraComponent>();
		for (auto gameObject : view)
		{
			CameraComponent& cameraComponent = view.get<CameraComponent>(gameObject);
			if (!cameraComponent.m_isFixedAspectRatio)
				cameraComponent.m_camera.SetViewportSize(width, height);
		}
	}

	eastl::vector<GameObject> Scene::GetAllGameObjects()
	{
		eastl::vector<GameObject> gameObjects;
		m_registry.each([&](auto gameObjectID)
			{
				gameObjects.emplace_back(GameObject{ gameObjectID , this });
			});
		return gameObjects;
	}

	bool Scene::SerializeScene(const eastl::string& filepath)
	{
		if (filepath.empty())
		{
			if (!m_sceneResource.IsReferenceHeld())
			{
				// Cannot save scene if there is no available file or resource.
				EXE_LOG_CATEGORY_ERROR("SceneSerialization", "Failed to Serialize Scene. No possible filepath or ResourceID provided.");
				return false;
			}

			EXE_LOG_CATEGORY_INFO("SceneSerialization", "Serializing Scene Resource '{}'.", m_sceneResource.GetID().Get().c_str());

			// Tell the resourcemanager to save this resource.
			m_sceneResource.GetAs<TextFileResource>()->SetRawText(InternalSerializeScene());
			m_sceneResource.SaveResource();
			return true;
		}

		EXE_LOG_CATEGORY_INFO("SceneSerialization", "Attempting to Serialize Scene to '{}'", filepath.c_str());

		m_sceneResource.Release();
		if (!m_sceneResource.CreateNew(filepath))
			return false;

		m_sceneResource.GetAs<TextFileResource>()->SetRawText(InternalSerializeScene());
		m_sceneResource.SaveResource();
		return true;
	}

	bool Scene::DeserializeScene(const ResourceID& sceneResourceID)
	{
		EXE_LOG_CATEGORY_INFO("SceneDeserialization", "Attempting to deserialize Scene: '{}'", sceneResourceID.Get().c_str());

		// We are attempting to deserialize a scene into this scene, which
		// already has deserialized a scene previously. This is not the
		// expected use of this class, and another scene should be constructed
		// instead. In this case we will just bail.
		if (m_sceneResource.IsReferenceHeld())
		{
			EXE_LOG_CATEGORY_WARN("SceneDeserialization", "Failed to deserialize Scene. This Scene object already holds a Scene. Construct a new Scene.");
			return false;
		}

		if (!sceneResourceID.IsValid())
		{
			EXE_LOG_CATEGORY_ERROR("SceneDeserialization", "Failed to deserialize Scene. ResourceID was invalid.");
			return false;
		}
		
		m_sceneResource.SetResourceID(sceneResourceID);

		// GetAs will aquire the resource, incrementing the reference count.
		// This reference count will only decrement now in the case that THIS
		// scene is destroyed.
		TextFileResource* pTextFileResource = m_sceneResource.GetAs<TextFileResource>();

		// We expect a preloaded resource. Bail otherwise.
		if (!pTextFileResource)
		{
			EXE_LOG_CATEGORY_WARN("SceneDeserialization", "Failed to deserialize Scene. Scene Resource was not loaded.");
			return false;
		}

		eastl::string rawText = pTextFileResource->GetRawText();

		// Parse the text as JSON data.
		rapidjson::Document jsonDoc;
		if (jsonDoc.Parse(rawText.c_str()).HasParseError())
		{
			EXE_LOG_CATEGORY_ERROR("SceneDeserialization", "Failed to Parse JSON text: rapidjson error '{0}'", jsonDoc.GetParseError());
			return false;
		}

		auto sceneMember = jsonDoc.FindMember("Scene");

		if (sceneMember == jsonDoc.MemberEnd())
		{
			EXE_LOG_CATEGORY_ERROR("SceneDeserialization", "No 'Scene' root node found.");
			return false;
		}

		auto gameObjectsMember = sceneMember->value.FindMember("GameObjects");
		if (gameObjectsMember == jsonDoc.MemberEnd())
		{
			EXE_LOG_CATEGORY_ERROR("SceneDeserialization", "No 'GameObjects' array found.");
			return false;
		}

		EXE_ASSERT(gameObjectsMember->value.IsArray());

		for (auto& gameObject : gameObjectsMember->value.GetArray())
		{
			EXE_ASSERT(gameObject.IsObject());

			const auto guidComponentMember = gameObject.FindMember("GUIDComponent");
			if (guidComponentMember == gameObject.MemberEnd())
			{
				EXE_LOG_CATEGORY_ERROR("SceneDeserialization", "GameObject found with no GUID.");
				return false;
			}
			EXE_ASSERT(guidComponentMember->value.IsNumber());
			GUID objectGUID = guidComponentMember->value.GetUint64();

			const auto nameComponentMember = gameObject.FindMember("NameComponent");
			if (nameComponentMember == gameObject.MemberEnd())
			{
				EXE_LOG_CATEGORY_ERROR("SceneDeserialization", "GameObject found with no Name.");
				return false;
			}
			EXE_ASSERT(nameComponentMember->value.IsString());
			eastl::string objectName = nameComponentMember->value.GetString();

			GameObject loadedObject = CreateGameObjectWithGUID(objectGUID, objectName);

			{
				const auto transformComponentMember = gameObject.FindMember("TransformComponent");
				if (transformComponentMember == gameObject.MemberEnd())
				{
					EXE_LOG_CATEGORY_ERROR("SceneDeserialization", "GameObject found with no Transform.");
					return false;
				}
				EXE_ASSERT(transformComponentMember->value.IsArray());


				TransformComponent& transformComponent = loadedObject.GetComponent<TransformComponent>();
				transformComponent.DeserializeComponent(transformComponentMember->value);
			}

			const auto cameraComponentMember = gameObject.FindMember("CameraComponent");
			if (cameraComponentMember != gameObject.MemberEnd())
			{
				CameraComponent& cameraComponent = loadedObject.AddComponent<CameraComponent>();
				cameraComponent.DeserializeComponent(cameraComponentMember->value);
			}

			const auto spriteComponentMember = gameObject.FindMember("SpriteRendererComponent");
			if (spriteComponentMember != gameObject.MemberEnd())
			{
				SpriteRendererComponent& spriteComponent = loadedObject.AddComponent<SpriteRendererComponent>();
				spriteComponent.DeserializeComponent(spriteComponentMember->value);
			}

			const auto circleRendererComponentMember = gameObject.FindMember("CircleRendererComponent");
			if (circleRendererComponentMember != gameObject.MemberEnd())
			{
				CircleRendererComponent& circleRendererComponent = loadedObject.AddComponent<CircleRendererComponent>();
				circleRendererComponent.DeserializeComponent(circleRendererComponentMember->value);
			}

			const auto rigidbodyComponentMember = gameObject.FindMember("RigidbodyComponent");
			if (rigidbodyComponentMember != gameObject.MemberEnd())
			{
				RigidbodyComponent& rigidbodyComponent = loadedObject.AddComponent<RigidbodyComponent>();
				rigidbodyComponent.DeserializeComponent(rigidbodyComponentMember->value);
			}

			const auto boxColliderComponentMember = gameObject.FindMember("BoxColliderComponent");
			if (boxColliderComponentMember != gameObject.MemberEnd())
			{
				BoxColliderComponent& boxCollider = loadedObject.AddComponent<BoxColliderComponent>();
				boxCollider.DeserializeComponent(boxColliderComponentMember->value);
			}

			const auto circleColliderComponentMember = gameObject.FindMember("CircleColliderComponent");
			if (circleColliderComponentMember != gameObject.MemberEnd())
			{
				CircleColliderComponent& circleCollider = loadedObject.AddComponent<CircleColliderComponent>();
				circleCollider.DeserializeComponent(circleColliderComponentMember->value);
			}

			const auto luaScriptComponentMember = gameObject.FindMember("LuaScriptComponent");
			if (luaScriptComponentMember != gameObject.MemberEnd())
			{
				LuaScriptComponent& luaScript = loadedObject.AddComponent<LuaScriptComponent>();
				luaScript.DeserializeComponent(luaScriptComponentMember->value);
			}
		}

		EXE_LOG_CATEGORY_INFO("SceneDeserialization", "Scene '{}' Deserialized Successfully!", sceneResourceID.Get().c_str());
		return true;
	}

	void Scene::RenderSceneForActiveCameras()
	{
		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto gameObjectWithCamera : view)
		{
			auto [cameraTransform, cameraComponent] = view.get<TransformComponent, CameraComponent>(gameObjectWithCamera);

			if (!cameraComponent.m_isActive)
				continue;

			Renderer2D::GetInstance()->Begin2DScene(cameraComponent.m_camera, cameraTransform.GetTransform());
			{
				SubmitSprites();
				SubmitCircles();
			}
			Renderer2D::GetInstance()->End2DScene();
		}
	}

	void Scene::SubmitSprites()
	{
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto gameObject : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(gameObject);

			Renderer2D::GetInstance()->DrawSprite(transform.GetTransform(), sprite, (int)gameObject);
		}
	}

	void Scene::SubmitCircles()
	{
		auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
		for (auto gameObject : view)
		{
			auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(gameObject);

			Renderer2D::GetInstance()->DrawCircle(transform.GetTransform(), circle.m_color, circle.m_thickness, circle.m_fade, (int)gameObject);
		}
	}

	static void InternalSerializeGameObject(rapidjson::Writer<rapidjson::StringBuffer>& writer, GameObject gameObject)
	{
		writer.StartObject(); // Start New GameObject.
		{
			if (gameObject.HasComponent<GUIDComponent>())
			{
				writer.Key("GUIDComponent");
				writer.Uint64(gameObject.GetGUID());
			}

			if (gameObject.HasComponent<NameComponent>())
			{
				writer.Key("NameComponent");
				writer.String(gameObject.GetName().c_str(), (rapidjson::SizeType)(gameObject.GetName().size()));
			}

			if (gameObject.HasComponent<TransformComponent>())
			{
				TransformComponent& transformComponent = gameObject.GetComponent<TransformComponent>();
				transformComponent.SerializeComponent(writer);
			}

			if (gameObject.HasComponent<CameraComponent>())
			{
				CameraComponent& cameraComponent = gameObject.GetComponent<CameraComponent>();
				cameraComponent.SerializeComponent(writer);
			}

			if (gameObject.HasComponent<SpriteRendererComponent>())
			{
				SpriteRendererComponent& spriteComponent = gameObject.GetComponent<SpriteRendererComponent>();
				spriteComponent.SerializeComponent(writer);
			}

			if (gameObject.HasComponent<CircleRendererComponent>())
			{
				CircleRendererComponent& circleRendererComponent = gameObject.GetComponent<CircleRendererComponent>();
				circleRendererComponent.SerializeComponent(writer);
			}

			if (gameObject.HasComponent<RigidbodyComponent>())
			{
				RigidbodyComponent& rigidbodyComponent = gameObject.GetComponent<RigidbodyComponent>();
				rigidbodyComponent.SerializeComponent(writer);
			}

			if (gameObject.HasComponent<BoxColliderComponent>())
			{
				BoxColliderComponent& boxColliderComponent = gameObject.GetComponent<BoxColliderComponent>();
				boxColliderComponent.SerializeComponent(writer);
			}

			if (gameObject.HasComponent<CircleColliderComponent>())
			{
				CircleColliderComponent& circleColliderComponent = gameObject.GetComponent<CircleColliderComponent>();
				circleColliderComponent.SerializeComponent(writer);
			}

			if (gameObject.HasComponent<LuaScriptComponent>())
			{
				LuaScriptComponent& luaScriptComponent = gameObject.GetComponent<LuaScriptComponent>();
				luaScriptComponent.SerializeComponent(writer);
			}
		}
		writer.EndObject(); // End New GameObject.
	}

	eastl::string Scene::InternalSerializeScene()
	{
		rapidjson::StringBuffer buffer;

		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		writer.StartObject(); // Start JSON Object.
		writer.Key("Scene");
		writer.StartObject(); // Start "Scene" Object.
		writer.Key("GameObjects");
		writer.StartArray(); // Start "GameObjects" Array.

		m_registry.each([&](auto gameObjectGUID)
			{
				GameObject gameObject = { gameObjectGUID, this };
				if (!gameObject)
					return;
				InternalSerializeGameObject(writer, gameObject);
			});

		writer.EndArray(); // End "GameObjects" Array.
		writer.EndObject(); // End "Scene" Object.
		writer.EndObject(); // End JSON Object.

		return buffer.GetString();
	}

	eastl::string Scene::SerializeGameObject(GameObject gameObject)
	{
		rapidjson::StringBuffer buffer;

		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
		writer.StartObject(); // Start JSON Object.
		writer.Key("Prefab");
		InternalSerializeGameObject(writer, gameObject);
		writer.EndObject(); // End JSON Object.

		return buffer.GetString();
	}

	GameObject Scene::DeserializeGameObject(const ResourceID& prefabResourceID)
	{
		EXE_ASSERT(m_pPhysicsSystem);
		EXE_ASSERT(m_pScriptingSystem);

		EXE_LOG_CATEGORY_INFO("PrefabDeserialization", "Attempting to deserialize Prefab: '{}'", prefabResourceID.Get().c_str());

		if (!prefabResourceID.IsValid())
		{
			EXE_LOG_CATEGORY_ERROR("PrefabDeserialization", "Failed to deserialize Prefab. ResourceID was invalid.");
			return {};
		}

		// Resource will be temporarily acquired.
		ResourceHandle prefabResource(prefabResourceID);

		if (!prefabResource.IsReferenceHeld())
		{
			EXE_LOG_CATEGORY_ERROR("PrefabDeserialization", "Failed to deserialize Prefab. Not Loaded.");
			return {};
		}

		TextFileResource* pTextFileResource = prefabResource.GetAs<TextFileResource>();

		// We expect a preloaded resource. Bail otherwise.
		if (!pTextFileResource)
		{
			EXE_LOG_CATEGORY_ERROR("PrefabDeserialization", "Failed to deserialize Prefab. Prefab Resource was not loaded.");
			return {};
		}

		eastl::string rawText = pTextFileResource->GetRawText();

		// Parse the text as JSON data.
		rapidjson::Document jsonDoc;
		if (jsonDoc.Parse(rawText.c_str()).HasParseError())
		{
			EXE_LOG_CATEGORY_ERROR("PrefabDeserialization", "Failed to Parse JSON text: rapidjson error '{0}'", jsonDoc.GetParseError());
			return {};
		}

		auto prefabMember = jsonDoc.FindMember("Prefab");

		if (prefabMember == jsonDoc.MemberEnd())
		{
			EXE_LOG_CATEGORY_ERROR("PrefabDeserialization", "No 'Prefab' root node found.");
			return {};
		}

		EXE_ASSERT(prefabMember->value.IsObject());

		const auto guidComponentMember = prefabMember->value.FindMember("GUIDComponent");
		if (guidComponentMember == prefabMember->value.MemberEnd())
		{
			EXE_LOG_CATEGORY_ERROR("PrefabDeserialization", "GameObject found with no GUID.");
			return {};
		}
		EXE_ASSERT(guidComponentMember->value.IsNumber());
		GUID objectGUID = guidComponentMember->value.GetUint64();

		const auto nameComponentMember = prefabMember->value.FindMember("NameComponent");
		if (nameComponentMember == prefabMember->value.MemberEnd())
		{
			EXE_LOG_CATEGORY_ERROR("PrefabDeserialization", "GameObject found with no Name.");
			return {};
		}
		EXE_ASSERT(nameComponentMember->value.IsString());
		eastl::string objectName = nameComponentMember->value.GetString();

		GameObject loadedObject = CreateGameObjectWithGUID(objectGUID, objectName);

		{
			const auto transformComponentMember = prefabMember->value.FindMember("TransformComponent");
			if (transformComponentMember == prefabMember->value.MemberEnd())
			{
				EXE_LOG_CATEGORY_ERROR("PrefabDeserialization", "GameObject found with no Transform.");
				return {};
			}
			EXE_ASSERT(transformComponentMember->value.IsArray());

			TransformComponent& transformComponent = loadedObject.GetComponent<TransformComponent>();
			transformComponent.DeserializeComponent(transformComponentMember->value);
		}

		const auto cameraComponentMember = prefabMember->value.FindMember("CameraComponent");
		if (cameraComponentMember != prefabMember->value.MemberEnd())
		{
			CameraComponent& cameraComponent = loadedObject.AddComponent<CameraComponent>();
			cameraComponent.DeserializeComponent(cameraComponentMember->value);
		}

		const auto spriteComponentMember = prefabMember->value.FindMember("SpriteRendererComponent");
		if (spriteComponentMember != prefabMember->value.MemberEnd())
		{
			SpriteRendererComponent& spriteComponent = loadedObject.AddComponent<SpriteRendererComponent>();
			spriteComponent.DeserializeComponent(spriteComponentMember->value);
		}

		const auto circleRendererComponentMember = prefabMember->value.FindMember("CircleRendererComponent");
		if (circleRendererComponentMember != prefabMember->value.MemberEnd())
		{
			CircleRendererComponent& circleRendererComponent = loadedObject.AddComponent<CircleRendererComponent>();
			circleRendererComponent.DeserializeComponent(circleRendererComponentMember->value);
		}

		const auto rigidbodyComponentMember = prefabMember->value.FindMember("RigidbodyComponent");
		if (rigidbodyComponentMember != prefabMember->value.MemberEnd())
		{
			RigidbodyComponent& rigidbodyComponent = loadedObject.AddComponent<RigidbodyComponent>();
			rigidbodyComponent.DeserializeComponent(rigidbodyComponentMember->value);
			m_pPhysicsSystem->TryAddRuntimeBody(loadedObject);
		}

		const auto boxColliderComponentMember = prefabMember->value.FindMember("BoxColliderComponent");
		if (boxColliderComponentMember != prefabMember->value.MemberEnd())
		{
			BoxColliderComponent& boxCollider = loadedObject.AddComponent<BoxColliderComponent>();
			boxCollider.DeserializeComponent(boxColliderComponentMember->value);
			m_pPhysicsSystem->TryAddRuntimeBoxCollider(loadedObject);
		}

		const auto circleColliderComponentMember = prefabMember->value.FindMember("CircleColliderComponent");
		if (circleColliderComponentMember != prefabMember->value.MemberEnd())
		{
			CircleColliderComponent& circleCollider = loadedObject.AddComponent<CircleColliderComponent>();
			circleCollider.DeserializeComponent(circleColliderComponentMember->value);
			m_pPhysicsSystem->TryAddRuntimeCircleCollider(loadedObject);
		}

		const auto luaScriptComponentMember = prefabMember->value.FindMember("LuaScriptComponent");
		if (luaScriptComponentMember != prefabMember->value.MemberEnd())
		{
			LuaScriptComponent& luaScript = loadedObject.AddComponent<LuaScriptComponent>();
			luaScript.DeserializeComponent(luaScriptComponentMember->value);
			m_pScriptingSystem->TryAddRuntimeScript(loadedObject);
		}

		EXE_LOG_CATEGORY_INFO("PrefabDeserialization", "Prefab '{}' Deserialized Successfully!", prefabResourceID.Get().c_str());
		return loadedObject;
	}
}