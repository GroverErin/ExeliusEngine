#pragma once

#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Camera
	{
	protected:
		glm::mat4 m_projection = glm::mat4(1.0f);
	public:
		Camera() = default;
		Camera(const glm::mat4 & projection)
			: m_projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_projection; }
	};
}
