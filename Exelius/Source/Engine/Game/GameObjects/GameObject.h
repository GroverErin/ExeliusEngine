#pragma once

#include <EASTL/vector.h>

namespace Exelius
{
    class Component;
    class JSONResource;

	class GameObject
	{
        int m_id;
        bool m_enabled;
    public:
        GameObject(int id)
            : m_id(id)
            , m_enabled(true)
        {
        }
        GameObject(const GameObject&) = delete;
        GameObject(GameObject&&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject& operator=(GameObject&&) = delete;
        ~GameObject() = default;

        bool Initialize(JSONResource* pResource);
        void Destroy();

        void Update(float deltaTime);
        void Render();

        bool IsEnabled() const { return m_enabled; }
        void SetEnabled(bool isEnabled = true) { m_enabled = isEnabled; }

        void AddComponent(Component* pComponent);

        Component* GetComponent();
    
        template<class ComponentType>
        ComponentType* GetComponent()
        {
            return static_cast<ComponentType*>GetComponent(ComponentType::kIdentifier);
        }

        int GetId() const { return m_id; }

    private:
        eastl::vector<Component*> m_components;
	};
}