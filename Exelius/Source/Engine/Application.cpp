#include "EXEPCH.h"

#include "Source/Engine/Application.h"
#include "Source/Engine/Resources/ExeliusResourceFactory.h"
#include "Source/Engine/Game/GameObjectSystem/GameObjectSystem.h"
#include "Source/Engine/Game/GameObjectSystem/Components/ExeliusComponentFactory.h"

#include "Source/Engine/Settings/ConfigFile.h"

#include "Source/Debugging/LogManager.h"

#include "Source/Render/RenderManager.h"

#include "Source/Resource/ResourceManager.h"

#include "Source/OS/Input/InputManager.h"
#include "Source/OS/Events/ApplicationEvents.h"
#include "Source/OS/Interface/Graphics/Window.h"

#include "Source/Utility/Generic/Time.h"

#include <iostream>

//#include "Source/Utility/Containers/Vector2.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Clients must inheret from this class in order to hook into the engine.
	/// This class contains the main loop and provides functions for the client
	/// to inject into the main loop.
	/// </summary>
	Application::Application()
		: m_pResourceFactory(nullptr)
		, m_pComponentFactory(nullptr)
		, m_lastFrameTime(0.0f)
		, m_isRunning(true)
		, m_hasLostFocus(false)
	{
		//
	}

	Application::~Application()
	{
		GameObjectSystem::DestroySingleton();

		delete m_pComponentFactory;
		m_pComponentFactory = nullptr;

		RenderManager::GetInstance()->GetWindow()->GetEventMessenger().RemoveObserver(*InputManager::GetInstance());
		InputManager::DestroySingleton();

		RenderManager::GetInstance()->GetWindow()->GetEventMessenger().RemoveObserver(*this);
		RenderManager::DestroySingleton();

		ResourceManager::DestroySingleton();

		LogManager::DestroySingleton();
	}

	bool Application::PreInitializeExelius()
	{
		LogManager::SetSingleton(new LogManager());
		EXE_ASSERT(LogManager::GetInstance());
		if (!LogManager::GetInstance()->PreInitialize())
		{
			// This should ideally be the only use of std::cout in Exelius.
			// TODO: I wouldn't really be upset if this was an assert instead...
			std::cout << "Failed to pre-initialize LogManager\n";
			return false;
		}

		return true;
	}

	bool Application::InitializeExelius()
	{
		Log defaultLog;

		//-----------------------------------------------
		// Config File - Open & Parse
		//-----------------------------------------------
		
		// Read in the config file. This uses the logging system,
		// which is why the PreInit exists for the LoggingManager.
		ConfigFile configFile;
		if (!configFile.OpenConfigFile())
		{
			defaultLog.Warn("Failed to open config file. Exelius will initialize with default settings.");
		}

		//-----------------------------------------------
		// Logging - Initialization
		//-----------------------------------------------

		if (!InitializeLogManager(configFile))
			return false;

		//-----------------------------------------------
		// Rendering - Initialization
		//-----------------------------------------------

		if (!InitializeRenderManager(configFile))
			return false;

		// TODO: This SUCKS
		RenderManager::GetInstance()->GetWindow()->GetEventMessenger().AddObserver(*this);
		//RenderManager::GetInstance()->GetWindow()->SetVSync(false);

		//-----------------------------------------------
		// Input - Initialization
		//-----------------------------------------------

		if (!InitializeInputManager(configFile))
			return false;

		//-----------------------------------------------
		// Resource Factory - Initialization
		//-----------------------------------------------
		
		SetResourceFactory();
		EXE_ASSERT(m_pResourceFactory);

		//-----------------------------------------------
		// Resource Management - Initialization
		//-----------------------------------------------

		if (!InitializeResourceManager(configFile))
			return false;

		//-----------------------------------------------
		// Component Factory - Initialization
		//-----------------------------------------------
		
		// This will call either the default (Exelius) version, or the client's if defined.
		SetComponentFactory();
		EXE_ASSERT(m_pComponentFactory);

		//-----------------------------------------------
		// Gameobject & Component System - Initialization
		//-----------------------------------------------

		if (!InitializeGameObjectSystem(configFile))
			return false;

		//-----------------------------------------------
		// Client Application - Initialization
		//-----------------------------------------------

		return Initialize();
	}

	/// <summary>
	/// Applications main loop.
	/// Not inheretable, the client can't change this loop.
	/// </summary>
	void Application::Run()
	{
		//Log log;
		auto previousTime = eastl::chrono::high_resolution_clock::now();
		while (m_isRunning)
		{
			auto time = eastl::chrono::high_resolution_clock::now();
			eastl::chrono::duration<float> deltaTime = time - previousTime;
			// log.Trace("DeltaTime: {}", deltaTime.count());

			Time::DeltaTime.SetFromSeconds(deltaTime.count());

			// Poll Window Events.
			RenderManager::GetInstance()->Update();

			// Update Components.
			GameObjectSystem::GetInstance()->Update();

			// Client Update.
			Update();

			// Queue all renderable Components.
			GameObjectSystem::GetInstance()->Render();

			// Push render list to render thread.
			RenderManager::GetInstance()->EndRenderFrame();

			previousTime = time;
		}
	}

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
	void Application::SetComponentFactory()
	{
		m_pComponentFactory = new ExeliusComponentFactory();
	}

	/// <summary>
	/// Sets the resource factory to use when creating resources.
	/// 
	/// NOTE:
	///		This will override any engine specific resources unless
	///		the client's defined ResourceFactory inherets from
	///		ExeliusResourceFactory and calls it's CreateResource()
	///		function.
	/// 
	///	Example:
	///		case default:
	///			return Exelius::ExeliusResourceFactory::CreateResource(resourceID);
	/// </summary>
	void Application::SetResourceFactory()
	{
		m_pResourceFactory = new ExeliusResourceFactory();
	}

	/// <summary>
	/// OS event handler. Handles the events that the platform sends.
	/// This function should not be inhereted, engine determins how OS events are handled.
	/// Example: SFML pollevent().
	/// </summary>
	/// <param name="evnt">The event to handle.</param>
	void Application::HandleEvent(Event& evnt)
	{
		if (evnt.GetEventType() == EventType::WindowClosed)
		{
			//WindowClosedEvent* pWinClosed = static_cast<WindowClosedEvent*>(&evnt);
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

	/// <summary>
	/// Initialize the LogManager using the config file data if necessary.
	/// </summary>
	/// <param name="configFile">- The pre-parsed config file.</param>
	/// <returns>True on success, false otherwise.</returns>
	bool Application::InitializeLogManager(const ConfigFile& configFile) const
	{
		Log defaultLog;

		FileLogDefinition fileDefinition;
		ConsoleLogDefinition consoleDefinition;
		eastl::vector<LogData> logData;

		if (!configFile.PopulateLogData(fileDefinition, consoleDefinition, logData))
		{
			defaultLog.Warn("Failed to populate log data correctly. Please verify config file.");
		}

		if (!LogManager::GetInstance()->Initialize(fileDefinition, consoleDefinition, logData))
		{
			defaultLog.Fatal("Exelius::LogManager::Initialize Failed.");
			return false;
		}

		return true;
	}

	/// <summary>
	/// Initialize the RenderManager using the config file data if necessary.
	/// </summary>
	/// <param name="configFile">- The pre-parsed config file.</param>
	/// <returns>True on success, false otherwise.</returns>
	bool Application::InitializeRenderManager(const ConfigFile& configFile) const
	{
		Log defaultLog;

		eastl::string windowTitle("ExeliusApplication");
		Vector2u windowSize({ 720, 640 });
		bool isVSyncEnabled = false;

		if (!configFile.PopulateWindowData(windowTitle, windowSize, isVSyncEnabled))
		{
			defaultLog.Warn("Failed to populate window data correctly. Please verify config file.");
		}

		RenderManager::SetSingleton(new RenderManager());
		EXE_ASSERT(RenderManager::GetInstance());
		if (!RenderManager::GetInstance()->Initialize(windowTitle, windowSize, isVSyncEnabled))
		{
			defaultLog.Fatal("Exelius::RenderManager failed to initialize.");
			return false;
		}

		return true;
	}

	/// <summary>
	/// Initialize the InputManager using the config file data if necessary.
	/// </summary>
	/// <param name="configFile">- The pre-parsed config file.</param>
	/// <returns>True on success, false otherwise.</returns>
	bool Application::InitializeInputManager([[maybe_unused]] const ConfigFile& configFile) const
	{
		Log defaultLog;

		InputManager::SetSingleton(new InputManager());
		EXE_ASSERT(InputManager::GetInstance());
		if (!InputManager::GetInstance()->Initialize())
		{
			defaultLog.Fatal("Exelius::InputManager failed to initialize.");
			return false;
		}

		// Attach the event observer to the window to handle window events.
		// TODO: I don't like this very much.
		RenderManager::GetInstance()->GetWindow()->GetEventMessenger().AddObserver(*InputManager::GetInstance());

		return true;
	}

	/// <summary>
	/// Initialize the ResourceManager using the config file data if necessary.
	/// </summary>
	/// <param name="configFile">- The pre-parsed config file.</param>
	/// <returns>True on success, false otherwise.</returns>
	bool Application::InitializeResourceManager([[maybe_unused]] const ConfigFile& configFile) const
	{
		Log defaultLog;

		// Create Resource Manager Singleton.
		ResourceManager::SetSingleton(new ResourceManager());
		EXE_ASSERT(ResourceManager::GetInstance());
		if (!ResourceManager::GetInstance()->Initialize(m_pResourceFactory, "EngineResources/", true))
		{
			defaultLog.Fatal("Exelius::ResourceManager failed to initialize.");
			return false;
		}

		return true;
	}

	/// <summary>
	/// Initialize the GameObject System using the config file data if necessary.
	/// </summary>
	/// <param name="configFile">- The pre-parsed config file.</param>
	/// <returns>True on success, false otherwise.</returns>
	bool Application::InitializeGameObjectSystem([[maybe_unused]] const ConfigFile& configFile) const
	{
		Log defaultLog;

		GameObjectSystem::SetSingleton(new GameObjectSystem());
		EXE_ASSERT(GameObjectSystem::GetInstance());
		if (!GameObjectSystem::GetInstance()->Initialize(m_pComponentFactory))
		{
			defaultLog.Fatal("Exelius::GameObjectSystem failed to initialize.");
			return false;
		}

		return true;
	}
}