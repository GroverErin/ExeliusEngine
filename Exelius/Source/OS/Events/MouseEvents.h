#pragma once

#include "Source/OS/Events/Event.h"

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

	private:
		int m_mouseX;
		int m_mouseY;
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
		MouseGainedFocusEvent()
		{
			//
		}

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
		MouseLostFocusEvent()
		{
			//
		}

		EVENT_CLASS_TYPE(MouseLostFocus)
		EVENT_CLASS_CATEGORY((int)EventCategory::Mouse | (int)EventCategory::Input)
	};

	/// <summary>
	/// Event that occurs when a mouse scroll wheel move is detected.
	/// </summary>
	class MouseScrolledEvent
		: public Event
	{
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

	private:
		float m_delta;
	};

	/// <summary>
	/// Event that occurs when a mouse button event is detected.
	/// </summary>
	class MouseButtonEvent
		: public Event
	{
	public:
		/// <summary>
		/// Get the button data.
		/// </summary>
		/// <returns>Button data.</returns>
		int GetMouseButton() const
		{
			return m_button;
		}

		EVENT_CLASS_CATEGORY((int)EventCategory::Mouse | (int)EventCategory::Input | (int)EventCategory::MouseButton)

	protected:
		/// <summary>
		/// Event that occurs when a mouse button event is detected.
		/// </summary>
		/// <param name="button">Button data.</param>
		MouseButtonEvent(const int button)
			: m_button(button)
		{
			//
		}

	private:
		int m_button;
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
		MouseButtonPressedEvent(const int button)
			: MouseButtonEvent(button)
		{
			//
		}

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
		MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent(button)
		{
			//
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}