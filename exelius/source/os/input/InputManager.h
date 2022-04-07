#pragma once
#include "source/utility/generic/Singleton.h"
#include "source/os/events/EventManagement.h"
#include "source/os/input/MouseCodes.h"
#include "source/os/input/KeyCodes.h"

#include <EASTL/array.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class InputManager
		: public Singleton<InputManager>, public OSEventObserver
	{
		eastl::array<bool, static_cast<size_t>(KeyCode::kCount)> m_keyState;
		eastl::array<bool, static_cast<size_t>(KeyCode::kCount)> m_previousKeyState;

		eastl::array<bool, static_cast<size_t>(MouseCode::kCount)> m_mouseButtonState;
		eastl::array<bool, static_cast<size_t>(MouseCode::kCount)> m_previousMouseButtonState;
		eastl::array<bool, static_cast<size_t>(ScrollWheelDirection::WheelNone)> m_wheelState;

		double m_mouseX;
		double m_mouseY;
	public:
		InputManager()
			: m_keyState({})
			, m_previousKeyState({})
			, m_mouseButtonState({})
			, m_previousMouseButtonState({})
			, m_wheelState({})
			, m_mouseX(0.0f)
			, m_mouseY(0.0f)
		{
			//
		}

		bool Initialize();

		/// <summary>
		/// Sets the input key to be up or down.
		/// </summary>
		/// <param name="key">Key to set.</param>
		/// <param name="isDown">True if down, false if up.</param>
		void SetKeyState(KeyCode key, bool isDown);

		/// <summary>
		/// Check if input key is currently down.
		/// </summary>
		/// <param name="key">Key to check.</param>
		/// <returns>True if key is down, false if key is up.</returns>
		bool IsKeyDown(KeyCode key) const;

		/// <summary>
		/// Check if input key was pressed in this frame.
		/// </summary>
		/// <param name="key">Key to check.</param>
		/// <returns>True if key was pressed, false if key was not pressed.</returns>
		bool IsKeyPressed(KeyCode key) const;

		/// <summary>
		/// Check if input key was released in this frame.
		/// </summary>
		/// <param name="key">Key to check.</param>
		/// <returns>True if key was released, false if key was not released.</returns>
		bool IsKeyReleased(KeyCode key) const;

		/// <summary>
		/// Sets the input button to be up or down.
		/// </summary>
		/// <param name="key">Button to set.</param>
		/// <param name="down">True if down, false if up.</param>
		void SetButtonState(MouseCode button, bool down);

		/// <summary>
		/// Sets the wheel movement to be up or down.
		/// </summary>
		/// <param name="key">Direction to set.</param>
		/// <param name="down">True if in that direction, false if not.</param>
		void SetWheelState(ScrollWheelDirection direction);

		/// <summary>
		/// Check if input button is currently down.
		/// </summary>
		/// <param name="key">Button to check.</param>
		/// <returns>True if button is down, false if button is up.</returns>
		bool IsMouseButtonDown(MouseCode button) const;

		/// <summary>
		/// Check if input button was pressed in this frame.
		/// </summary>
		/// <param name="key">Button to check.</param>
		/// <returns>True if button was pressed, false if button was not pressed.</returns>
		bool IsMouseButtonPressed(MouseCode button) const;

		/// <summary>
		/// Check if input button was released in this frame.
		/// </summary>
		/// <param name="key">Button to check.</param>
		/// <returns>True if button was released, false if button was not released.</returns>
		bool IsMouseButtonReleased(MouseCode button) const;

		/// <summary>
		/// Check if the mouse wheel scrolled in input direction this frame.
		/// </summary>
		/// <param name="direction">Direction to check.</param>
		/// <returns>True if scrolled in that direction, false if not.</returns>
		bool DidScrollInDirection(ScrollWheelDirection direction) const;

		double GetMouseX() const { return m_mouseX; }
		double GetMouseY() const { return m_mouseY; }

		/// <summary>
		/// Sets the previous button to the current button states.
		/// Sets the wheels movement directions to none.
		/// Sets the previous key states to the current key states.
		/// </summary>
		void NextFrame();

		/// <summary>
		/// OS event handler. Handles the events that the platform sends.
		/// This function should not be inhereted, engine determines how OS events are handled.
		/// Example: SFML pollevent().
		/// </summary>
		/// <param name="evnt">The event to handle.</param>
		virtual void HandleEvent(Event& evnt) final override;
	};
}