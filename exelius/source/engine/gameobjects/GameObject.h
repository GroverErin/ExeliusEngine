#pragma once
#include "source/utility/generic/Macros.h"
#include "source/engine/scenesystem/Scene.h"
#include "components/GUIDComponent.h"
#include "components/NameComponent.h"

#include <entt/entt.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class GameObject
	{
		entt::entity m_gameObjectID;
		Scene* m_pOwningScene = nullptr;

	public:
		GameObject()
			: m_gameObjectID(entt::null)
			, m_pOwningScene(nullptr)
		{
			//
		}

		GameObject(entt::entity gameObjectID, Scene* pOwningScene)
			: m_gameObjectID(gameObjectID)
			, m_pOwningScene(pOwningScene)
		{
			//
		}

		GameObject(const GameObject& other) = default;

		GUID GetGUID() { return GetComponent<GUIDComponent>().m_GUID; }
		const eastl::string& GetName() { return GetComponent<NameComponent>().m_name; }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EXE_ASSERT(!HasComponent<T>()); // Component already exists!
			T& component = m_pOwningScene->m_registry.emplace<T>(m_gameObjectID, std::forward<Args>(args)...);
			component.OnComponentAdded(m_pOwningScene);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			EXE_ASSERT(m_pOwningScene);
			T& component = m_pOwningScene->m_registry.emplace_or_replace<T>(m_gameObjectID, std::forward<Args>(args)...);
			component.OnComponentAdded(m_pOwningScene);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			EXE_ASSERT(HasComponent<T>());
			return m_pOwningScene->m_registry.get<T>(m_gameObjectID);
		}

		template<typename T>
		bool HasComponent()
		{
			if (!m_pOwningScene)
				return false;
			if (m_pOwningScene->m_registry.valid(m_gameObjectID))
				return m_pOwningScene->m_registry.all_of<T>(m_gameObjectID);

			return false;
		}

		template<typename T>
		void RemoveComponent()
		{
			EXE_ASSERT(HasComponent<T>());
			m_pOwningScene->m_registry.remove<T>(m_gameObjectID);
		}

		operator bool() const { return m_gameObjectID != entt::null; }
		operator entt::entity() const { return m_gameObjectID; }
		operator uint32_t() const { return (uint32_t)m_gameObjectID; }

		bool operator==(const GameObject& other) const { return m_gameObjectID == other.m_gameObjectID && m_pOwningScene == other.m_pOwningScene; }
		bool operator!=(const GameObject& other) const { return !(*this == other); }
	};
}
