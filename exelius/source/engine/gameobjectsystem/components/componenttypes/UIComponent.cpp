#include "EXEPCH.h"
#include "Source/Engine/GameObjectSystem/Components/ComponentTypes/UIComponent.h"

#include "Source/Resource/ResourceHandle.h"
#include "Source/Render/RenderManager.h"

#include "Source/Engine/GameObjectSystem/GameObject.h"
#include "Source/Engine/GameObjectSystem/Components/ComponentTypes/TransformComponent.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    bool UIComponent::Initialize()
    {
        EXE_ASSERT(m_pOwner);
        return true;
    }

    bool UIComponent::Initialize(const rapidjson::Value& jsonComponentData)
    {
        Log log("GameObjectSystem");

        EXE_ASSERT(m_pOwner);


        auto textureData = jsonComponentData.FindMember("Texture");

        if (textureData == jsonComponentData.MemberEnd())
        {
            log.Warn("Initialization for SpriteComponent failed: No Spritesheet data found.");
            return false;
        }

        EXE_ASSERT(textureData->name.IsString());

        m_textureID = textureData->value.GetString();
        EXE_ASSERT(m_textureID.IsValid());

        ResourceHandle texture(m_textureID);
        texture.QueueLoad(true);
        texture.LockResource();

        return true;
    }

    void UIComponent::Render() const
    {
        Log log("GameObjectSystem");
        if (!m_pOwner)
        {
            log.Fatal("Owner of UIComponent was nullptr. This should NEVER happen.");
            return;
        }

        if (!m_textureID.IsValid())
        {
            log.Error("Bailing render because m_textureID was invalid.");
            return;
        }

        auto transformComponent = m_pOwner->GetComponent<TransformComponent>();

        if (!transformComponent.IsValid())
            return;

        RenderCommand command;
        command.m_renderLayer = RenderCommand::RenderLayer::UI;
        command.m_texture = m_textureID;
        command.m_spriteFrame = IRectangle(0, 0, 256, 256);
        command.m_scaleFactor = { 1.0f, 1.0f };
        command.m_position = { transformComponent->GetX(), transformComponent->GetY() };
        RenderManager::GetInstance()->PushRenderCommand(command);
    }

    void UIComponent::Destroy()
    {
        ResourceHandle texture(m_textureID);
        texture.UnlockResource();
    }
}