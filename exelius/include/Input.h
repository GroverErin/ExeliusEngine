#pragma once
#include "source/os/input/MouseCodes.h"
#include "source/os/input/KeyCodes.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	bool IsKeyDown(KeyCode key);

	bool IsKeyPressed(KeyCode key);

	bool IsKeyReleased(KeyCode key);

	bool IsMouseButtonDown(MouseCode button);

	bool IsMouseButtonPressed(MouseCode button);

	bool IsMouseButtonReleased(MouseCode button);
}