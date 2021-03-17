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
	/// Joystick Axis that the engine can manage.
	/// Currently these mimic the axis available from SFML
	/// </summary>
	enum class JoystickAxis
	{
		INVALID,
		X,    // The X axis
		Y,    // The Y axis
		Z,    // The Z axis
		R,    // The R axis
		U,    // The U axis
		V,    // The V axis
		PovX, // The X axis of the point-of-view hat
		PovY  // The Y axis of the point-of-view hat
	};

	/// <summary>
	/// Event that occurs when a Joystick event is detected.
	/// </summary>
	class JoystickEvent
		: public Event
	{
		unsigned int m_joystickID;
	public:
		JoystickEvent() = delete;
		JoystickEvent(const JoystickEvent&) = delete;
		JoystickEvent(JoystickEvent&&) = delete;
		JoystickEvent& operator=(const JoystickEvent&) = delete;
		JoystickEvent& operator=(JoystickEvent&&) = delete;
		virtual ~JoystickEvent() = default;

		/// <summary>
		/// Get the unique identifier of the triggering joystick.
		/// </summary>
		/// <returns>Joystick ID.</returns>
		unsigned int GetJoystickID() const
		{
			return m_joystickID;
		}

	protected:
		/// <summary>
		/// Event that occurs when a Joystick event is detected.
		/// </summary>
		/// <param name="joystickID">Joystick ID.</param>
		JoystickEvent(int joystickID)
			: m_joystickID(joystickID)
		{
			//
		}
	};

	/// <summary>
	/// Event that occurs when a Joystick axis is moved.
	/// </summary>
	class JoystickMovedEvent
		: public JoystickEvent
	{
		float m_position;
		JoystickAxis m_axis;
	public:
		/// <summary>
		/// Event that occurs when a Joystick axis is moved.
		/// </summary>
		/// <param name="joystickID">Joystick ID.</param>
		/// <param name="position">Joystick position data.</param>
		/// <param name="axis">Triggering Axis.</param>
		JoystickMovedEvent(const unsigned int joystickID, const float position, JoystickAxis axis = JoystickAxis::INVALID)
			: JoystickEvent(joystickID)
			, m_position(position)
			, m_axis(axis)
		{
			//
		}

		JoystickMovedEvent() = delete;
		JoystickMovedEvent(const JoystickMovedEvent&) = delete;
		JoystickMovedEvent(JoystickMovedEvent&&) = delete;
		JoystickMovedEvent& operator=(const JoystickMovedEvent&) = delete;
		JoystickMovedEvent& operator=(JoystickMovedEvent&&) = delete;
		virtual ~JoystickMovedEvent() = default;

		/// <summary>
		/// Get the triggering axis' positional data.
		/// </summary>
		/// <returns>Joystick position data.</returns>
		float GetPosition() const
		{
			return m_position;
		}

		/// <summary>
		/// Set the axis that is being triggered.
		/// This is currently not an ideal setup,
		/// It may be better to rework this into
		/// the constructor of this event.
		/// </summary>
		/// <param name="axis">Triggering Axis.</param>
		void SetAxis(const JoystickAxis axis)
		{
			m_axis = axis;
		}

		/// <summary>
		/// Get the triggering axis.
		/// </summary>
		/// <returns>Triggering Axis.</returns>
		JoystickAxis GetAxis() const
		{
			return m_axis;
		}

		EVENT_CLASS_TYPE(JoystickMoved)
		EVENT_CLASS_CATEGORY((int)EventCategory::Joystick | (int)EventCategory::Input)
	};

	/// <summary>
	/// Event that occurs when a Joystick button is pressed.
	/// </summary>
	class JoystickButtonPressedEvent
		: public JoystickEvent
	{
		unsigned int m_button;
	public:
		/// <summary>
		/// Event that occurs when a Joystick button is pressed.
		/// </summary>
		/// <param name="joystickID">Joystick ID.</param>
		/// <param name="button">Button code.</param>
		JoystickButtonPressedEvent(const unsigned int joystickID, const unsigned int button)
			: JoystickEvent(joystickID)
			, m_button(button)
		{
			//
		}

		JoystickButtonPressedEvent() = delete;
		JoystickButtonPressedEvent(const JoystickButtonPressedEvent&) = delete;
		JoystickButtonPressedEvent(JoystickButtonPressedEvent&&) = delete;
		JoystickButtonPressedEvent& operator=(const JoystickButtonPressedEvent&) = delete;
		JoystickButtonPressedEvent& operator=(JoystickButtonPressedEvent&&) = delete;
		virtual ~JoystickButtonPressedEvent() = default;

		EVENT_CLASS_TYPE(JoystickButtonPressed)
		EVENT_CLASS_CATEGORY((int)EventCategory::Joystick | (int)EventCategory::Input | (int)EventCategory::JoystickButton)
	};

	/// <summary>
	/// Event that occurs when a Joystick button is released.
	/// </summary>
	class JoystickButtonReleasedEvent
		: public JoystickEvent
	{
		unsigned int m_button;
	public:
		/// <summary>
		/// Event that occurs when a Joystick button is released.
		/// </summary>
		/// <param name="joystickID">Joystick ID.</param>
		/// <param name="button">Button code.</param>
		JoystickButtonReleasedEvent(const unsigned int joystickID, const unsigned int button)
			: JoystickEvent(joystickID)
			, m_button(button)
		{
			//
		}

		JoystickButtonReleasedEvent() = delete;
		JoystickButtonReleasedEvent(const JoystickButtonReleasedEvent&) = delete;
		JoystickButtonReleasedEvent(JoystickButtonReleasedEvent&&) = delete;
		JoystickButtonReleasedEvent& operator=(const JoystickButtonReleasedEvent&) = delete;
		JoystickButtonReleasedEvent& operator=(JoystickButtonReleasedEvent&&) = delete;
		virtual ~JoystickButtonReleasedEvent() = default;

		EVENT_CLASS_TYPE(JoystickButtonReleased)
		EVENT_CLASS_CATEGORY((int)EventCategory::Joystick | (int)EventCategory::Input | (int)EventCategory::JoystickButton)
	};

	/// <summary>
	/// Event that occurs when a Joystick is connected.
	/// </summary>
	class JoystickConnectedEvent
		: public JoystickEvent
	{
	public:
		/// <summary>
		/// Event that occurs when a Joystick is connected.
		/// </summary>
		/// <param name="joystickID">Joystick ID.</param>
		JoystickConnectedEvent(const unsigned int joystickID)
			: JoystickEvent(joystickID)
		{
			//
		}

		JoystickConnectedEvent() = delete;
		JoystickConnectedEvent(const JoystickConnectedEvent&) = delete;
		JoystickConnectedEvent(JoystickConnectedEvent&&) = delete;
		JoystickConnectedEvent& operator=(const JoystickConnectedEvent&) = delete;
		JoystickConnectedEvent& operator=(JoystickConnectedEvent&&) = delete;
		virtual ~JoystickConnectedEvent() = default;

		EVENT_CLASS_TYPE(JoystickConnected)
		EVENT_CLASS_CATEGORY((int)EventCategory::Joystick | (int)EventCategory::Input)
	};

	/// <summary>
	/// Event that occurs when a Joystick is disconnected.
	/// </summary>
	class JoystickDisconnectedEvent
		: public JoystickEvent
	{
	public:
		/// <summary>
		/// Event that occurs when a Joystick is disconnected.
		/// </summary>
		/// <param name="joystickID">Joystick ID.</param>
		JoystickDisconnectedEvent(const unsigned int joystickID)
			: JoystickEvent(joystickID)
		{
			//
		}

		JoystickDisconnectedEvent() = delete;
		JoystickDisconnectedEvent(const JoystickDisconnectedEvent&) = delete;
		JoystickDisconnectedEvent(JoystickDisconnectedEvent&&) = delete;
		JoystickDisconnectedEvent& operator=(const JoystickDisconnectedEvent&) = delete;
		JoystickDisconnectedEvent& operator=(JoystickDisconnectedEvent&&) = delete;
		virtual ~JoystickDisconnectedEvent() = default;

		EVENT_CLASS_TYPE(JoystickDisconnected)
		EVENT_CLASS_CATEGORY((int)EventCategory::Joystick | (int)EventCategory::Input)
	};
}