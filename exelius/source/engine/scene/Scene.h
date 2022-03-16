#pragma once

#include <EASTL/string.h>
#include <entt/entt.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Entity;
	class EditorCamera;

	class Scene
	{
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0;
		uint32_t m_viewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const eastl::string& name = eastl::string());
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime();
		void OnUpdateEditor(EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	};
}