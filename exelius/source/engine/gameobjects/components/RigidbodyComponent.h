#pragma once
#include "source/engine/gameobjects/Component.h"

#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct RigidbodyComponent
		: public Component
	{
		enum class BodyType
		{
			Static = 0,
			Dynamic,
			Kinematic
		};

		BodyType m_bodyType = BodyType::Static;
		bool m_isFixedRotation = false;
		void* m_pRuntimeBody = nullptr;
		float m_gravityScale = 1.0f;

		RigidbodyComponent() = default;
		RigidbodyComponent(const RigidbodyComponent&) = default;

		virtual void SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer) final override;

		virtual void DeserializeComponent(const rapidjson::Value& componentValue) final override;

	private:
		BodyType RigidbodyBodyTypeFromString(const eastl::string& bodyTypeString);
		eastl::string RigidbodyComponent::RigidbodyBodyTypeToString(BodyType bodyType) const;
	};
}