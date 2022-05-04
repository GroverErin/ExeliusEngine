#pragma once
#include "source/engine/gameobjects/Component.h"
#include "source/utility/generic/Macros.h"

#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct CircleColliderComponent
		: public Component
	{
		glm::vec2 m_offset = { 0.0f, 0.0f };
		float m_radius = 0.5f;

		float m_density = 1.0f;
		float m_friction = 0.5f;
		float m_restitution = 0.0f;
		float m_restitutionThreshold = 0.5f;

		bool m_isSensor = false;

		// Storage for runtime
		void* m_pRuntimeFixture = nullptr;

		CircleColliderComponent() = default;

		virtual void SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer) final override;

		virtual void DeserializeComponent(const rapidjson::Value& componentValue) final override;
	};
}