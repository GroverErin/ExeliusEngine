#include "EXEPCH.h"
#include "source/os/input/InputManager.h"

#include "source/os/events/KeyEvents.h"
#include "source/os/events/MouseEvents.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	bool InputManager::Initialize()
	{
		m_keyState = { false };
		m_previousKeyState = { false };
		m_mouseButtonState = { false };
		m_previousMouseButtonState = { false };
		m_wheelState = { false };
		return true;
	}

	void InputManager::SetKeyState(KeyCode key, bool isDown)
	{
		EXE_ASSERT(key != KeyCode::kCount);

		//Set it's state.
		m_keyState[(size_t)key] = isDown;
	}

	bool InputManager::IsKeyDown(KeyCode key) const
	{
		EXE_ASSERT(static_cast<size_t>(key) < m_keyState.size());
		return m_keyState[(size_t)key];
	}

	bool InputManager::IsKeyPressed(KeyCode key) const
	{
		return (m_keyState[(size_t)key] && !m_previousKeyState[(size_t)key]);
	}

	bool InputManager::IsKeyReleased(KeyCode key) const
	{
		return (!m_keyState[(size_t)key] && m_previousKeyState[(size_t)key]);
	}

	void InputManager::SetButtonState(MouseCode button, bool down)
	{
		if (button != MouseCode::kCount)
		{
			m_mouseButtonState[(size_t)button] = down;
		}
	}

	void InputManager::SetWheelState(ScrollWheelDirection direction)
	{
		if (direction == ScrollWheelDirection::WheelUp)
		{
			m_wheelState[(size_t)ScrollWheelDirection::WheelUp] = true;
			m_wheelState[(size_t)ScrollWheelDirection::WheelDown] = false;
		}
		else if (direction == ScrollWheelDirection::WheelDown)
		{
			m_wheelState[(size_t)ScrollWheelDirection::WheelUp] = false;
			m_wheelState[(size_t)ScrollWheelDirection::WheelDown] = true;
		}
		else
		{
			//For possible x directions...
		}
	}

	bool InputManager::IsMouseButtonDown(MouseCode button) const
	{
		return m_mouseButtonState[(size_t)button];
	}

	bool InputManager::IsMouseButtonPressed(MouseCode button) const
	{
		return m_mouseButtonState[(size_t)button] && !m_previousMouseButtonState[(size_t)button];
	}

	bool InputManager::IsMouseButtonReleased(MouseCode button) const
	{
		return !m_mouseButtonState[(size_t)button] && m_previousMouseButtonState[(size_t)button];
	}

	bool InputManager::DidScrollInDirection(ScrollWheelDirection direction) const
	{
		return m_wheelState[(size_t)direction];
	}

	void InputManager::NextFrame()
	{
		m_previousKeyState = std::move(m_keyState);
		m_previousMouseButtonState = std::move(m_mouseButtonState);
		m_wheelState[(size_t)ScrollWheelDirection::WheelUp] = false;
		m_wheelState[(size_t)ScrollWheelDirection::WheelDown] = false;
	}

	void InputManager::HandleEvent(Event& evnt)
	{
		EventType type = evnt.GetEventType();

		switch (type)
		{
		case EventType::KeyPressed:
			{
				KeyPressedEvent* pKeyPressedEvent = static_cast<KeyPressedEvent*>(&evnt);
				SetKeyState(pKeyPressedEvent->GetKeyCode(), true);
				break;
			}
		case EventType::KeyReleased:
			{
				KeyReleasedEvent* pKeyReleasedEvent = static_cast<KeyReleasedEvent*>(&evnt);
				SetKeyState(pKeyReleasedEvent->GetKeyCode(), false);
				break;
			}
		case EventType::MouseButtonPressed:
			{
				MouseButtonPressedEvent* pMousePressedEvent = static_cast<MouseButtonPressedEvent*>(&evnt);
				SetButtonState(pMousePressedEvent->GetMouseButton(), true);
				break;
			}
		case EventType::MouseButtonReleased:
			{
				MouseButtonReleasedEvent* pMouseReleasedEvent = static_cast<MouseButtonReleasedEvent*>(&evnt);
				SetButtonState(pMouseReleasedEvent->GetMouseButton(), false);
				break;
			}
		case EventType::MouseScrolled:
			{
				MouseScrolledEvent* pMouseScrolledEvent = static_cast<MouseScrolledEvent*>(&evnt);
				pMouseScrolledEvent->GetWheelDelta();
				if (pMouseScrolledEvent->GetWheelDelta() > 0)
					SetWheelState(ScrollWheelDirection::WheelUp);
				else
					SetWheelState(ScrollWheelDirection::WheelDown);
				break;
			}
		default:
			{
				//Unhandled event.
				break;
			}
		}
	}
}