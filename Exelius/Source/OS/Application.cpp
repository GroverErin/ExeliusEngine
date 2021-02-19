#include "EXEPCH.h"

#include "Source/OS/Application.h"
#include "Source/OS/Events/ApplicationEvents.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Clients must inheret from this class in order to hook into the engine.
	/// This class contains the main loop and provides functions for the client
	/// to inject into the main loop.
	/// </summary>
	/// <param name="title">The name of the window to be opened. Default: "Exelius Engine"</param>
	/// <param name="width">The width of the window to be opened. Default: 1280</param>
	/// <param name="height">The height of the window to be opened. Default: 720</param>
	Application::Application(const eastl::string& title, unsigned int width, unsigned int height)
		: m_window(title, width, height)
		, m_lastFrameTime(0.0f)
		, m_isRunning(true)
		, m_hasLostFocus(false)
	{
		assert(!s_pAppInstance);
		s_pAppInstance = this;

		m_window.GetEventMessenger().AddObserver(*this);
	}

	Application::~Application()
	{
		m_window.GetEventMessenger().RemoveObserver(*this);
	}

	/// <summary>
	/// Applications main loop.
	/// Not inheretable, the client can't change this loop.
	/// </summary>
	void Application::Run()
	{
		auto previousTime = eastl::chrono::high_resolution_clock::now();
		while (m_isRunning)
		{
			auto time = eastl::chrono::high_resolution_clock::now();
			eastl::chrono::duration<float> deltaTime = time - previousTime;
			m_window.OnUpdate();
		}
	}

	/// <summary>
	/// OS event handler. Handles the events that the platform sends.
	/// This function should not be inhereted, engine determins how OS events are handled.
	/// Example: SFML pollevent().
	/// </summary>
	/// <param name="evnt">The event to handle.</param>
	void Application::HandleEvent(Event& evnt)
	{
		//WindowClosedEvent* pWinClosed = dynamic_cast<WindowClosedEvent*>(&evnt);

		if (evnt.GetEventType() == EventType::WindowClosed)
		{
			CloseApplication();
		}
	}

	/// <summary>
	/// Close the application.
	/// </summary>
	void Application::CloseApplication()
	{
		m_isRunning = false;
	}
}