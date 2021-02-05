#include "EXEPCH.h"
#include "SFMLWindow.h"

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

namespace Exelius
{
	Window* Window::Create(const WindowProperties& properties)
	{
		return new SFMLWindow(properties);
	}

	SFMLWindow::SFMLWindow(const WindowProperties& properties)
	{
		Initialize(properties);
	}

	SFMLWindow::~SFMLWindow()
	{
		Shutdown();
	}

	void SFMLWindow::Initialize(const WindowProperties& properties)
	{
		m_data.m_title = properties.m_title;
		m_data.m_width = properties.m_width;
		m_data.m_height = properties.m_height;

		EXE_CORE_INFO("Creating SFML Window: {0} ({1}, {2})", properties.m_title, properties.m_width, properties.m_height);

		m_pWindow = new sf::RenderWindow(sf::VideoMode(properties.m_width, properties.m_height), properties.m_title);

		if (!m_pWindow->isOpen())
		{
			EXE_CORE_ERROR("Failed to create SFML RenderWindow.");
			return;
		}

		SetVSync(true);
	}

	void SFMLWindow::Shutdown()
	{
		m_pWindow->close();
	}

	void SFMLWindow::OnUpdate()
	{
		sf::Event evnt;
		while (m_pWindow->pollEvent(evnt))
		{
			switch (evnt.type)
			{
				//----------------------------------------------------------------------------------------------------
				// Window Events
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::Closed:
				break;
			case sf::Event::EventType::Resized:
				break;
			case sf::Event::EventType::LostFocus:
				break;
			case sf::Event::EventType::GainedFocus:
				break;

				//----------------------------------------------------------------------------------------------------
				// Keyboard Events
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::TextEntered:
				break;
			case sf::Event::EventType::KeyPressed:
				break;
			case sf::Event::EventType::KeyReleased:
				break;

				//----------------------------------------------------------------------------------------------------
				// Mouse Events
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::MouseWheelScrolled:
				break;
			case sf::Event::EventType::MouseButtonPressed:
				break;
			case sf::Event::EventType::MouseButtonReleased:
				break;
			case sf::Event::EventType::MouseMoved:
				break;
			case sf::Event::EventType::MouseEntered:
				break;
			case sf::Event::EventType::MouseLeft:
				break;

				//----------------------------------------------------------------------------------------------------
				// Joystick/Gamepad Events
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::JoystickButtonPressed:
				break;
			case sf::Event::EventType::JoystickButtonReleased:
				break;
			case sf::Event::EventType::JoystickConnected:
				break;
			case sf::Event::EventType::JoystickDisconnected:
				break;
			case sf::Event::EventType::JoystickMoved:
				break;

				//----------------------------------------------------------------------------------------------------
				// Touchscreen Events
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::TouchBegan:
				break;
			case sf::Event::EventType::TouchEnded:
				break;
			case sf::Event::EventType::TouchMoved:
				break;

				//----------------------------------------------------------------------------------------------------
				// Sensor Events (Accelerometer, Gyroscope, etc)
				//----------------------------------------------------------------------------------------------------

			case sf::Event::EventType::SensorChanged:
				break;

				//----------------------------------------------------------------------------------------------------
				// End
				//----------------------------------------------------------------------------------------------------

			default:
				EXE_CORE_ERROR("This event is unidentified by SFML");
				break;
			}
		}

		m_pWindow->display();
	}

	void SFMLWindow::SetVSync(bool enabled)
	{
		m_pWindow->setVerticalSyncEnabled(enabled);
		m_data.m_isVSync = enabled;
	}

	bool SFMLWindow::IsVSync() const
	{
		return m_data.m_isVSync;
	}
}
