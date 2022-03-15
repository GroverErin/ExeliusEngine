#include "EXEPCH.h"
#include "include/Input.h"

#include "source/os/input/InputManager.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	bool IsKeyDown(KeyCode key)
	{
		EXE_ASSERT(InputManager::GetInstance());
		return InputManager::GetInstance()->IsKeyDown(key);
	}

	bool IsKeyPressed(KeyCode key)
	{
		EXE_ASSERT(InputManager::GetInstance());
		return InputManager::GetInstance()->IsKeyPressed(key);
	}

	bool IsKeyReleased(KeyCode key)
	{
		EXE_ASSERT(InputManager::GetInstance());
		return InputManager::GetInstance()->IsKeyReleased(key);
	}

	bool IsMouseButtonDown(MouseCode button)
	{
		EXE_ASSERT(InputManager::GetInstance());
		return InputManager::GetInstance()->IsMouseButtonDown(button);
	}

	bool IsMouseButtonPressed(MouseCode button)
	{
		EXE_ASSERT(InputManager::GetInstance());
		return InputManager::GetInstance()->IsMouseButtonPressed(button);
	}

	bool IsMouseButtonReleased(MouseCode button)
	{
		EXE_ASSERT(InputManager::GetInstance());
		return InputManager::GetInstance()->IsMouseButtonReleased(button);
	}
	float GetMouseX()
	{
		EXE_ASSERT(InputManager::GetInstance());
		return InputManager::GetInstance()->GetMouseX();
	}
	float GetMouseY()
	{
		EXE_ASSERT(InputManager::GetInstance());
		return InputManager::GetInstance()->GetMouseY();
	}
}