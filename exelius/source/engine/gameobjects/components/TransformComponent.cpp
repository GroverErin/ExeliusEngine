#include "EXEPCH.h"
#include "TransformComponent.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	TransformComponent::TransformComponent(const glm::vec3& translation)
		: m_translation(translation)
		, m_rotation({ 0.0f, 0.0f, 0.0f })
		, m_scale ({ 1.0f, 1.0f, 1.0f })
	{
		//
	}

	glm::mat4 TransformComponent::GetTransform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(m_rotation));

		return glm::translate(glm::mat4(1.0f), m_translation)
			* rotation
			* glm::scale(glm::mat4(1.0f), m_scale);
	}

	void TransformComponent::SerializeComponent(rapidjson::Writer<rapidjson::StringBuffer>& writer)
	{
		writer.Key("TransformComponent");
		writer.StartArray(); // Start the Transform Array.
		{
			writer.StartArray(); // Start the Translation Array.
			{
				writer.Double((double)m_translation.x);
				writer.Double((double)m_translation.y);
				writer.Double((double)m_translation.z);
			}
			writer.EndArray(); // End the Translation Array.

			writer.StartArray(); // Start the Rotation Array.
			{
				writer.Double((double)m_rotation.x);
				writer.Double((double)m_rotation.y);
				writer.Double((double)m_rotation.z);
			}
			writer.EndArray(); // End the Rotation Array.

			writer.StartArray(); // Start the Scale Array.
			{
				writer.Double((double)m_scale.x);
				writer.Double((double)m_scale.y);
				writer.Double((double)m_scale.z);
			}
			writer.EndArray(); // End the Scale Array.
		}
		writer.EndArray(); // End the Transform Array.
	}

	void TransformComponent::DeserializeComponent(const rapidjson::Value& componentValue)
	{
		m_translation.x = componentValue.GetArray()[0].GetArray()[0].GetFloat();
		m_translation.y = componentValue.GetArray()[0].GetArray()[1].GetFloat();
		m_translation.z = componentValue.GetArray()[0].GetArray()[2].GetFloat();

		m_rotation.x = componentValue.GetArray()[1].GetArray()[0].GetFloat();
		m_rotation.y = componentValue.GetArray()[1].GetArray()[1].GetFloat();
		m_rotation.z = componentValue.GetArray()[1].GetArray()[2].GetFloat();

		m_scale.x = componentValue.GetArray()[2].GetArray()[0].GetFloat();
		m_scale.y = componentValue.GetArray()[2].GetArray()[1].GetFloat();
		m_scale.z = componentValue.GetArray()[2].GetArray()[2].GetFloat();
	}
}