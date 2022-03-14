#include "EXEPCH.h"
#include "source/engine/gameobjectsystem/components/componenttypes/AnimationComponent.h"
#include "source/resource/ResourceHandle.h"

#include "source/engine/gameobjectsystem/GameObject.h"
#include "source/render/RenderManager.h"
#include "source/engine/gameobjectsystem/components/componenttypes/TransformComponent.h"
#include "source/engine/resources/resourcetypes/SpritesheetResource.h"

#include "source/utility/generic/Time.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    bool AnimationComponent::Initialize()
    {
        EXE_ASSERT(m_pOwner);
        return true;
    }

    bool AnimationComponent::Initialize(const rapidjson::Value& jsonComponentData)
    {
        EXE_ASSERT(m_pOwner);

        // Get the spritesheet associated with this animation.
        auto spritesheetData = jsonComponentData.FindMember("Spritesheet");

        if (spritesheetData == jsonComponentData.MemberEnd())
        {
            m_gameObjectSystemLog.Warn("Initialization for SpriteComponent failed: No Spritesheet data found.");
            return false;
        }

        EXE_ASSERT(spritesheetData->name.IsString());

        if (!ParseSpritesheet(spritesheetData->value))
            return false;


        // Now we have the spritesheet, so lets populate the animation frame data.

        auto animationData = jsonComponentData.FindMember("Animations");

        if (animationData == jsonComponentData.MemberEnd())
        {
            m_gameObjectSystemLog.Warn("Initialization for AnimationComponent failed: No Animation data found.");
            Destroy();
            return false;
        }

        if (!ParseAnimations(animationData->value))
        {
            Destroy();
            return false;
        }

        // Now we have the animations. Lets set the default one.

        auto defaultAnimation = jsonComponentData.FindMember("DefaultAnimation");

        if (defaultAnimation == jsonComponentData.MemberEnd())
        {
            m_gameObjectSystemLog.Warn("Initialization for AnimationComponent failed: No default Animation data found.");
            Destroy();
            return false;
        }

        EXE_ASSERT(defaultAnimation->value.IsString());
        m_currentAnimationName = defaultAnimation->value.GetString();
        m_defaultAnimationName = m_currentAnimationName;


        return true;
    }

    void AnimationComponent::Destroy()
    {
        ResourceHandle spriteSheet(m_spriteSheetID);
        spriteSheet.UnlockResource();
    }

    void AnimationComponent::Update()
    {
        if (!m_pOwner)
        {
            m_gameObjectSystemLog.Fatal("Owner of UIComponent was nullptr. This should NEVER happen.");
            return;
        }

        if (!m_pOwner->IsEnabled())
            return;

        if (m_currentFrameTime >= m_currentAnimationFramerate / 60.0f)
        {
            // Get this animation
            auto found = m_animations.find(m_currentAnimationName);
            if (found == m_animations.end())
            {
                return;
            }

            // Reset frame timer
            m_currentFrameTime = 0.0f;

            // Change to next frame
            ++m_currentFrameIndex;

            // Check frame index bounds.
            if (m_currentFrameIndex >= found->second.m_frameIDs.size() && found->second.m_doesLoop)
            {
                m_currentFrameIndex = 0;
            }
            else if (m_currentFrameIndex >= found->second.m_frameIDs.size())
            {
                m_currentFrameIndex = 0;
                m_currentAnimationName = m_defaultAnimationName;

                found = m_animations.find(m_currentAnimationName);
                if (found == m_animations.end())
                {
                    return;
                }

                m_currentAnimationFramerate = found->second.m_animationFramerate;
            }

            // Retrieve the current frames sprite id.
            m_currentSpriteFrameName = found->second.m_frameIDs[m_currentFrameIndex];
        }
        else
            m_currentFrameTime += Time::DeltaTime;
    }

    void AnimationComponent::Render() const
    {
        if (!m_pOwner)
        {
            m_gameObjectSystemLog.Fatal("Owner of UIComponent was nullptr. This should NEVER happen.");
            return;
        }

        if (!m_pOwner->IsEnabled())
            return;

        if (!m_currentSpriteFrameName.IsValid())
        {
            m_gameObjectSystemLog.Error("Bailing render because sprite ID was invalid.");
            return;
        }

        // Get the transform component sibling.
        auto transformComponent = m_pOwner->GetComponent<TransformComponent>();

        ResourceHandle spriteSheet(m_spriteSheetID);

        auto* pSheet = spriteSheet.GetAs<SpritesheetResource>();

        auto* pRenderManager = RenderManager::GetInstance();
        if (!pRenderManager)
            return;

        if (!pSheet)
            return;

        if (!transformComponent.IsValid())
            return;

        RenderCommand command;
        command.m_renderLayer = RenderCommand::RenderLayer::World;
        command.m_texture = pSheet->GetTextureResource();
        command.m_destinationFrame = { transformComponent->GetX(), transformComponent->GetY(), transformComponent->GetW(), transformComponent->GetH() };
        command.m_sourceFrame = pSheet->GetSprite(m_currentSpriteFrameName);
        //command.m_spriteFrame = pSheet->GetSprite(m_spriteID);
        //command.m_scaleFactor = { m_xScale, m_yScale };
        //command.m_position = { transformComponent->GetX() + m_xOffset, transformComponent->GetY() + m_yOffset };
        pRenderManager->PushRenderCommand(command);
    }

    void AnimationComponent::Play(const StringIntern& animationName)
    {
        m_currentFrameIndex = 0;
        m_currentAnimationName = animationName;

        auto found = m_animations.find(m_currentAnimationName);
        if (found == m_animations.end())
        {
            return;
        }

        m_currentAnimationFramerate = found->second.m_animationFramerate;

        // Retrieve the current frames sprite id.
        m_currentSpriteFrameName = found->second.m_frameIDs[m_currentFrameIndex];
    }

    void AnimationComponent::Stop()
    {
    }

    bool AnimationComponent::IsPlaying()
    {
        return false;
    }

    bool AnimationComponent::ParseSpritesheet(const rapidjson::Value& spritesheetData)
    {
        EXE_ASSERT(spritesheetData.IsString());

        m_spriteSheetID = spritesheetData.GetString();
        EXE_ASSERT(m_spriteSheetID.IsValid());

        ResourceHandle spriteSheet(m_spriteSheetID);
        spriteSheet.QueueLoad(true);

        // We lock here until this component is destroyed.
        spriteSheet.LockResource();

        return true;
    }

    bool AnimationComponent::ParseAnimations(const rapidjson::Value& animationData)
    {
        EXE_ASSERT(animationData.IsArray());

        for (const auto& animation : animationData.GetArray())
        {
            EXE_ASSERT(animation.IsObject());

            Animation newAnimation;

            auto nameMember = animation.FindMember("name");
            EXE_ASSERT(nameMember->value.IsString());
            StringIntern animationName = nameMember->value.GetString();

            auto framerateMember = animation.FindMember("frameRate");
            EXE_ASSERT(framerateMember->value.IsNumber());
            newAnimation.m_animationFramerate = framerateMember->value.GetFloat();

            auto loopMember = animation.FindMember("doesLoop");
            EXE_ASSERT(loopMember->value.IsBool());
            newAnimation.m_doesLoop = loopMember->value.GetBool();

            auto spriteArrayMember = animation.FindMember("spriteArray");
            EXE_ASSERT(spriteArrayMember->value.IsArray());

            for (const auto& spriteName : spriteArrayMember->value.GetArray())
            {
                EXE_ASSERT(spriteName.IsString());
                newAnimation.m_frameIDs.emplace_back(spriteName.GetString());
            }

            m_animations.emplace(animationName, newAnimation);
        }

        return true;
    }
}