#include "EXEPCH.h"
#include "CircleColliderComponent.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void CircleColliderComponent::SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("CircleColliderComponent");
		writer.StartObject(); // Start Circle Collider Component.
		{
			writer.Key("Offset");
			writer.StartArray(); // Start Offset Array.
			{
				writer.Double((double)m_offset.x);
				writer.Double((double)m_offset.y);
			}
			writer.EndArray(); // End Offset Array.

			writer.Key("Radius");
			writer.Double((double)m_radius);

			writer.Key("Density");
			writer.Double((double)m_density);

			writer.Key("Friction");
			writer.Double((double)m_friction);

			writer.Key("Restitution");
			writer.Double((double)m_restitution);

			writer.Key("RestitutionThreshold");
			writer.Double((double)m_restitutionThreshold);

			writer.Key("IsSensor");
			writer.Bool(m_isSensor);
		}
		writer.EndObject(); // End Circle Collider Component.
	}

	void CircleColliderComponent::DeserializeComponent(const rapidjson::Value& componentValue)
    {
		m_offset.x = componentValue.FindMember("Offset")->value.GetArray()[0].GetFloat();
		m_offset.y = componentValue.FindMember("Offset")->value.GetArray()[1].GetFloat();

		m_radius = componentValue.FindMember("Radius")->value.GetFloat();

		m_density = componentValue.FindMember("Density")->value.GetFloat();
		m_friction = componentValue.FindMember("Friction")->value.GetFloat();
		m_restitution = componentValue.FindMember("Restitution")->value.GetFloat();
		m_restitutionThreshold = componentValue.FindMember("RestitutionThreshold")->value.GetFloat();

		m_isSensor = componentValue.FindMember("IsSensor")->value.GetBool();
    }
}
