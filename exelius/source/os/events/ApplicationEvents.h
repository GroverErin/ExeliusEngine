#pragma once

#include "source/os/events/Event.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Event that occurs when the window is resized.
	/// </summary>
	class WindowResizedEvent
		: public Event
	{
		unsigned int m_width;
		unsigned int m_height;
	public:
		/// <summary>
		/// Event that occurs when the window is resized.
		/// </summary>
		WindowResizedEvent(unsigned int width, unsigned int height)
			: m_width(width), m_height(height)
		{
			//
		}
		WindowResizedEvent() = delete;
		WindowResizedEvent(const WindowResizedEvent&) = delete;
		WindowResizedEvent(WindowResizedEvent&&) = delete;
		WindowResizedEvent& operator=(const WindowResizedEvent&) = delete;
		WindowResizedEvent& operator=(WindowResizedEvent&&) = delete;
		virtual ~WindowResizedEvent() = default;

		/// <summary>
		/// Get the width data of the window.
		/// </summary>
		/// <returns>Window width data.</returns>
		unsigned int GetWidth() const
		{
			return m_width;
		}

		/// <summary>
		/// Get the height data of the window.
		/// </summary>
		/// <returns>Window height data.</returns>
		unsigned int GetHeight() const
		{
			return m_height;
		}

		EVENT_CLASS_TYPE(WindowResized)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	/// <summary>
	/// Event that occurs when the window is moved.
	/// </summary>
	class WindowMovedEvent
		: public Event
	{
		unsigned int m_x;
		unsigned int m_y;
	public:
		/// <summary>
		/// Event that occurs when the window is resized.
		/// </summary>
		WindowMovedEvent(unsigned int x, unsigned int y)
			: m_x(x), m_y(y)
		{
			//
		}
		WindowMovedEvent() = delete;
		WindowMovedEvent(const WindowMovedEvent&) = delete;
		WindowMovedEvent(WindowMovedEvent&&) = delete;
		WindowMovedEvent& operator=(const WindowMovedEvent&) = delete;
		WindowMovedEvent& operator=(WindowMovedEvent&&) = delete;
		virtual ~WindowMovedEvent() = default;

		/// <summary>
		/// Get the x data of the window.
		/// </summary>
		/// <returns>Window x data.</returns>
		unsigned int GetX() const
		{
			return m_x;
		}

		/// <summary>
		/// Get the y data of the window.
		/// </summary>
		/// <returns>Window y data.</returns>
		unsigned int GetY() const
		{
			return m_y;
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	/// <summary>
	/// Event that occurs when the window is closed.
	/// </summary>
	class WindowClosedEvent
		: public Event
	{
	public:
		WindowClosedEvent() = default;
		WindowClosedEvent(const WindowClosedEvent&) = delete;
		WindowClosedEvent(WindowClosedEvent&&) = delete;
		WindowClosedEvent& operator=(const WindowClosedEvent&) = delete;
		WindowClosedEvent& operator=(WindowClosedEvent&&) = delete;
		virtual ~WindowClosedEvent() = default;

		EVENT_CLASS_TYPE(WindowClosed)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	/// <summary>
	/// Event that occurs when the window is refocused.
	/// </summary>
	class WindowGainedFocusEvent
		: public Event
	{
	public:
		WindowGainedFocusEvent() = default;
		WindowGainedFocusEvent(const WindowGainedFocusEvent&) = delete;
		WindowGainedFocusEvent(WindowGainedFocusEvent&&) = delete;
		WindowGainedFocusEvent& operator=(const WindowGainedFocusEvent&) = delete;
		WindowGainedFocusEvent& operator=(WindowGainedFocusEvent&&) = delete;
		virtual ~WindowGainedFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowGainedFocus)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};

	/// <summary>
	/// Event that occurs when the window is unfocused or minimized.
	/// </summary>
	class WindowLostFocusEvent
		: public Event
	{
	public:
		WindowLostFocusEvent() = default;
		WindowLostFocusEvent(const WindowLostFocusEvent&) = delete;
		WindowLostFocusEvent(WindowLostFocusEvent&&) = delete;
		WindowLostFocusEvent& operator=(const WindowLostFocusEvent&) = delete;
		WindowLostFocusEvent& operator=(WindowLostFocusEvent&&) = delete;
		virtual ~WindowLostFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(EventCategory::Application)
	};
}