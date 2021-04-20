#include "EXEPCH.h"
#include "Include/Input.h"

#include "Source/OS/Input/InputManager.h"

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
}