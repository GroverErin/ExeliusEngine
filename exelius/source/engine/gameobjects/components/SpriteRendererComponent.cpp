#include "EXEPCH.h"
#include "SpriteRendererComponent.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	SpriteRendererComponent::SpriteRendererComponent(const glm::vec4& color)
		: m_color(color)
		, m_textureTilingMultiplier(1.0f)
	{
		//
	}

	void SpriteRendererComponent::SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("SpriteRendererComponent");
		writer.StartObject(); // Start Sprite Renderer Component.
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

			writer.Key("TilingMultiplier");
			writer.Double((double)m_textureTilingMultiplier);

			writer.Key("Texture");
			writer.String(m_textureResource.GetID().Get().c_str(), m_textureResource.GetID().Get().size());
		}
		writer.EndObject(); // End Sprite Renderer Component.
	}

	void SpriteRendererComponent::DeserializeComponent(const rapidjson::Value& componentValue)
    {
		m_color.r = (uint8_t)componentValue.FindMember("Color")->value.GetArray()[0].GetUint();
		m_color.g = (uint8_t)componentValue.FindMember("Color")->value.GetArray()[1].GetUint();
		m_color.b = (uint8_t)componentValue.FindMember("Color")->value.GetArray()[2].GetUint();
		m_color.a = (uint8_t)componentValue.FindMember("Color")->value.GetArray()[3].GetUint();

		m_textureTilingMultiplier = componentValue.FindMember("TilingMultiplier")->value.GetFloat();

		if (componentValue.FindMember("Texture") != componentValue.MemberEnd())
		{
			m_textureResource.SetResourceID(componentValue.FindMember("Texture")->value.GetString());
			m_textureResource.LoadNow();
		}
    }
}
