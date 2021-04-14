#include "EXEPCH.h"
#include "Source/Engine/Game/GameObjectSystem/Components/ComponentTypes/TransformComponent.h"

namespace Exelius
{
    bool TransformComponent::Initialize(GameObject* pOwner)
    {
        EXE_ASSERT(pOwner);
        m_pOwner = pOwner;
        return true;
    }

    bool TransformComponent::Initialize(GameObject* pOwner, const rapidjson::Value& jsonComponentData)
    {
        EXE_ASSERT(pOwner);
        m_pOwner = pOwner;

        for (auto componentDataObj = jsonComponentData.MemberBegin(); componentDataObj != jsonComponentData.MemberEnd(); ++componentDataObj)
        {
            eastl::string objectName = componentDataObj->name.GetString();

            if (objectName == "x")
                m_position.x = componentDataObj->value.GetFloat();
            else if (objectName == "y")
                m_position.y = componentDataObj->value.GetFloat();
            else if (objectName == "w")
                m_dimensions.w = componentDataObj->value.GetFloat();
            else if (objectName == "h")
                m_dimensions.h = componentDataObj->value.GetFloat();
            else
                EXELOG_ENGINE_WARN("Unhandled Component Data Object: {}", componentDataObj->name.GetString());
        }

        return true;
    }
}