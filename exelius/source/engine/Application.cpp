#include "EXEPCH.h"

#include "source/engine/Application.h"
#include "source/engine/resources/ExeliusResourceFactory.h"
#include "source/engine/gameobjectsystem/GameObjectSystem.h"
#include "source/engine/gameobjectsystem/components/ExeliusComponentFactory.h"

#include "source/engine/settings/ConfigFile.h"

#include "source/debug/LogManager.h"

#include "source/render/RenderManager.h"

#include "source/resource/ResourceLoader.h"

#include "source/os/input/InputManager.h"
#include "source/os/events/ApplicationEvents.h"
#include "source/os/interface/graphics/Window.h"

#include "source/utility/generic/Time.h"

// TEST
#include "source/os/memory/PoolAllocator.h"

class PoolTester
{
	int testInt;
public:
	PoolTester() : testInt(0) { }
	void* operator new(size_t size);
};

static auto& GetPool()
{
	static Exelius::PoolAllocator<sizeof(PoolTester), 4096> m_pool;
	return m_pool;
}

void* PoolTester::operator new(size_t size) { void* p = GetPool().Allocate(size, 0, "PoolTester", 0); return p; }

// END TEST

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
		: m_pApplicationLog(nullptr)
		, m_pResourceFactory(nullptr)
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

		ResourceLoader::DestroySingleton();

		EXELIUS_DELETE(m_pApplicationLog);

		MemoryManager::GetInstance()->GetGlobalAllocator()->DumpMemoryData();

		LogManager::DestroySingleton();

		MemoryManager::DestroySingleton();
	}

	bool Application::PreInitializeExelius()
	{
		// Should be the only call to "new" inside any Exelius code.
		MemoryManager::SetSingleton(new MemoryManager());
		EXE_ASSERT(MemoryManager::GetInstance());
		MemoryManager::GetInstance()->Initialize(true);

		LogManager::SetSingleton(new LogManager());
		EXE_ASSERT(LogManager::GetInstance());
		if (!LogManager::GetInstance()->PreInitialize())
		{
			// Failed to pre-initialize LogManager
			EXE_ASSERT(false);
			return false;
		}

		return true;
	}

	bool Application::InitializeExelius()
	{
		// We instantiate this log here so that the config file warn messege will appear if needed.
		m_pApplicationLog = EXELIUS_NEW(Log("Application"));
		EXE_ASSERT(m_pApplicationLog);

		//-----------------------------------------------
		// Config File - Open & Parse
		//-----------------------------------------------
		
		//PoolTester* pTest = new PoolTester();                                                                 <== Test code... to be removed

		// Read in the config file. This uses the logging system,
		// which is why the PreInit exists for the LoggingManager.
		ConfigFile configFile;
		if (!configFile.OpenConfigFile())
		{
			m_pApplicationLog->Warn("Failed to open config file. Exelius will initialize with default settings.");
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

		if (!InitializeResourceLoader(configFile))
			return false;

		//-----------------------------------------------
		// Component Factory - Initialization
		//-----------------------------------------------
		
		// This will call either the default (Exelius)
		// version, or the client's if defined.
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
		EXE_ASSERT(m_pApplicationLog);

		constexpr int kNumFramesToAVG = 6000;
		int numFramesSinceAVG = 0;
		float accumulatedDeltaTime = 0.0f;

		//Log log;
		auto previousTime = eastl::chrono::high_resolution_clock::now();
		while (m_isRunning)
		{
			auto lastFrameTime = eastl::chrono::high_resolution_clock::now();
			eastl::chrono::duration<float> deltaTime = lastFrameTime - previousTime;
			previousTime = lastFrameTime;
			Time::DeltaTime.SetFromSeconds(deltaTime.count());

			if (numFramesSinceAVG < kNumFramesToAVG)
			{
				accumulatedDeltaTime += Time::DeltaTime.GetAsSeconds();
				++numFramesSinceAVG;
			}
			else
			{
				float avgFrameRate = accumulatedDeltaTime / (float)kNumFramesToAVG;
				m_pApplicationLog->Info("FPS: {}", 1.0f / avgFrameRate);
				numFramesSinceAVG = 0;
				accumulatedDeltaTime = 0.0f;
			}

			// Poll Window Events.
			RenderManager::GetInstance()->Update();

			// Update Components.
			GameObjectSystem::GetInstance()->Update();

			// Client Update.
			Update();

			// Refresh Input State.
			InputManager::GetInstance()->NextFrame();

			// Queue all renderable Components.
			GameObjectSystem::GetInstance()->Render();

			// Push render list to render thread.
			RenderManager::GetInstance()->EndRenderFrame();

			// Deallocate any resources necessary.
			ResourceLoader::GetInstance()->ProcessUnloadQueue();
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
		EXE_ASSERT(m_pApplicationLog);

		FileLogDefinition fileDefinition;
		ConsoleLogDefinition consoleDefinition;
		eastl::vector<LogData> logData;

		if (!configFile.PopulateLogData(fileDefinition, consoleDefinition, logData))
		{
			m_pApplicationLog->Warn("Failed to populate log data correctly. Please verify config file.");
		}

		if (!LogManager::GetInstance()->Initialize(fileDefinition, consoleDefinition, logData))
		{
			m_pApplicationLog->Fatal("Exelius::LogManager::Initialize Failed.");
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
		EXE_ASSERT(m_pApplicationLog);

		eastl::string windowTitle("ExeliusApplication");
		Vector2u windowSize({ 720, 640 });
		bool isVSyncEnabled = false;

		if (!configFile.PopulateWindowData(windowTitle, windowSize, isVSyncEnabled))
		{
			m_pApplicationLog->Warn("Failed to populate window data correctly. Please verify config file.");
		}

		RenderManager::SetSingleton(new RenderManager());
		EXE_ASSERT(RenderManager::GetInstance());
		if (!RenderManager::GetInstance()->Initialize(windowTitle, windowSize, isVSyncEnabled))
		{
			m_pApplicationLog->Fatal("Exelius::RenderManager failed to initialize.");
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
		EXE_ASSERT(m_pApplicationLog);

		InputManager::SetSingleton(new InputManager());
		EXE_ASSERT(InputManager::GetInstance());
		if (!InputManager::GetInstance()->Initialize())
		{
			m_pApplicationLog->Fatal("Exelius::InputManager failed to initialize.");
			return false;
		}

		// Attach the event observer to the window to handle window events.
		// TODO: I don't like this very much.
		RenderManager::GetInstance()->GetWindow()->GetEventMessenger().AddObserver(*InputManager::GetInstance());

		return true;
	}

	/// <summary>
	/// Initialize the ResourceLoader using the config file data if necessary.
	/// </summary>
	/// <param name="configFile">- The pre-parsed config file.</param>
	/// <returns>True on success, false otherwise.</returns>
	bool Application::InitializeResourceLoader([[maybe_unused]] const ConfigFile& configFile) const
	{
		EXE_ASSERT(m_pApplicationLog);

		// Create Resource Manager Singleton.
		ResourceLoader::SetSingleton(new ResourceLoader());
		EXE_ASSERT(ResourceLoader::GetInstance());
		if (!ResourceLoader::GetInstance()->Initialize(m_pResourceFactory, "EngineResources/", true))
		{
			m_pApplicationLog->Fatal("Exelius::ResourceLoader failed to initialize.");
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
		EXE_ASSERT(m_pApplicationLog);

		GameObjectSystem::SetSingleton(new GameObjectSystem());
		EXE_ASSERT(GameObjectSystem::GetInstance());
		if (!GameObjectSystem::GetInstance()->Initialize(m_pComponentFactory))
		{
			m_pApplicationLog->Fatal("Exelius::GameObjectSystem failed to initialize.");
			return false;
		}

		return true;
	}
}