#include "EXEPCH.h"
#include "source/engine/gameobjectsystem/components/componenttypes/UIComponent.h"

#include "source/resource/ResourceHandle.h"
#include "source/render/RenderManager.h"
#include "source/os/interface/graphics/Window.h"

#include "source/engine/gameobjectsystem/GameObject.h"
#include "source/engine/gameobjectsystem/components/componenttypes/TransformComponent.h"

#include "source/engine/ui/elements/UIImage.h"



#include <iostream>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    bool UIComponent::Initialize(const rapidjson::Value& jsonComponentData)
    {
        EXE_ASSERT(m_pOwner);
        m_gameObjectSystemLog.Info("Creating UI Component");

        if (!m_uiRootElement.Initialize(jsonComponentData))
        {
            m_gameObjectSystemLog.Error("UI Component Failed to Initialize.");
            return false;
        }

        return true;
    }

    void UIComponent::Update()
    {
        if (!m_pOwner)
        {
            m_gameObjectSystemLog.Fatal("Owner of UIComponent was nullptr. This should NEVER happen.");
            return;
        }

        if (!m_pOwner->IsEnabled())
            return;

        //static int renderedNum = 0;
        //static bool changed = false;
        auto size = RenderManager::GetInstance()->GetWindow()->GetWindowSize();
        /*if (renderedNum > 60000 && !changed)
        {
            RenderManager::GetInstance()->GetWindow()->SetWindowSize({ 360, 320 });
            changed = true;
        }
        else
            ++renderedNum;*/

        //RenderManager::GetInstance()->GetWindow()->SetView(View(FRectangle(0.0f, 0.0f, (float)size.x, (float)size.y)));
        m_uiRootElement.OnUpdate({ 0.0f, 0.0f, (float)size.w, (float)size.h });
    }

    void UIComponent::Render()
    {
        if (!m_pOwner)
        {
            m_gameObjectSystemLog.Fatal("Owner of UIComponent was nullptr. This should NEVER happen.");
            return;
        }

        /*std::cout << "\n\n";
        std::cout << (void*)m_pOwner << "\n";
        std::cout << sizeof(GameObject) << "\n";
        std::cout << alignof(GameObject) << "\n";
        std::cout << (void*)((uintptr_t)m_pOwner + offsetof(Exelius::GameObject, m_enabled)) << "\n";
        std::cout << offsetof(Exelius::GameObject, m_enabled) << "\n";*/
        /*if (!m_pOwner)
        {
            __debugbreak();
        }*/

        if (!m_pOwner->IsEnabled())
            return;

        // TODO: Make use of gameobject transform with UI.
        /*auto transformComponent = m_pOwner->GetComponent<TransformComponent>();

        if (!transformComponent.IsValid())
        {
            m_gameObjectSystemLog.Warn("Transform Component was invalid!");
            return;
        }*/

        m_uiRootElement.OnRender();
    }

    void UIComponent::Destroy()
    {
        m_uiRootElement.OnDestroy();
    }
}