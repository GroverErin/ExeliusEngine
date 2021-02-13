#include "EXEPCH.h"
#include "SFMLWindow.h"

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "Source/OS/Events/ApplicationEvents.h"
#include "Source/OS/Events/KeyEvents.h"
#include "Source/OS/Events/MouseEvents.h"
#include "Source/OS/Events/JoystickEvents.h"
#include "Source/OS/Events/TouchEvents.h"
#include "Source/OS/Events/MiscEvents.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// The SFML window class that handles SFML specific events.
	/// This is the platform independent window class.
	/// </summary>
	/// <param name="title">The name of the window to be opened. Default: "Exelius Engine"</param>
	/// <param name="width">The width of the window to be opened. Default: 1280</param>
	/// <param name="height">The height of the window to be opened. Default: 720</param>
	SFMLWindow::SFMLWindow(const std::string& title, unsigned int width, unsigned int height)
		: m_pWindow(nullptr)
		, m_title(title)
		, m_width(width)
		, m_height(height)
	{
		Initialize();
	}

	SFMLWindow::~SFMLWindow()
	{
		Shutdown();
	}

	/// <summary>
	/// Retrieves and notifies of any SFML events.
	/// </summary>
	void SFMLWindow::OnUpdate()
	{
		assert(m_pWindow);

		sf::Event evnt;
		while (m_pWindow->pollEvent(evnt))
		{
			switch (evnt.type)
			{
				//----------------------------------------------------------------------------------------------------
				// Window Events
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::Closed:
			{
				WindowClosedEvent windowClosedEvent;
				m_messenger.NotifyObservers(windowClosedEvent);
				break;
			}
			case sf::Event::EventType::Resized:
			{
				WindowResizedEvent windowResizedEvent(evnt.size.width, evnt.size.height);
				m_messenger.NotifyObservers(windowResizedEvent);
				break;
			}
			case sf::Event::EventType::LostFocus:
			{
				WindowLostFocusEvent windowLostFocusEvent;
				m_messenger.NotifyObservers(windowLostFocusEvent);
				break;
			}
			case sf::Event::EventType::GainedFocus:
			{
				WindowGainedFocusEvent windowGainedFocusEvent;
				m_messenger.NotifyObservers(windowGainedFocusEvent);
				break;
			}

				//----------------------------------------------------------------------------------------------------
				// Keyboard Events
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::TextEntered:
			{
				KeyTypedEvent keyTypedEvent(evnt.key.code, evnt.text.unicode);
				m_messenger.NotifyObservers(keyTypedEvent);
				break;
			}
			case sf::Event::EventType::KeyPressed:
			{
				KeyPressedEvent keyPressedEvent(evnt.key.code);
				m_messenger.NotifyObservers(keyPressedEvent);
				break;
			}
			case sf::Event::EventType::KeyReleased:
			{
				KeyReleasedEvent keyReleasedEvent(evnt.key.code);
				m_messenger.NotifyObservers(keyReleasedEvent);
				break;
			}

				//----------------------------------------------------------------------------------------------------
				// Mouse Events
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::MouseWheelScrolled:
			{
				MouseScrolledEvent windowScrolledEvent(evnt.mouseWheelScroll.delta);
				m_messenger.NotifyObservers(windowScrolledEvent);
				break;
			}
			case sf::Event::EventType::MouseButtonPressed:
			{
				MouseButtonPressedEvent mouseButtonPressed(evnt.mouseButton.button);
				m_messenger.NotifyObservers(mouseButtonPressed);
				break;
			}
			case sf::Event::EventType::MouseButtonReleased:
			{
				MouseButtonReleasedEvent mouseButtonReleased(evnt.mouseButton.button);;
				m_messenger.NotifyObservers(mouseButtonReleased);
				break;
			}
			case sf::Event::EventType::MouseMoved:
			{
				MouseMovedEvent mouseMovedEvent(evnt.mouseMove.x, evnt.mouseMove.y);
				m_messenger.NotifyObservers(mouseMovedEvent);
				break;
			}
			case sf::Event::EventType::MouseEntered:
			{
				MouseGainedFocusEvent mouseGainedFocusEvent;
				m_messenger.NotifyObservers(mouseGainedFocusEvent);
				break;
			}
			case sf::Event::EventType::MouseLeft:
			{
				MouseLostFocusEvent mouseLostFocusEvent;
				m_messenger.NotifyObservers(mouseLostFocusEvent);
				break;
			}

				//----------------------------------------------------------------------------------------------------
				// Joystick/Gamepad Events
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::JoystickButtonPressed:
			{
				JoystickButtonPressedEvent joyButtonPressed(evnt.joystickButton.joystickId, evnt.joystickButton.button);
				m_messenger.NotifyObservers(joyButtonPressed);
				break;
			}
			case sf::Event::EventType::JoystickButtonReleased:
			{
				JoystickButtonPressedEvent joyButtonReleased(evnt.joystickButton.joystickId, evnt.joystickButton.button);
				m_messenger.NotifyObservers(joyButtonReleased);
				break;
			}
			case sf::Event::EventType::JoystickConnected:
			{
				JoystickConnectedEvent joyConnected(evnt.joystickConnect.joystickId);
				m_messenger.NotifyObservers(joyConnected);
				break;
			}
			case sf::Event::EventType::JoystickDisconnected:
			{
				JoystickDisconnectedEvent joyDisconnected(evnt.joystickConnect.joystickId);
				m_messenger.NotifyObservers(joyDisconnected);
				break;
			}
			case sf::Event::EventType::JoystickMoved:
			{
				JoystickMovedEvent joystickMovedEvent(evnt.joystickMove.joystickId, evnt.joystickMove.position);

				// There is probably a better way to handle this here.
				switch (evnt.joystickMove.axis)
				{
				case sf::Joystick::Axis::X:
					joystickMovedEvent.SetAxis(JoystickAxis::X);
					break;
				case sf::Joystick::Axis::Y:
					joystickMovedEvent.SetAxis(JoystickAxis::Y);
					break;
				case sf::Joystick::Axis::Z:
					joystickMovedEvent.SetAxis(JoystickAxis::Z);
					break;
				case sf::Joystick::Axis::R:
					joystickMovedEvent.SetAxis(JoystickAxis::R);
					break;
				case sf::Joystick::Axis::U:
					joystickMovedEvent.SetAxis(JoystickAxis::U);
					break;
				case sf::Joystick::Axis::V:
					joystickMovedEvent.SetAxis(JoystickAxis::V);
					break;
				case sf::Joystick::Axis::PovX:
					joystickMovedEvent.SetAxis(JoystickAxis::PovX);
					break;
				case sf::Joystick::Axis::PovY:
					joystickMovedEvent.SetAxis(JoystickAxis::PovY);
					break;
				}

				m_messenger.NotifyObservers(joystickMovedEvent);
				break;
			}

				//----------------------------------------------------------------------------------------------------
				// Touchscreen Events
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::TouchBegan:
			{
				TouchBeganEvent touchBegan(evnt.touch.finger, evnt.touch.x, evnt.touch.y);
				m_messenger.NotifyObservers(touchBegan);
				break;
			}
			case sf::Event::EventType::TouchEnded:
			{
				TouchEndedEvent touchEnded(evnt.touch.finger, evnt.touch.x, evnt.touch.y);
				m_messenger.NotifyObservers(touchEnded);
				break;
			}
			case sf::Event::EventType::TouchMoved:
			{
				TouchMovedEvent touchMoved(evnt.touch.finger, evnt.touch.x, evnt.touch.y);
				m_messenger.NotifyObservers(touchMoved);
				break;
			}

				//----------------------------------------------------------------------------------------------------
				// Sensor Events (Accelerometer, Gyroscope, etc)
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::SensorChanged:
			{
				SensorEvent sensorEvent(evnt.sensor.x, evnt.sensor.y, evnt.sensor.z);
				m_messenger.NotifyObservers(sensorEvent);
				break;
			}

				//----------------------------------------------------------------------------------------------------
				// End
				//----------------------------------------------------------------------------------------------------

			default:
			{
				EXE_ENGINE_ERROR("This event is unidentified by SFML");
				break;
			}
			}
		}

		m_pWindow->display();
	}

	/// <summary>
	/// Sets VSync for this window.
	/// </summary>
	/// <param name="enabled">Whether to turn VSync on or off.</param>
	void SFMLWindow::SetVSync(bool enabled)
	{
		assert(m_pWindow);
		m_pWindow->setVerticalSyncEnabled(enabled);
		m_isVSync = enabled;
	}

	/// <summary>
	/// Returns the VSync state of this window.
	/// </summary>
	/// <returns>Whether VSync is currently on or off.</returns>
	bool SFMLWindow::IsVSync() const
	{
		return m_isVSync;
	}

	/// <summary>
	/// Call any SFML initialization code.
	/// </summary>
	void SFMLWindow::Initialize()
	{
		EXE_ENGINE_INFO("Creating SFML Window: {0} ({1}, {2})", m_title, m_width, m_height);

		m_pWindow = new sf::RenderWindow(sf::VideoMode(m_width, m_height), m_title);

		if (!m_pWindow->isOpen())
		{
			EXE_ENGINE_ERROR("Failed to create SFML RenderWindow.");
			return;
		}

		SetVSync(true);
	}

	/// <summary>
	/// Call any code required to shut down a window in SFML.
	/// </summary>
	void SFMLWindow::Shutdown()
	{
		assert(m_pWindow);
		m_pWindow->close();
	}
}
