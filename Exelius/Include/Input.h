#pragma once
#include "Source/OS/Input/MouseCodes.h"
#include "Source/OS/Input/KeyCodes.h"

namespace Exelius
{
	bool IsKeyDown(KeyCode key);

	bool IsKeyPressed(KeyCode key);

	bool IsKeyReleased(KeyCode key);

	bool IsMouseButtonDown(MouseCode button);

	bool IsMouseButtonPressed(MouseCode button);

	bool IsMouseButtonReleased(MouseCode button);
}