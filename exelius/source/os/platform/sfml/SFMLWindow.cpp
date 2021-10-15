#include "EXEPCH.h"
#include "source/os/platform/sfml/SFMLWindow.h"
#include "source/os/platform/sfml/SFMLInputConversions.h"
#include "source/os/events/ApplicationEvents.h"
#include "source/os/events/KeyEvents.h"
#include "source/os/events/MouseEvents.h"
#include "source/os/events/JoystickEvents.h"
#include "source/os/events/TouchEvents.h"
#include "source/os/events/MiscEvents.h"
#include "source/os/interface/graphics/View.h"
#include "source/os/interface/graphics/VertexArray.h"
#include "source/os/interface/graphics/Texture.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// 
	/// </summary>
	SFMLWindow::SFMLWindow()
		: m_graphicsInterfaceLog("GraphicsInterface")
		, m_pWindow(nullptr)
		, m_isVSync(false)
	{
		m_pWindow = new sf::RenderWindow();
		EXE_ASSERT(m_pWindow);
	}

	/// <summary>
	/// The SFML window class that handles SFML specific events.
	/// This is the platform independent window class.
	/// </summary>
	/// <param name="title">The name of the window to be opened. Default: "Exelius Engine"</param>
	/// <param name="width">The width of the window to be opened. Default: 1280</param>
	/// <param name="height">The height of the window to be opened. Default: 720</param>
	SFMLWindow::SFMLWindow(const eastl::string& title, const Vector2u& windowSize)
		: m_graphicsInterfaceLog("GraphicsInterface")
		, m_pWindow(nullptr)
		, m_isVSync(false)
	{
		m_pWindow = new sf::RenderWindow(sf::VideoMode(windowSize.w, windowSize.h), title.c_str());
		EXE_ASSERT(m_pWindow);

		if (!m_pWindow->isOpen())
		{
			m_graphicsInterfaceLog.Error("Failed to create SFML RenderWindow.");
			return; // removes warning in release.
		}
	}

	/// <summary>
	/// 
	/// </summary>
	SFMLWindow::~SFMLWindow()
	{
		Shutdown();
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="title"></param>
	/// <param name="windowSize"></param>
	bool SFMLWindow::CreateWindow(const eastl::string& title, const Vector2u& windowSize)
	{
		EXE_ASSERT(m_pWindow);
		m_graphicsInterfaceLog.Info("Creating SFML Window: {0} ({1}, {2})", title.c_str(), windowSize.w, windowSize.h);

		m_pWindow->create(sf::VideoMode(windowSize.w, windowSize.h), title.c_str());

		if (!m_pWindow->isOpen())
		{
			m_graphicsInterfaceLog.Error("Failed to create SFML RenderWindow.");
			return false;
		}

		return true;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool SFMLWindow::IsOpen() const
	{
		EXE_ASSERT(m_pWindow);

		return m_pWindow->isOpen();
	}

	/// <summary>
	/// Retrieves and notifies of any SFML events.
	/// </summary>
	void SFMLWindow::Update()
	{
		EXE_ASSERT(m_pWindow);

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
					KeyTypedEvent keyTypedEvent(ConvertKeyCode(evnt.key.code), evnt.text.unicode);
					m_messenger.NotifyObservers(keyTypedEvent);
					break;
				}
				case sf::Event::EventType::KeyPressed:
				{
					KeyPressedEvent keyPressedEvent(ConvertKeyCode(evnt.key.code));
					m_messenger.NotifyObservers(keyPressedEvent);
					break;
				}
				case sf::Event::EventType::KeyReleased:
				{
					KeyReleasedEvent keyReleasedEvent(ConvertKeyCode(evnt.key.code));
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
					MouseButtonPressedEvent mouseButtonPressed(ConvertMouseCode(evnt.mouseButton.button));
					m_messenger.NotifyObservers(mouseButtonPressed);
					break;
				}
				case sf::Event::EventType::MouseButtonReleased:
				{
					MouseButtonReleasedEvent mouseButtonReleased(ConvertMouseCode(evnt.mouseButton.button));
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
					m_graphicsInterfaceLog.Error("This event is unidentified by SFML");
					break;
				}
			}
		}
	}

	Vector2i SFMLWindow::GetPosition() const
	{
		EXE_ASSERT(m_pWindow);

		Vector2i vect;
		sf::Vector2i sfmlVect = m_pWindow->getPosition();

		// TODO: Write SFML to Exelius conversion tools.
		vect.x = sfmlVect.x;
		vect.y = sfmlVect.y;
		return vect;
	}

	void SFMLWindow::SetPosition(const Vector2i& position)
	{
		EXE_ASSERT(m_pWindow);

		sf::Vector2i sfmlVect(position.x, position.y);

		m_pWindow->setPosition(sfmlVect);
	}

	Vector2u SFMLWindow::GetWindowSize() const
	{
		EXE_ASSERT(m_pWindow);

		Vector2u vect;
		sf::Vector2u sfmlVect = m_pWindow->getSize();

		// TODO: Write SFML to Exelius conversion tools.
		vect.x = sfmlVect.x;
		vect.y = sfmlVect.y;
		return vect;
	}

	void SFMLWindow::SetWindowSize(const Vector2u& windowSize)
	{
		EXE_ASSERT(m_pWindow);

		sf::Vector2u sfmlVect(windowSize.x, windowSize.y);

		m_pWindow->setSize(sfmlVect);
	}

	void SFMLWindow::SetWindowTitle(const eastl::string& title)
	{
		EXE_ASSERT(m_pWindow);

		m_pWindow->setTitle(title.c_str());
	}

	void SFMLWindow::SetIcon(const Vector2u& size, const uint8_t* pPixels)
	{
		EXE_ASSERT(m_pWindow);

		m_pWindow->setIcon(size.w, size.h, pPixels);
	}

	void SFMLWindow::SetVisible(bool isVisible)
	{
		EXE_ASSERT(m_pWindow);

		m_pWindow->setVisible(isVisible);
	}

	bool SFMLWindow::HasFocus() const
	{
		EXE_ASSERT(m_pWindow);

		return m_pWindow->hasFocus();
	}

	void SFMLWindow::Render() const
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->display();
	}

	void SFMLWindow::Draw(const VertexArray& vertices, const Texture& texture)
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->draw(vertices.GetNativeVertexArray().GetNativeVertexArray(), texture.GetNativeTexture().GetSFMLTexture());
	}

	void SFMLWindow::Draw(const VertexArray& vertices)
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->draw(vertices.GetNativeVertexArray().GetNativeVertexArray());
	}

	void SFMLWindow::Clear()
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->clear();
	}

	void SFMLWindow::SetView(const View& view)
	{
		EXE_ASSERT(m_pWindow);

		m_pWindow->setView(view.GetNativeView().GetSFMLView());
	}

	View SFMLWindow::GetView() const
	{
		EXE_ASSERT(m_pWindow);
		
		//TODO:
		// Handle this more elegantly.

		auto& sfmlView = m_pWindow->getView();

		View view;
		view.SetCenter(sfmlView.getCenter().x, sfmlView.getCenter().y);
		view.SetRotation(sfmlView.getRotation());
		view.SetSize(sfmlView.getSize().x, sfmlView.getSize().y);

		sf::FloatRect sfmlViewport = sfmlView.getViewport();
		FRectangle viewport;
		viewport.m_top = sfmlViewport.top;
		viewport.m_left = sfmlViewport.left;
		viewport.m_width = sfmlViewport.width;
		viewport.m_height = sfmlViewport.height;

		view.SetViewport(viewport);

		return view;
	}

	View SFMLWindow::GetDefaultView() const
	{
		EXE_ASSERT(m_pWindow);

		//TODO:
		// Handle this more elegantly.

		auto& sfmlView = m_pWindow->getDefaultView();

		View view;
		view.SetCenter(sfmlView.getCenter().x, sfmlView.getCenter().y);
		view.SetRotation(sfmlView.getRotation());
		view.SetSize(sfmlView.getSize().x, sfmlView.getSize().y);

		sf::FloatRect sfmlViewport = sfmlView.getViewport();
		FRectangle viewport;
		viewport.m_top = sfmlViewport.top;
		viewport.m_left = sfmlViewport.left;
		viewport.m_width = sfmlViewport.width;
		viewport.m_height = sfmlViewport.height;

		view.SetViewport(viewport);

		return view;
	}

	IRectangle SFMLWindow::GetViewport(const View& view) const
	{
		EXE_ASSERT(m_pWindow);

		//TODO:
		// Handle this more elegantly.

		sf::View sfmlView;
		sfmlView.setCenter(view.GetCenter().x, view.GetCenter().y);
		sfmlView.setRotation(view.GetRotation());
		sfmlView.setSize(view.GetSize().x, view.GetSize().y);

		FRectangle rect = view.GetViewport();
		sf::FloatRect sfmlRect;
		sfmlRect.top = view.GetViewport().m_top;
		sfmlRect.left = view.GetViewport().m_left;
		sfmlRect.width = view.GetViewport().m_width;
		sfmlRect.height = view.GetViewport().m_height;

		sfmlView.setViewport(sfmlRect);

		sf::IntRect sfmlViewport = m_pWindow->getViewport(sfmlView);

		IRectangle viewport;
		viewport.m_top = sfmlViewport.top;
		viewport.m_left = sfmlViewport.left;
		viewport.m_width = sfmlViewport.width;
		viewport.m_height = sfmlViewport.height;

		return viewport;
	}

	Vector2f SFMLWindow::MapPixelToCoords(const Vector2i& point) const
	{
		EXE_ASSERT(m_pWindow);

		sf::Vector2i sfmlVect(point.x, point.y);

		auto sfmlCoordVect = m_pWindow->mapPixelToCoords(sfmlVect);

		Vector2f coordVect(sfmlCoordVect.x, sfmlCoordVect.y);

		return coordVect;
	}

	Vector2f SFMLWindow::MapPixelToCoords(const Vector2i& point, const View& view) const
	{
		EXE_ASSERT(m_pWindow);

		sf::Vector2i sfmlVect(point.x, point.y);

		sf::View sfmlView;
		sfmlView.setCenter(view.GetCenter().x, view.GetCenter().y);
		sfmlView.setRotation(view.GetRotation());
		sfmlView.setSize(view.GetSize().x, view.GetSize().y);

		FRectangle rect = view.GetViewport();
		sf::FloatRect sfmlRect;
		sfmlRect.top = view.GetViewport().m_top;
		sfmlRect.left = view.GetViewport().m_left;
		sfmlRect.width = view.GetViewport().m_width;
		sfmlRect.height = view.GetViewport().m_height;

		sfmlView.setViewport(sfmlRect);

		auto sfmlCoordVect = m_pWindow->mapPixelToCoords(sfmlVect, sfmlView);

		Vector2f coordVect(sfmlCoordVect.x, sfmlCoordVect.y);

		return coordVect;
	}

	Vector2i SFMLWindow::MapCoordsToPixel(const Vector2f& point) const
	{
		EXE_ASSERT(m_pWindow);

		sf::Vector2f sfmlVect(point.x, point.y);

		auto sfmlPixelVect = m_pWindow->mapCoordsToPixel(sfmlVect);

		Vector2i pixelVect(sfmlPixelVect.x, sfmlPixelVect.y);

		return Vector2i();
	}

	Vector2i SFMLWindow::MapCoordsToPixel(const Vector2f& point, const View& view) const
	{
		EXE_ASSERT(m_pWindow);

		sf::Vector2f sfmlVect(point.x, point.y);

		sf::View sfmlView;
		sfmlView.setCenter(view.GetCenter().x, view.GetCenter().y);
		sfmlView.setRotation(view.GetRotation());
		sfmlView.setSize(view.GetSize().x, view.GetSize().y);

		FRectangle rect = view.GetViewport();
		sf::FloatRect sfmlRect;
		sfmlRect.top = view.GetViewport().m_top;
		sfmlRect.left = view.GetViewport().m_left;
		sfmlRect.width = view.GetViewport().m_width;
		sfmlRect.height = view.GetViewport().m_height;

		sfmlView.setViewport(sfmlRect);

		auto sfmlPixelVect = m_pWindow->mapCoordsToPixel(sfmlVect, sfmlView);

		Vector2i pixelVect(sfmlPixelVect.x, sfmlPixelVect.y);

		return pixelVect;
	}

	void SFMLWindow::PushGLStates()
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->pushGLStates();
	}

	void SFMLWindow::PopGLStates()
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->popGLStates();
	}

	void SFMLWindow::ResetGLStates()
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->resetGLStates();
	}

	/// <summary>
	/// Sets VSync for this window.
	/// </summary>
	/// <param name="enabled">Whether to turn VSync on or off.</param>
	void SFMLWindow::SetVSync(bool isEnabled)
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->setVerticalSyncEnabled(isEnabled);
		m_isVSync = isEnabled;
	}

	/// <summary>
	/// Returns the VSync state of this window.
	/// </summary>
	/// <returns>Whether VSync is currently on or off.</returns>
	bool SFMLWindow::IsVSync() const
	{
		EXE_ASSERT(m_pWindow);
		return m_isVSync;
	}

	void SFMLWindow::SetMouseCursorVisible(bool isVisible)
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->setMouseCursorVisible(isVisible);
	}

	void SFMLWindow::SetMouseCursorGrabbed(bool isGrabbed)
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->setMouseCursorGrabbed(isGrabbed);
	}

	void SFMLWindow::SetKeyRepeatEnabled(bool isEnabled)
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->setKeyRepeatEnabled(isEnabled);
	}

	void SFMLWindow::SetFramerateLimit(unsigned int framerateLimit)
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->setFramerateLimit(framerateLimit);
	}

	void SFMLWindow::SetJoystickThreshold(float threshold)
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->setJoystickThreshold(threshold);
	}

	bool SFMLWindow::SetActive(bool isActive) const
	{
		EXE_ASSERT(m_pWindow);
		return m_pWindow->setActive(isActive);
	}

	void SFMLWindow::RequestFocus()
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->requestFocus();
	}

	/// <summary>
	/// Call any code required to shut down a window in SFML.
	/// </summary>
	void SFMLWindow::Shutdown()
	{
		if (!m_pWindow)
			return;

		m_pWindow->setActive(true);

		m_pWindow->close();
		delete m_pWindow;
		m_pWindow = nullptr;
	}
}
