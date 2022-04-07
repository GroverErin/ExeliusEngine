#include "EXEPCH.h"
#include "CircleRendererComponent.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void CircleRendererComponent::SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("CircleRendererComponent");
		writer.StartObject(); // Start Circle Renderer Component.
		{
			writer.Key("Color");
			writer.StartArray(); // Start Color Array.
			{
				writer.Uint(m_color.r);
				writer.Uint(m_color.g);
				writer.Uint(m_color.b);
				writer.Uint(m_color.a);
			}
			writer.EndArray(); // End Color Array.

			writer.Key("Thickness");
			writer.Double((double)m_thickness);

			writer.Key("Fade");
			writer.Double((double)m_fade);
		}
		writer.EndObject(); // End Circle Renderer Component.
	}

	void CircleRendererComponent::DeserializeComponent(const rapidjson::Value& componentValue)
    {
		m_color.r = (uint8_t)componentValue.FindMember("Color")->value.GetArray()[0].GetUint();
		m_color.g = (uint8_t)componentValue.FindMember("Color")->value.GetArray()[1].GetUint();
		m_color.b = (uint8_t)componentValue.FindMember("Color")->value.GetArray()[2].GetUint();
		m_color.a = (uint8_t)componentValue.FindMember("Color")->value.GetArray()[3].GetUint();

		m_thickness = componentValue.FindMember("Thickness")->value.GetFloat();
		m_fade = componentValue.FindMember("Fade")->value.GetFloat();
    }
}
