#include "EXEPCH.h"
#include "BoxColliderComponent.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void BoxColliderComponent::SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("BoxColliderComponent");
		writer.StartObject(); // Start Box Collider Component.
		{
			writer.Key("Offset");
			writer.StartArray(); // Start Offset Array.
			{
				writer.Double((double)m_offset.x);
				writer.Double((double)m_offset.y);
			}
			writer.EndArray(); // End Offset Array.

			writer.Key("Size");
			writer.StartArray(); // Start Size Array.
			{
				writer.Double((double)m_size.x);
				writer.Double((double)m_size.y);
			}
			writer.EndArray(); // End Size Array.

			writer.Key("Density");
			writer.Double((double)m_density);

			writer.Key("Friction");
			writer.Double((double)m_friction);

			writer.Key("Restitution");
			writer.Double((double)m_restitution);

			writer.Key("RestitutionThreshold");
			writer.Double((double)m_restitutionThreshold);
		}
		writer.EndObject(); // End Box Collider Component.
	}

	void BoxColliderComponent::DeserializeComponent(const rapidjson::Value& componentValue)
	{
		m_offset.x = componentValue.FindMember("Offset")->value.GetArray()[0].GetFloat();
		m_offset.y = componentValue.FindMember("Offset")->value.GetArray()[1].GetFloat();

		m_size.x = componentValue.FindMember("Size")->value.GetArray()[0].GetFloat();
		m_size.y = componentValue.FindMember("Size")->value.GetArray()[1].GetFloat();

		m_density = componentValue.FindMember("Density")->value.GetFloat();
		m_friction = componentValue.FindMember("Friction")->value.GetFloat();
		m_restitution = componentValue.FindMember("Restitution")->value.GetFloat();
		m_restitutionThreshold = componentValue.FindMember("RestitutionThreshold")->value.GetFloat();
	}
}
