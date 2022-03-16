#pragma once
#include "Scene.h"

#include <entt/entt.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Entity
	{
		entt::entity m_entityID;
		Scene* m_pScene = nullptr;

	public:
		Entity()
			: m_entityID(entt::null)
			, m_pScene(nullptr)
		{
			//
		}

		Entity(entt::entity handle, Scene* pScene)
			: m_entityID(handle), m_pScene(pScene)
		{
			//
		}

		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EXE_ASSERT(!HasComponent<T>());
			T& component = m_pScene->m_registry.emplace<T>(m_entityID, std::forward<Args>(args)...);
			m_pScene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			EXE_ASSERT(HasComponent<T>());
			return m_pScene->m_registry.get<T>(m_entityID);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_pScene->m_registry.all_of<T>(m_entityID);
		}

		template<typename T>
		void RemoveComponent()
		{
			EXE_ASSERT(HasComponent<T>());
			m_pScene->m_Registry.remove<T>(m_entityID);
		}

		operator bool() const { return m_entityID != entt::null; }
		operator entt::entity() const { return m_entityID; }
		operator uint32_t() const { return (uint32_t)m_entityID; }

		bool operator==(const Entity& other) const
		{
			return m_entityID == other.m_entityID && m_pScene == other.m_pScene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	};
}
