#pragma once

#include "Source/Utility/Generic/Singleton.h"
#include "Source/OS/Interface/Window.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	class ResourceFactory;
	class ComponentFactory;

	/// <summary>
	/// The application class is to be inhereted by the client,
	/// it defines the running application and holds the main loop.
	/// </summary>
	class Application
		: public Singleton<Application>, public OSEventObserver
	{
		//inline static Application* s_pAppInstance = nullptr;
		Window m_window;
		ResourceFactory* m_pResourceFactory;
		ComponentFactory* m_pComponentFactory;
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
		bool InitializeExelius();

		/// <summary>
		/// Applications main loop.
		/// Not inheretable, the client can't change this loop.
		/// </summary>
		void Run();

		/// <summary>
		/// Client Initialization Hook. Clients are required to define this function
		/// to access the Initialization of their application.
		/// 
		/// NOTE:
		///		This is called AFTER the engine has initialized.
		/// 
		/// TODO: This could be better... maybe?
		/// </summary>
		virtual bool Initialize() { return true; }

		/// <summary>
		/// Sets the component factory to use when creating GameObjects
		/// and Components. 
		/// 
		/// NOTE:
		///		This will override any engine specific components unless
		///		the client's defined ComponentFactory inherets from
		///		ExeliusComponentFactory and calls it's CreateComponent()
		///		function.
		/// 
		///	Example:
		///		case default:
		///			return Exelius::ExeliusComponentFactory::CreateComponent(componentType, pOwningObject, componentData);
		/// </summary>
		virtual void SetComponentFactory();

		/// <summary>
		/// Sets the resource factory to use when creating resources.
		/// 
		/// NOTE:
		///		This will override any engine specific components unless
		///		the client's defined ResourceFactory inherets from
		///		ExeliusResourceFactory and calls it's CreateResource()
		///		function.
		/// 
		///	Example:
		///		case default:
		///			return Exelius::ExeliusResourceFactory::CreateResource(resourceID);
		/// </summary>
		virtual void SetResourceFactory();

		/// <summary>
		/// Client Update Loop Hook. Clients are required to define this function
		/// to access the update loop of the engine.
		/// 
		/// TODO: This could be better... maybe?
		/// </summary>
		virtual void Update() {}

		/// <summary>
		/// OS event handler. Handles the events that the platform sends.
		/// This function should not be inhereted, engine determines how OS events are handled.
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
		//static const Application& GetInstance() { return *s_pAppInstance; }

		// TEMP
		Window& GetWindow() { return m_window; }
	};
	
	// Defined in Entrypoint.h and by the client.
	Application* CreateApplication();
}