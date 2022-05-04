#pragma once
#include "source/engine/gameobjects/Component.h"
#include "source/resource/ResourceHandle.h"
#include "source/utility/generic/Color.h"

#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct SpriteRendererComponent
		: public Component
	{
		ResourceHandle m_textureResource;
		float m_textureTilingMultiplier = 1.0f;
		Color m_color;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4& color);

		virtual void SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer) final override;

		virtual void DeserializeComponent(const rapidjson::Value& componentValue) final override;
	};
}