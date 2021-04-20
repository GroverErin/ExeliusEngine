#pragma once
#include "Source/OS/Input/KeyCodes.h"
#include "Source/OS/Input/MouseCodes.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace Exelius
{
	Exelius::KeyCode ConvertKeyCode(sf::Keyboard::Key key);
	Exelius::MouseCode ConvertMouseCode(sf::Mouse::Button button);
}