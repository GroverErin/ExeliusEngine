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
	/// Event that occurs when a touchscreen event is detected.
	/// This event will not be triggered on Windows.
	/// </summary>
	class TouchEvent
		: public Event
	{
		unsigned int m_fingerIndex;
		int m_touchX;
		int m_touchY;
	public:

		/// <summary>
		/// Get the index of the finger in the case of multi-touch.
		/// </summary>
		/// <returns>Finger index data.</returns>
		unsigned int GetFingerIndex() const
		{
			return m_fingerIndex;
		}

		/// <summary>
		/// Get the X data of the touch event.
		/// </summary>
		/// <returns>X position data.</returns>
		int GetX() const
		{
			return m_touchX;
		}

		/// <summary>
		/// Get the Y data of the touch event.
		/// </summary>
		/// <returns>Y position data.</returns>
		int GetY() const
		{
			return m_touchY;
		}

		EVENT_CLASS_CATEGORY((int)EventCategory::Touch | (int)EventCategory::Input)

	protected:
		
		/// <summary>
		/// Event that occurs when a touchscreen event is detected.
		/// This event will not be triggered on Windows.
		/// </summary>
		/// <param name="fingerIndex">Finger index data.</param>
		/// <param name="xPos">X position data.</param>
		/// <param name="yPos">Y position data.</param>
		TouchEvent(unsigned int fingerIndex, int xPos, int yPos)
			: m_fingerIndex(fingerIndex)
			, m_touchX(xPos)
			, m_touchY(yPos)
		{
			//
		}
	};

	class TouchMovedEvent
		: public TouchEvent
	{
	public:
		/// <summary>
		/// Event that occurs when a touchscreen move event is detected.
		/// </summary>
		/// <param name="fingerIndex">Finger index data.</param>
		/// <param name="xPos">X position data.</param>
		/// <param name="yPos">Y position data.</param>
		TouchMovedEvent(unsigned int fingerIndex, int xPos, int yPos)
			: TouchEvent(fingerIndex, xPos, yPos)
		{
			//
		}

		EVENT_CLASS_TYPE(TouchMoved)
	};

	/// <summary>
	/// Event that occurs when a touch began event is detected.
	/// </summary>
	class TouchBeganEvent
		: public TouchEvent
	{
	public:
		/// <summary>
		/// Event that occurs when a touch began event is detected.
		/// </summary>
		/// <param name="fingerIndex">Finger index data.</param>
		/// <param name="xPos">X position data.</param>
		/// <param name="yPos">Y position data.</param>
		TouchBeganEvent(unsigned int fingerIndex, int xPos, int yPos)
			: TouchEvent(fingerIndex, xPos, yPos)
		{
			//
		}

		EVENT_CLASS_TYPE(TouchBegan)
	};

	/// <summary>
	/// Event that occurs when a touch ended event is detected.
	/// </summary>
	class TouchEndedEvent
		: public TouchEvent
	{
	public:
		/// <summary>
		/// Event that occurs when a touch ended event is detected.
		/// </summary>
		/// <param name="fingerIndex">Finger index data.</param>
		/// <param name="xPos">X position data.</param>
		/// <param name="yPos">Y position data.</param>
		TouchEndedEvent(unsigned int fingerIndex, int xPos, int yPos)
			: TouchEvent(fingerIndex, xPos, yPos)
		{
			//
		}

		EVENT_CLASS_TYPE(TouchEnded)
	};
}