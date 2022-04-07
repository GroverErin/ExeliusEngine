#pragma once
#include "source/utility/generic/GUID.h"
#include "source/resource/ResourceHandle.h"

#include <EASTL/string.h>
#include <EASTL/unordered_map.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

class b2World;

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class GameObject;
	class EditorCamera;

	class Scene
	{
		friend class GameObject;
		
		ResourceHandle m_sceneResource;

		// Physics members.
		b2World* m_pPhysicsWorld;
		glm::vec2 m_globalGravity;
		int32_t m_velocityIterations;
		int32_t m_positionIterations;

		// ENTT
		entt::registry m_registry;

		// TODO: Remove these, as they belong to Cameras
		uint32_t m_viewportWidth;
		uint32_t m_viewportHeight;

	public:
		Scene();
		~Scene();

		static SharedPtr<Scene> Copy(SharedPtr<Scene> other);

		GameObject CreateGameObject(const eastl::string& name = eastl::string());
		GameObject CreateGameObjectWithGUID(GUID id, const eastl::string& name = eastl::string());
		
		void DuplicateGameObject(GameObject gameObject);

		void DestroyGameObject(GameObject gameObject);

		void OnRuntimeStart();
		void OnRuntimeUpdate();
		void OnRuntimeStop();

		void OnUpdateEditor(EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		eastl::vector<GameObject> GetAllGameObjects();

		template<typename... Components>
		auto GetAllGameObjectsWith()
		{
			return m_registry.view<Components...>();
		}

		uint32_t GetViewportWidth() const { return m_viewportWidth; }
		uint32_t GetViewportHeight() const { return m_viewportHeight; }

		bool SerializeScene(const eastl::string& filepath = eastl::string());

		// Here, we will not LOAD a resource, it is expected to be loaded already.
		bool DeserializeScene(const ResourceID& sceneResourceID);

	private:

		void InitializeRuntimePhysics();

		void InitializeRuntimeScripts();

		void UpdateRuntimePhysics();

		void UpdateRuntimeScripts();

		void RenderSceneForActiveCameras();

		void SubmitSprites();

		void SubmitCircles();

		eastl::string InternalSerializeScene();
	};
}