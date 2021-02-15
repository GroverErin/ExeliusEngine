#pragma once
#include "Source/Utility/Macros.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// EventType that is handled by Exelius.
	/// </summary>
	enum class EventType
	{
		None = 0,
		WindowClosed, WindowResized, WindowGainedFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled, MouseGainedFocus, MouseLostFocus,
		JoystickButtonPressed, JoystickButtonReleased, JoystickMoved, JoystickConnected, JoystickDisconnected,
		TouchBegan, TouchEnded, TouchMoved,
		Misc
	};

	/// <summary>
	/// Category that events are a part of.
	/// </summary>
	enum class EventCategory
	{
		None			= 0,
		Application		= BIT(0),
		Input			= BIT(1),
		Keyboard		= BIT(2),
		Mouse			= BIT(3),
		MouseButton		= BIT(4),
		Joystick		= BIT(5),
		JoystickButton	= BIT(6),
		Touch			= BIT(7),
		Misc			= BIT(8)
	};

	/// <summary>
	/// OS event triggered by the platform window.
	/// </summary>
	class Event
	{
		bool m_handled;
	public:
		/// <summary>
		/// OS event triggered by the platform window.
		/// </summary>
		Event()
			: m_handled(false)
		{
			//
		}

		virtual ~Event() = default;

		/// <summary>
		/// Get the type of event this event is.
		/// </summary>
		/// <returns>The event type.</returns>
		virtual EventType GetEventType() const = 0;

		/// <summary>
		/// Get the categories that this event is in.
		/// </summary>
		/// <returns>The event categories.</returns>
		virtual int GetCategoryFlags() const = 0;

		/// <summary>
		/// Get the text name of this event.
		/// </summary>
		/// <returns>Event name.</returns>
		virtual const char* GetName() const = 0;

		/// <summary>
		/// Check if this event is in a category.
		/// </summary>
		/// <param name="category">Category to check.</param>
		/// <returns>True if in given category, false if not.</returns>
		bool IsInCategory(const EventCategory category)
		{
			return (int)GetCategoryFlags() & (int)category;
		}

		/// <summary>
		/// Check if the event has been handled already or not.
		/// Some events may be allowed to propagate down.
		/// </summary>
		/// <returns>Event handled state.</returns>
		bool IsEventHandled() const { return m_handled; }

		/// <summary>
		/// Set the event to be handled. (No longer propagates)
		/// </summary>
		void SetEventHandled() { m_handled = true; }
	};

	/// <summary>
	/// Sets some of the basic functions required for a base inheretor of the Event class.
	/// </summary>
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const final override { return GetStaticType(); }\
								virtual const char* GetName() const final override { return #type; }

	/// <summary>
	/// Return the categories this event belongs to. Set at Compile Time.
	/// </summary>
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const final override { return (int)category; }
}