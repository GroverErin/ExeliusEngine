#pragma once

#include "Source/OS/Events/Event.h"
#include "Source/OS/Input/MouseCodes.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Event that occurs when a mouse movement event is detected.
	/// </summary>
	class MouseMovedEvent
		: public Event
	{
		int m_mouseX;
		int m_mouseY;
	public:
		/// <summary>
		/// Event that occurs when a mouse movement event is detected.
		/// </summary>
		/// <param name="x">X position data.</param>
		/// <param name="y">Y position data.</param>
		MouseMovedEvent(const int x, const int y)
			: m_mouseX(x)
			, m_mouseY(y)
		{
			//
		}

		MouseMovedEvent() = delete;
		MouseMovedEvent(const MouseMovedEvent&) = delete;
		MouseMovedEvent(MouseMovedEvent&&) = delete;
		MouseMovedEvent& operator=(const MouseMovedEvent&) = delete;
		MouseMovedEvent& operator=(MouseMovedEvent&&) = delete;
		virtual ~MouseMovedEvent() = default;

		/// <summary>
		/// Get the x position data.
		/// </summary>
		/// <returns>X position data.</returns>
		int GetX() const
		{
			return m_mouseX;
		}

		/// <summary>
		/// Get the Y position data.
		/// </summary>
		/// <returns>Y position data.</returns>
		int GetY() const
		{
			return m_mouseY;
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY((int)EventCategory::Mouse | (int)EventCategory::Input)
	};

	/// <summary>
	/// Event that occurs when a mouse moves from
	/// off-screen to on-screen.
	/// </summary>
	class MouseGainedFocusEvent
		: public Event
	{
	public:
		/// <summary>
		/// Event that occurs when a mouse moves from
		/// off-screen to on-screen.
		/// </summary>
		MouseGainedFocusEvent() = default;
		MouseGainedFocusEvent(const MouseGainedFocusEvent&) = delete;
		MouseGainedFocusEvent(MouseGainedFocusEvent&&) = delete;
		MouseGainedFocusEvent& operator=(const MouseGainedFocusEvent&) = delete;
		MouseGainedFocusEvent& operator=(MouseGainedFocusEvent&&) = delete;
		virtual ~MouseGainedFocusEvent() = default;

		EVENT_CLASS_TYPE(MouseGainedFocus)
		EVENT_CLASS_CATEGORY((int)EventCategory::Mouse | (int)EventCategory::Input)
	};

	/// <summary>
	/// Event that occurs when a mouse moves from
	/// on-screen to off-screen.
	/// </summary>
	class MouseLostFocusEvent
		: public Event
	{
	public:
		/// <summary>
		/// Event that occurs when a mouse moves from
		/// on-screen to off-screen.
		/// </summary>
		MouseLostFocusEvent() = default;
		MouseLostFocusEvent(const MouseLostFocusEvent&) = delete;
		MouseLostFocusEvent(MouseLostFocusEvent&&) = delete;
		MouseLostFocusEvent& operator=(const MouseLostFocusEvent&) = delete;
		MouseLostFocusEvent& operator=(MouseLostFocusEvent&&) = delete;
		virtual ~MouseLostFocusEvent() = default;

		EVENT_CLASS_TYPE(MouseLostFocus)
		EVENT_CLASS_CATEGORY((int)EventCategory::Mouse | (int)EventCategory::Input)
	};

	/// <summary>
	/// Event that occurs when a mouse scroll wheel move is detected.
	/// </summary>
	class MouseScrolledEvent
		: public Event
	{
		float m_delta;
	public:
		/// <summary>
		/// Event that occurs when a mouse scroll wheel move is detected.
		/// </summary>
		/// <param name="delta">Scroll change data.</param>
		MouseScrolledEvent(const float delta)
			: m_delta(delta)
		{
			//
		}

		MouseScrolledEvent() = delete;
		MouseScrolledEvent(const MouseScrolledEvent&) = delete;
		MouseScrolledEvent(MouseScrolledEvent&&) = delete;
		MouseScrolledEvent& operator=(const MouseScrolledEvent&) = delete;
		MouseScrolledEvent& operator=(MouseScrolledEvent&&) = delete;
		virtual ~MouseScrolledEvent() = default;

		/// <summary>
		/// Get the positional data for the scroll wheel movement.
		/// </summary>
		/// <returns>Scroll change data.</returns>
		float GetWheelDelta() const
		{
			return m_delta;
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY((int)EventCategory::Mouse | (int)EventCategory::Input)
	};

	/// <summary>
	/// Event that occurs when a mouse button event is detected.
	/// </summary>
	class MouseButtonEvent
		: public Event
	{
		MouseCode m_button;
	public:
		MouseButtonEvent() = delete;
		MouseButtonEvent(const MouseButtonEvent&) = delete;
		MouseButtonEvent(MouseButtonEvent&&) = delete;
		MouseButtonEvent& operator=(const MouseButtonEvent&) = delete;
		MouseButtonEvent& operator=(MouseButtonEvent&&) = delete;
		virtual ~MouseButtonEvent() = default;

		/// <summary>
		/// Get the button data.
		/// </summary>
		/// <returns>Button data.</returns>
		MouseCode GetMouseButton() const
		{
			return m_button;
		}

		EVENT_CLASS_CATEGORY((int)EventCategory::Mouse | (int)EventCategory::Input | (int)EventCategory::MouseButton)

	protected:
		/// <summary>
		/// Event that occurs when a mouse button event is detected.
		/// </summary>
		/// <param name="button">Button data.</param>
		MouseButtonEvent(MouseCode button)
			: m_button(button)
		{
			//
		}
	};

	/// <summary>
	/// Event that occurs when a mouse button pressed event is detected.
	/// </summary>
	class MouseButtonPressedEvent
		: public MouseButtonEvent
	{
	public:
		/// <summary>
		/// Event that occurs when a mouse button pressed event is detected.
		/// </summary>
		/// <param name="button">Button data.</param>
		MouseButtonPressedEvent(MouseCode button)
			: MouseButtonEvent(button)
		{
			//
		}

		MouseButtonPressedEvent() = delete;
		MouseButtonPressedEvent(const MouseButtonPressedEvent&) = delete;
		MouseButtonPressedEvent(MouseButtonPressedEvent&&) = delete;
		MouseButtonPressedEvent& operator=(const MouseButtonPressedEvent&) = delete;
		MouseButtonPressedEvent& operator=(MouseButtonPressedEvent&&) = delete;
		virtual ~MouseButtonPressedEvent() = default;

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	/// <summary>
	/// Event that occurs when a mouse button released event is detected.
	/// </summary>
	class MouseButtonReleasedEvent
		: public MouseButtonEvent
	{
	public:
		/// <summary>
		/// Event that occurs when a mouse button released event is detected.
		/// </summary>
		/// <param name="button">Button data.</param>
		MouseButtonReleasedEvent(MouseCode button)
			: MouseButtonEvent(button)
		{
			//
		}

		MouseButtonReleasedEvent() = delete;
		MouseButtonReleasedEvent(const MouseButtonReleasedEvent&) = delete;
		MouseButtonReleasedEvent(MouseButtonReleasedEvent&&) = delete;
		MouseButtonReleasedEvent& operator=(const MouseButtonReleasedEvent&) = delete;
		MouseButtonReleasedEvent& operator=(MouseButtonReleasedEvent&&) = delete;
		virtual ~MouseButtonReleasedEvent() = default;

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}