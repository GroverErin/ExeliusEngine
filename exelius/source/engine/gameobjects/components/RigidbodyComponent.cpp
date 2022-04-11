#include "EXEPCH.h"
#include "RigidbodyComponent.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void RigidbodyComponent::SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("RigidbodyComponent");
		writer.StartObject(); // Start Rigidbody Component.
		{
			writer.Key("BodyType");
			eastl::string bodyString = RigidbodyBodyTypeToString(m_bodyType);
			writer.String(bodyString.c_str(), (rapidjson::SizeType)bodyString.size());

			writer.Key("IsFixedRotation");
			writer.Bool(m_isFixedRotation);

			writer.Key("GravityScale");
			writer.Double((double)m_gravityScale);
		}
		writer.EndObject(); // End Rigidbody Component.
	}

	void RigidbodyComponent::DeserializeComponent(const rapidjson::Value& componentValue)
    {
        m_bodyType = RigidbodyBodyTypeFromString(componentValue.FindMember("BodyType")->value.GetString());
        m_isFixedRotation = componentValue.FindMember("IsFixedRotation")->value.GetBool();
        m_gravityScale = componentValue.FindMember("GravityScale")->value.GetFloat();
    }

	RigidbodyComponent::BodyType RigidbodyComponent::RigidbodyBodyTypeFromString(const eastl::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return RigidbodyComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return RigidbodyComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return RigidbodyComponent::BodyType::Kinematic;

		EXE_ASSERT(false);
		return RigidbodyComponent::BodyType::Static;
	}

	eastl::string RigidbodyComponent::RigidbodyBodyTypeToString(BodyType bodyType) const
	{
		switch (bodyType)
		{
			case RigidbodyComponent::BodyType::Static: return "Static";
			case RigidbodyComponent::BodyType::Dynamic: return "Dynamic";
			case RigidbodyComponent::BodyType::Kinematic: return "Kinematic";
		}

		EXE_ASSERT(false);
		return {};
	}
}
