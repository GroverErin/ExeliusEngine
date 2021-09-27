#pragma once
#include "source/os/input/KeyCodes.h"
#include "source/os/input/MouseCodes.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	Exelius::KeyCode ConvertKeyCode(sf::Keyboard::Key key);
	Exelius::MouseCode ConvertMouseCode(sf::Mouse::Button button);
}