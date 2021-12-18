#pragma once

#include "source/utility/generic/Singleton.h"
#include "source/os/events/EventManagement.h"

#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Log;
	class ResourceFactory;
	class ComponentFactory;
	class MessageFactory;
	class ConfigFile;

	/// <summary>
	/// The application class is to be inhereted by the client,
	/// it defines the running application and holds the main loop.
	/// </summary>
	class Application
		: public Singleton<Application>, public OSEventObserver
	{
		Log* m_pApplicationLog; // Needs to be pointer because it is created after LogManager Init();

		ResourceFactory* m_pResourceFactory;
		ComponentFactory* m_pComponentFactory;
		MessageFactory* m_pMessageFactory;

		float m_lastFrameTime;
		bool m_isRunning;
		bool m_hasLostFocus;

	public:
		/// <summary>
		/// Clients must inheret from this class in order to hook into the engine.
		/// This class contains the main loop and provides functions for the client
		/// to inject into the main loop.
		/// </summary>
		Application();
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;
		virtual ~Application();

		/// <summary>
		/// Pre-Initialize the engine, this will spin up the default logging system to be used during initialization.
		/// </summary>
		/// <returns>True on success, false on failure.</returns>
		bool PreInitializeExelius();

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
		/// Sets the resource factory to use when creating Resources.
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
		/// Sets the message factory to use when creating Messages.
		/// 
		/// NOTE:
		///		This will override any engine specific Messages unless
		///		the client's defined MessageFactory inherets from
		///		ExeliusMessageFactory and calls it's CreateMessage()
		///		function.
		/// 
		///	Example:
		///		case default:
		///			return Exelius::ExeliusMessageFactory::CreateMessage();
		/// </summary>
		virtual void SetMessageFactory();

		/// <summary>
		/// Client Update Loop Hook. Clients are required to define this function
		/// to access the update loop of the engine.
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

	private:
		
		/// <summary>
		/// Initialize the LogManager using the config file data if necessary.
		/// </summary>
		/// <param name="configFile">- The pre-parsed config file.</param>
		/// <returns>True on success, false otherwise.</returns>
		bool InitializeLogManager(const ConfigFile& configFile) const;

		/// <summary>
		/// Initialize the RenderManager using the config file data if necessary.
		/// </summary>
		/// <param name="configFile">- The pre-parsed config file.</param>
		/// <returns>True on success, false otherwise.</returns>
		bool InitializeRenderManager(const ConfigFile& configFile) const;

		/// <summary>
		/// Initialize the InputManager using the config file data if necessary.
		/// </summary>
		/// <param name="configFile">- The pre-parsed config file.</param>
		/// <returns>True on success, false otherwise.</returns>
		bool InitializeInputManager(const ConfigFile& configFile) const;

		/// <summary>
		/// Initialize the ResourceLoader using the config file data if necessary.
		/// </summary>
		/// <param name="configFile">- The pre-parsed config file.</param>
		/// <returns>True on success, false otherwise.</returns>
		bool InitializeResourceLoader(const ConfigFile& configFile) const;

		/// <summary>
		/// Initialize the GameObject System using the config file data if necessary.
		/// </summary>
		/// <param name="configFile">- The pre-parsed config file.</param>
		/// <returns>True on success, false otherwise.</returns>
		bool InitializeGameObjectSystem(const ConfigFile& configFile) const;
	};

	/// <summary>
	/// Allocates the Application Subclass as defined by the client.
	/// This should be defined in the client.
	/// @see Entrypoint.h
	/// </summary>
	/// <returns>The Application Subclass as Defined by the client.</returns>
	Application* CreateApplication();
}