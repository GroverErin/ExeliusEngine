#pragma once
#include "source/os/input/KeyCodes.h"
#include "source/os/input/MouseCodes.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	Exelius::KeyCode ConvertKeyCode(int key);
	Exelius::MouseCode ConvertMouseCode(int button);
}