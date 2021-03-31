#pragma once

#include "Source/OS/Interface/Window.h"
#include "Source/Resource/ResourceManager.h"
#include "Source/Engine/Resources/ExeliusResourceFactory.h"

#include "Source/Engine/Game/GameObjects/GameObjectFactory.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// The application class is to be inhereted by the client,
	/// it defines the running application and holds the main loop.
	/// </summary>
	class Application
		: public OSEventObserver
	{
		inline static Application* s_pAppInstance = nullptr;
		Window m_window;
		ResourceManager m_resourceManager;
		ExeliusResourceFactory m_resourceFactory;
		GameObjectFactory m_gameObjectFactory;
		float m_lastFrameTime;
		bool m_isRunning;
		bool m_hasLostFocus;

	public:
		/// <summary>
		/// Clients must inheret from this class in order to hook into the engine.
		/// This class contains the main loop and provides functions for the client
		/// to inject into the main loop.
		/// </summary>
		/// <param name="title">The name of the window to be opened. Default: "Exelius Engine"</param>
		/// <param name="width">The width of the window to be opened. Default: 1280</param>
		/// <param name="height">The height of the window to be opened. Default: 720</param>
		Application(const eastl::string& title = "Exelius Engine", unsigned int width = 1280, unsigned int height = 720);
		Application() = delete;
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;
		virtual ~Application();

		/// <summary>
		/// Initialize the engine, this will pull data from a config and initialize important systems.
		/// </summary>
		/// <returns>True on success, false on failure.</returns>
		bool Initialize();

		/// <summary>
		/// Applications main loop.
		/// Not inheretable, the client can't change this loop.
		/// </summary>
		void Run();

		/// <summary>
		/// OS event handler. Handles the events that the platform sends.
		/// This function should not be inhereted, engine determins how OS events are handled.
		/// Example: SFML pollevent().
		/// </summary>
		/// <param name="evnt">The event to handle.</param>
		virtual void HandleEvent(Event& evnt) final override;

		/// <summary>
		/// Close the application.
		/// Maybe this should be protected?
		/// </summary>
		void CloseApplication();

		/// <summary>
		/// Get the single instance of the running application.
		/// </summary>
		/// <returns> Application instance singleton.</returns>
		static const Application& GetInstance() { return *s_pAppInstance; }
	};
	
	// Defined in Entrypoint.h and by the client.
	Application* CreateApplication();
}