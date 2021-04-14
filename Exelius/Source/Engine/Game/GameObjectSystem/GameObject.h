#pragma once
#include <EASTL/string.h>
#include <EASTL/unordered_map.h>
#include <rapidjson/document.h>

#include "Source/Engine/Game/GameObjectSystem/Components/ComponentHandle.h"
#include "Source/Engine/Game/GameObjectSystem/Components/Component.h"

namespace Exelius
{
    class TextFileResource;

	class GameObject
	{
    private:
        uint32_t m_id;
        bool m_enabled;
        eastl::string m_name;
        eastl::unordered_map<Component::Type, Handle> m_components;

    public:
        GameObject(uint32_t id);
        GameObject(const GameObject&) = delete;
        GameObject(GameObject&&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject& operator=(GameObject&&) = delete;
        ~GameObject() = default;

        bool Initialize();
        bool Initialize(TextFileResource* pResource);
        void RemoveComponents();

        bool IsEnabled() const { return m_enabled; }
        void SetEnabled(bool isEnabled = true) { m_enabled = isEnabled; }

        template <class ComponentType>
        Handle AddComponent()
        {
            Handle newHandle = GameObjectSystem::GetInstance()->AddComponent<ComponentType>();
            m_components[ComponentType::kType] = newHandle;
        }
    
        template <class ComponentType>
        ComponentHandle<ComponentType> GetComponent()
        {
            return ComponentHandle<ComponentType>(m_components[ComponentType::kType]);
        }

        uint32_t GetId() const { return m_id; }
        const eastl::string& GetName() const { return m_name; }

    private:
        void ParseComponentArray(const rapidjson::Document& jsonDoc);
	};
}