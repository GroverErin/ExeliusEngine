#include "EXEPCH.h"
#include "Source/Engine/GameObjectSystem/Components/ComponentTypes/TransformComponent.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    bool TransformComponent::Initialize()
    {
        EXE_ASSERT(m_pOwner);
        return true;
    }

    bool TransformComponent::Initialize(const rapidjson::Value& jsonComponentData)
    {
        Log log("GameObjectSystem");

        EXE_ASSERT(m_pOwner);

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
                log.Warn("Unhandled Component Data Object: {}", componentDataObj->name.GetString());
        }

        return true;
    }
}