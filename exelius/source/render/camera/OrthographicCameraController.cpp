#include "EXEPCH.h"
#include "OrthographicCameraController.h"
#include "include/Input.h"

#include "source/utility/generic/Time.h"
#include "source/os/events/ApplicationEvents.h"
#include "source/os/events/MouseEvents.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_aspectRatio(aspectRatio)
		, m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel)
		, m_rotation(rotation)
	{
		//
	}

	void OrthographicCameraController::OnUpdate()
	{
		if (IsKeyPressed(KeyCode::A))
		{
			m_cameraPosition.x -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
			m_cameraPosition.y -= sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
		}
		else if (IsKeyPressed(KeyCode::D))
		{
			m_cameraPosition.x += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
			m_cameraPosition.y += sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
		}

		if (IsKeyPressed(KeyCode::W))
		{
			m_cameraPosition.x += -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
			m_cameraPosition.y += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
		}
		else if (IsKeyPressed(KeyCode::S))
		{
			m_cameraPosition.x -= -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
			m_cameraPosition.y -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
		}

		if (m_rotation)
		{
			if (IsKeyPressed(KeyCode::Q))
				m_cameraRotation += m_cameraRotationSpeed * Time::DeltaTime;
			if (IsKeyPressed(KeyCode::E))
				m_cameraRotation -= m_cameraRotationSpeed * Time::DeltaTime;

			if (m_cameraRotation > 180.0f)
				m_cameraRotation -= 360.0f;
			else if (m_cameraRotation <= -180.0f)
				m_cameraRotation += 360.0f;

			m_camera.SetRotation(m_cameraRotation);
		}

		m_camera.SetPosition(m_cameraPosition);

		m_cameraTranslationSpeed = m_zoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_aspectRatio = width / height;
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_zoomLevel -= e.GetWheelYDelta() * 0.25f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}