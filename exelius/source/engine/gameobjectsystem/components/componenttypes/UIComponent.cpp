#include "EXEPCH.h"
#include "source/engine/gameobjectsystem/components/componenttypes/UIComponent.h"

#include "source/resource/ResourceHandle.h"
#include "source/render/RenderManager.h"

#include "source/engine/gameobjectsystem/GameObject.h"
#include "source/engine/gameobjectsystem/components/componenttypes/TransformComponent.h"

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
        EXE_ASSERT(m_pOwner);

        m_gameObjectSystemLog.Info("Creating UI Component");

        auto textureData = jsonComponentData.FindMember("Texture");

        if (textureData == jsonComponentData.MemberEnd())
        {
            m_gameObjectSystemLog.Warn("Initialization for UIComponent failed: No texture data found.");
            return false;
        }

        EXE_ASSERT(textureData->value.IsString());

        m_textureID = textureData->value.GetString();
        EXE_ASSERT(m_textureID.IsValid());

        ResourceHandle texture(m_textureID);
        texture.QueueLoad(true);
        texture.LockResource();

        auto sliceData = jsonComponentData.FindMember("SliceData");
        if (sliceData == jsonComponentData.MemberEnd())
        {
            m_gameObjectSystemLog.Warn("Initialization for UIComponent failed: No slice data found.");
            return false;
        }

        EXE_ASSERT(sliceData->value.IsArray());

        int index = 0;
        eastl::array<float, 6> sliceList;
        for (auto slice = sliceData->value.Begin(); slice != sliceData->value.End(); ++slice)
        {
            EXE_ASSERT(slice->IsNumber());
            sliceList[index] = slice->GetFloat();
            ++index;
        }

        // Should NEVER change
        m_commands[0].m_sourceFrame = FRectangle(0.0f, 0.0f, sliceList[0], sliceList[2]); // Top-Left
        m_commands[1].m_sourceFrame = FRectangle(sliceList[0], 0.0f, sliceList[1] - sliceList[0], sliceList[2]); // Top
        m_commands[2].m_sourceFrame = FRectangle(sliceList[1], 0.0f, sliceList[4] - sliceList[1], sliceList[2]); // Top-Right
        m_commands[3].m_sourceFrame = FRectangle(0.0f, sliceList[2], sliceList[0], sliceList[3] - sliceList[2]); // Left
        m_commands[4].m_sourceFrame = FRectangle(sliceList[0], sliceList[2], sliceList[1] - sliceList[0], sliceList[3] - sliceList[2]); // Center
        m_commands[5].m_sourceFrame = FRectangle(sliceList[1], sliceList[2], sliceList[4] - sliceList[1], sliceList[3] - sliceList[2]); // Right
        m_commands[6].m_sourceFrame = FRectangle(0.0f, sliceList[3], sliceList[0], sliceList[5] - sliceList[3]); // Bottom-Left
        m_commands[7].m_sourceFrame = FRectangle(sliceList[0], sliceList[3], sliceList[1] - sliceList[0], sliceList[5] - sliceList[3]); // Bottom
        m_commands[8].m_sourceFrame = FRectangle(sliceList[1], sliceList[3], sliceList[4] - sliceList[1], sliceList[5] - sliceList[3]); // Bottom-Right

        // Changed directly via public functions.
        m_dimensions.w = sliceList[1] - sliceList[0];
        m_dimensions.h = sliceList[3] - sliceList[2];

        // Based on transform, although we do not know that yet.
        m_commands[0].m_destinationFrame = m_commands[0].m_sourceFrame;
        m_commands[1].m_destinationFrame = m_commands[1].m_sourceFrame;
        m_commands[2].m_destinationFrame = m_commands[2].m_sourceFrame;
        m_commands[3].m_destinationFrame = m_commands[3].m_sourceFrame;
        m_commands[4].m_destinationFrame = m_commands[4].m_sourceFrame;
        m_commands[5].m_destinationFrame = m_commands[5].m_sourceFrame;
        m_commands[6].m_destinationFrame = m_commands[6].m_sourceFrame;
        m_commands[7].m_destinationFrame = m_commands[7].m_sourceFrame;
        m_commands[8].m_destinationFrame = m_commands[8].m_sourceFrame;

        m_commands[0].m_renderLayer = RenderCommand::RenderLayer::UI;
        m_commands[1].m_renderLayer = RenderCommand::RenderLayer::UI;
        m_commands[2].m_renderLayer = RenderCommand::RenderLayer::UI;
        m_commands[3].m_renderLayer = RenderCommand::RenderLayer::UI;
        m_commands[4].m_renderLayer = RenderCommand::RenderLayer::UI;
        m_commands[5].m_renderLayer = RenderCommand::RenderLayer::UI;
        m_commands[6].m_renderLayer = RenderCommand::RenderLayer::UI;
        m_commands[7].m_renderLayer = RenderCommand::RenderLayer::UI;
        m_commands[8].m_renderLayer = RenderCommand::RenderLayer::UI;

        m_commands[0].m_texture = m_textureID;
        m_commands[1].m_texture = m_textureID;
        m_commands[2].m_texture = m_textureID;
        m_commands[3].m_texture = m_textureID;
        m_commands[4].m_texture = m_textureID;
        m_commands[5].m_texture = m_textureID;
        m_commands[6].m_texture = m_textureID;
        m_commands[7].m_texture = m_textureID;
        m_commands[8].m_texture = m_textureID;

        return true;
    }

    void UIComponent::Render()
    {
        if (!m_pOwner)
        {
            m_gameObjectSystemLog.Fatal("Owner of UIComponent was nullptr. This should NEVER happen.");
            return;
        }

        if (!m_pOwner->IsEnabled())
            return;

        if (!m_textureID.IsValid())
        {
            m_gameObjectSystemLog.Error("Bailing render because m_textureID was invalid.");
            return;
        }

        auto transformComponent = m_pOwner->GetComponent<TransformComponent>();

        if (!transformComponent.IsValid())
        {
            m_gameObjectSystemLog.Warn("Transform Component was invalid!");
            return;
        }

        for (int i = 0; i < 9; ++i)
        {
            auto command = m_commands[i];
            command.m_destinationFrame.m_left += transformComponent->GetX();
            command.m_destinationFrame.m_top += transformComponent->GetY();
            RenderManager::GetInstance()->PushRenderCommand(command);
        }

        //for (auto& command : m_commands)
        //{
        //    // Adjust the command according to the transform here.
        //    command.m_destinationFrame.m_left += transformComponent->GetX();
        //    command.m_destinationFrame.m_top += transformComponent->GetY();
        //    RenderManager::GetInstance()->PushRenderCommand(command);
        //}
    }

    void UIComponent::Destroy()
    {
        ResourceHandle texture(m_textureID);
        texture.UnlockResource();
    }

    void UIComponent::SetW(float w)
    {
        if (w < 1.0f)
            w = 1.0f;

        m_commands[1].m_destinationFrame.m_width = w;
        m_commands[4].m_destinationFrame.m_width = w;
        m_commands[7].m_destinationFrame.m_width = w;

        // Left side needs to adjust for the change in width.
        m_commands[2].m_destinationFrame.m_left = m_commands[1].m_destinationFrame.m_left + m_commands[1].m_destinationFrame.m_width;
        m_commands[5].m_destinationFrame.m_left = m_commands[4].m_destinationFrame.m_left + m_commands[4].m_destinationFrame.m_width;
        m_commands[8].m_destinationFrame.m_left = m_commands[7].m_destinationFrame.m_left + m_commands[7].m_destinationFrame.m_width;

        m_dimensions.w = w;
    }

    void UIComponent::SetH(float h)
    {
        if (h < 1.0f)
            h = 1.0f;

        m_commands[3].m_destinationFrame.m_height = h;
        m_commands[4].m_destinationFrame.m_height = h;
        m_commands[5].m_destinationFrame.m_height = h;

        // bottom needs to adjust for the change in height.
        m_commands[6].m_destinationFrame.m_top = m_commands[3].m_destinationFrame.m_top + m_commands[3].m_destinationFrame.m_height;
        m_commands[7].m_destinationFrame.m_top = m_commands[4].m_destinationFrame.m_top + m_commands[4].m_destinationFrame.m_height;
        m_commands[8].m_destinationFrame.m_top = m_commands[5].m_destinationFrame.m_top + m_commands[5].m_destinationFrame.m_height;

        m_dimensions.h = h;
    }

    void UIComponent::Reset()
    {
        m_commands[0].m_destinationFrame = m_commands[0].m_sourceFrame;
        m_commands[1].m_destinationFrame = m_commands[1].m_sourceFrame;
        m_commands[2].m_destinationFrame = m_commands[2].m_sourceFrame;
        m_commands[3].m_destinationFrame = m_commands[3].m_sourceFrame;
        m_commands[4].m_destinationFrame = m_commands[4].m_sourceFrame;
        m_commands[5].m_destinationFrame = m_commands[5].m_sourceFrame;
        m_commands[6].m_destinationFrame = m_commands[6].m_sourceFrame;
        m_commands[7].m_destinationFrame = m_commands[7].m_sourceFrame;
        m_commands[8].m_destinationFrame = m_commands[8].m_sourceFrame;

        m_dimensions.w = m_commands[4].m_sourceFrame.m_width;
        m_dimensions.h = m_commands[4].m_sourceFrame.m_height;
    }
}