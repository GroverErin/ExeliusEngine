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
		if (IsKeyDown(KeyCode::A))
		{
			m_cameraPosition.x -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
			m_cameraPosition.y -= sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
		}
		else if (IsKeyDown(KeyCode::D))
		{
			m_cameraPosition.x += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
			m_cameraPosition.y += sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
		}

		if (IsKeyDown(KeyCode::W))
		{
			m_cameraPosition.x += -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
			m_cameraPosition.y += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
		}
		else if (IsKeyDown(KeyCode::S))
		{
			m_cameraPosition.x -= -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
			m_cameraPosition.y -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * Time::DeltaTime;
		}

		if (m_rotation)
		{
			if (IsKeyDown(KeyCode::Q))
				m_cameraRotation += m_cameraRotationSpeed * Time::DeltaTime;
			if (IsKeyDown(KeyCode::E))
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

	void OrthographicCameraController::OnEvent(Event& evnt)
	{
		if (evnt.GetEventType() == EventType::MouseScrolled)
		{
			MouseScrolledEvent* pMouseScrolled = static_cast<MouseScrolledEvent*>(&evnt);
			OnMouseScrolled(*pMouseScrolled);
			evnt.m_isHandled = false;
		}
		else if (evnt.GetEventType() == EventType::WindowResized)
		{
			WindowResizedEvent* pWindowResized = static_cast<WindowResizedEvent*>(&evnt);
			OnWindowResized(*pWindowResized);
			evnt.m_isHandled = false;
		}
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