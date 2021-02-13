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
	/// Event that occurs when a keyboard key event is detected.
	/// </summary>
	class KeyEvent
		: public Event
	{
	public:
		/// <summary>
		/// Get the input key code.
		/// </summary>
		/// <returns>Key code.</returns>
		int GetKeyCode() const
		{
			return m_keyCode;
		}
		
		EVENT_CLASS_CATEGORY((int)EventCategory::Keyboard | (int)EventCategory::Input)

	protected:
		/// <summary>
		/// Event that occurs when a keyboard key event is detected.
		/// </summary>
		/// <param name="keycode">Key data.</param>
		KeyEvent(int keyCode)
			: m_keyCode(keyCode)
		{
			//
		}

	private:
		int m_keyCode;
	};

	/// <summary>
	/// Event that occurs when a keyboard keypressed event is detected.
	/// Not to be confused with the KeyTyped event.
	/// </summary>
	class KeyPressedEvent
		: public KeyEvent
	{
	public:
		/// <summary>
		/// Event that occurs when a keyboard keypressed event is detected.
		/// </summary>
		/// <param name="keycode">Key data.</param>
		KeyPressedEvent(int keyCode)
			: KeyEvent(keyCode)
		{
			//
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	/// <summary>
	/// Event that occurs when a keyboard keyreleased event is detected.
	/// </summary>
	class KeyReleasedEvent
		: public KeyEvent
	{
	public:
		/// <summary>
		/// Event that occurs when a keyboard keyreleased event is detected.
		/// </summary>
		/// <param name="keycode">Key data.</param>
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode)
		{
			//
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	/// <summary>
	/// Event that occurs when a keyboard key event is detected.
	/// Not to be confused with the Keypressed event.
	/// This event should be used for specific character retrieval.
	/// this can retrieve characters from foreign keyboards.
	/// (A key pressed event will also trigger).
	/// </summary>
	class KeyTypedEvent
		: public KeyEvent
	{
	public:
		/// <summary>
		/// Event that occurs when a keyboard key event is detected.
		/// Not to be confused with the Keypressed event.
		/// This event should be used for specific character retrieval.
		/// this can retrieve characters from foreign keyboards.
		/// (A key pressed event will also trigger).
		/// </summary>
		/// <param name="keycode">Key data.</param>
		/// <param name="characterTyped">Text character unicode.</param>
		KeyTypedEvent(int keyCode, uint32_t characterTyped)
			: KeyEvent(keyCode)
			, m_character(characterTyped)
		{
			//
		}

		/// <summary>
		/// Get the Text character captured by this event.
		/// </summary>
		/// <returns>Text character unicode.</returns>
		uint32_t GetCharacterTyped() const
		{
			return m_character;
		}

		EVENT_CLASS_TYPE(KeyTyped)
	private:
		uint32_t m_character;
	};
}