#pragma once

#include "Source/OS/Events/Event.h"
#include "Source/OS/Input/KeyCodes.h"

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
		KeyCode m_keyCode;
	public:
		KeyEvent() = delete;
		KeyEvent(const KeyEvent&) = delete;
		KeyEvent(KeyEvent&&) = delete;
		KeyEvent& operator=(const KeyEvent&) = delete;
		KeyEvent& operator=(KeyEvent&&) = delete;
		virtual ~KeyEvent() = default;

		/// <summary>
		/// Get the input key code.
		/// </summary>
		/// <returns>Key code.</returns>
		KeyCode GetKeyCode() const
		{
			return m_keyCode;
		}
		
		EVENT_CLASS_CATEGORY((int)EventCategory::Keyboard | (int)EventCategory::Input)

	protected:
		/// <summary>
		/// Event that occurs when a keyboard key event is detected.
		/// </summary>
		/// <param name="keycode">Key data.</param>
		KeyEvent(KeyCode keyCode)
			: m_keyCode(keyCode)
		{
			//
		}
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
		KeyPressedEvent(KeyCode keyCode)
			: KeyEvent(keyCode)
		{
			//
		}

		KeyPressedEvent() = delete;
		KeyPressedEvent(const KeyPressedEvent&) = delete;
		KeyPressedEvent(KeyPressedEvent&&) = delete;
		KeyPressedEvent& operator=(const KeyPressedEvent&) = delete;
		KeyPressedEvent& operator=(KeyPressedEvent&&) = delete;
		virtual ~KeyPressedEvent() = default;

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
		KeyReleasedEvent(KeyCode keyCode)
			: KeyEvent(keyCode)
		{
			//
		}

		KeyReleasedEvent() = delete;
		KeyReleasedEvent(const KeyReleasedEvent&) = delete;
		KeyReleasedEvent(KeyReleasedEvent&&) = delete;
		KeyReleasedEvent& operator=(const KeyReleasedEvent&) = delete;
		KeyReleasedEvent& operator=(KeyReleasedEvent&&) = delete;
		virtual ~KeyReleasedEvent() = default;

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
		uint32_t m_character;
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
		KeyTypedEvent(KeyCode keyCode, uint32_t characterTyped)
			: KeyEvent(keyCode)
			, m_character(characterTyped)
		{
			//
		}

		KeyTypedEvent() = delete;
		KeyTypedEvent(const KeyTypedEvent&) = delete;
		KeyTypedEvent(KeyTypedEvent&&) = delete;
		KeyTypedEvent& operator=(const KeyTypedEvent&) = delete;
		KeyTypedEvent& operator=(KeyTypedEvent&&) = delete;
		virtual ~KeyTypedEvent() = default;

		/// <summary>
		/// Get the Text character captured by this event.
		/// </summary>
		/// <returns>Text character unicode.</returns>
		uint32_t GetCharacterTyped() const
		{
			return m_character;
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}