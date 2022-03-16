#include "EXEPCH.h"

#include "source/engine/Application.h"

#include "source/engine/settings/ConfigFile.h"

#include "source/engine/resources/ExeliusResourceFactory.h"

#include "source/engine/gameobjectsystem/GameObjectSystem.h"

#include "source/engine/gameobjectsystem/components/ExeliusComponentFactory.h"

#include "source/messages/MessageServer.h"

#include "source/messages/exeliusmessages/ExeliusMessageFactory.h"

#include "source/networking/NetworkingManager.h"

#include "source/debug/LogManager.h"

#include "source/os/threads/JobSystem.h"

#include "source/resource/ResourceLoader.h"

#include "source/render/Renderer.h"

#include "source/os/input/InputManager.h"

#include "source/engine/layers/Layer.h"

#include "source/os/events/ApplicationEvents.h"

#include "source/utility/generic/Time.h"

// ---------------------------------------------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------------------------------------------

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
		, m_pMessageFactory(nullptr)
		, m_pImGuiLayer(nullptr)
		, m_lastFrameTime(0.0f)
		, m_isRunning(true)
		, m_hasLostFocus(false)
	{
		//
	}

	Application::~Application()
	{
		GameObjectSystem::DestroySingleton();

		EXELIUS_DELETE(m_pComponentFactory);
		EXELIUS_DELETE(m_pResourceFactory);
		EXELIUS_DELETE(m_pMessageFactory);

		EXE_ASSERT(m_pImGuiLayer);
		m_layerStack.PopOverlayLayer(m_pImGuiLayer);
		EXELIUS_DELETE(m_pImGuiLayer);

		Renderer::GetInstance()->GetWindow().GetEventMessenger().RemoveObserver(*InputManager::GetInstance());
		InputManager::DestroySingleton();

		Renderer::GetInstance()->GetWindow().GetEventMessenger().RemoveObserver(*this);
		Renderer::DestroySingleton();

		ResourceLoader::DestroySingleton();

		EXELIUS_DELETE(m_pApplicationLog);

		NetworkingManager::DestroySingleton();

		EXELIUS_DELETE(s_pGlobalJobSystem);

		MessageServer::DestroySingleton();

		LogManager::DestroySingleton();

		StringIntern::_ClearStringInternSet();

		MemoryManager::GetInstance()->GetGlobalAllocator()->DumpMemoryData();

		MemoryManager::DestroySingleton();
	}

	bool Application::PreInitializeExelius()
	{
		// Should be the only call to "new" inside any Exelius code.
		MemoryManager::SetSingleton(new MemoryManager());
		EXE_ASSERT(MemoryManager::GetInstance());
		MemoryManager::GetInstance()->Initialize(true);

		LogManager::SetSingleton(EXELIUS_NEW(LogManager()));
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
		
		//PoolTester* pTest = new PoolTester();																				<== Test code... to be removed

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
		// Messaging - Initialization
		//-----------------------------------------------

		MessageServer::SetSingleton(EXELIUS_NEW(MessageServer()));
		EXE_ASSERT(MessageServer::GetInstance());

		//-----------------------------------------------
		// Job System - Initialization
		//-----------------------------------------------

		if (!s_pGlobalJobSystem)
			s_pGlobalJobSystem = EXELIUS_NEW(JobSystem());
		s_pGlobalJobSystem->Initialize();

		//-----------------------------------------------
		// Networking System - Initialization
		//-----------------------------------------------

		SetMessageFactory();
		EXE_ASSERT(m_pMessageFactory);

		NetworkingManager::SetSingleton(EXELIUS_NEW(NetworkingManager()));
		EXE_ASSERT(NetworkingManager::GetInstance());
		if (!NetworkingManager::GetInstance()->Initialize(m_pMessageFactory))
			return false;

		//-----------------------------------------------
		// Rendering - Initialization
		//-----------------------------------------------

		if (!InitializeRenderManager(configFile))
			return false;

		// TODO: This SUCKS
		Renderer::GetInstance()->GetWindow().GetEventMessenger().AddObserver(*this);

		//-----------------------------------------------
		// Dear ImGui Layer - Initialization
		//-----------------------------------------------

		m_pImGuiLayer = EXELIUS_NEW(ImGuiLayer());
		m_layerStack.PushOverlayLayer(m_pImGuiLayer);

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
			if (!m_hasLostFocus)
			{
				auto lastFrameTime = eastl::chrono::high_resolution_clock::now();
				eastl::chrono::duration<float> deltaTime = lastFrameTime - previousTime;
				previousTime = lastFrameTime;
				Time::DeltaTime.SetFromSeconds(deltaTime.count());

				if (numFramesSinceAVG < kNumFramesToAVG)
				{
					accumulatedDeltaTime += Time::DeltaTime;
					++numFramesSinceAVG;
				}
				else
				{
					float avgFrameRate = accumulatedDeltaTime / (float)kNumFramesToAVG;
					m_pApplicationLog->Info("FPS: {}", 1.0f / avgFrameRate);
					numFramesSinceAVG = 0;
					accumulatedDeltaTime = 0.0f;
				}

				// Dispatch Messages
				MessageServer::GetInstance()->DispatchMessages();

				// Update Components.
				GameObjectSystem::GetInstance()->Update();

				// Handle Layers.
				for (Layer* pLayer : m_layerStack)
					pLayer->OnUpdate();

				for (Layer* pLayer : m_layerStack)
					pLayer->OnRender();

				// Refresh Input State.
				InputManager::GetInstance()->NextFrame();

				// Queue all renderable Components.
				GameObjectSystem::GetInstance()->Render();

				// Deallocate any resources necessary.
				ResourceLoader::GetInstance()->ProcessUnloadQueue();
			}

			// TODO: Move to render thread?
			EXE_ASSERT(m_pImGuiLayer);
			m_pImGuiLayer->Begin();
			{
				for (Layer* pLayer : m_layerStack)
					pLayer->OnImGuiRender();
			}
			m_pImGuiLayer->End();

			// Poll Window Events
			Renderer::GetInstance()->Update();
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
		m_pComponentFactory = EXELIUS_NEW(ExeliusComponentFactory());
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
		m_pResourceFactory = EXELIUS_NEW(ExeliusResourceFactory());
	}

	void Application::PushLayer(Layer* pLayer)
	{
		m_layerStack.PushLayer(pLayer);
	}

	void Application::PushOverlayLayer(Layer* pOverlayLayer)
	{
		m_layerStack.PushOverlayLayer(pOverlayLayer);
	}

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
	void Application::SetMessageFactory()
	{
		m_pMessageFactory = EXELIUS_NEW(ExeliusMessageFactory());
	}

	/// <summary>
	/// OS event handler. Handles the events that the platform sends.
	/// This function should not be inhereted, engine determines how OS events are handled.
	/// Example: SFML pollevent().
	/// </summary>
	/// <param name="evnt">The event to handle.</param>
	void Application::HandleEvent(Event& evnt)
	{
		if (evnt.GetEventType() == EventType::WindowClosed)
		{
			//WindowClosedEvent* pWinClosed = static_cast<WindowClosedEvent*>(&evnt);
			CloseApplication();
			evnt.m_isHandled = true;
		}
		else if (evnt.GetEventType() == EventType::WindowResized)
		{
			WindowResizedEvent* pWinResized = static_cast<WindowResizedEvent*>(&evnt);
			if (pWinResized->GetWidth() == 0 || pWinResized->GetHeight() == 0)
			{
				m_hasLostFocus = true;
				return;
			}

			m_hasLostFocus = false;
			Renderer::GetInstance()->OnWindowResize(pWinResized->GetWidth(), pWinResized->GetHeight());
		}

		for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
		{
			if (evnt.m_isHandled)
				break;
			(*it)->OnEvent(evnt);
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

		Renderer::SetSingleton(EXELIUS_NEW(Renderer(windowTitle, windowSize)));
		if (!Renderer::GetInstance())
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

		InputManager::SetSingleton(EXELIUS_NEW(InputManager()));
		EXE_ASSERT(InputManager::GetInstance());
		if (!InputManager::GetInstance()->Initialize())
		{
			m_pApplicationLog->Fatal("Exelius::InputManager failed to initialize.");
			return false;
		}

		// Attach the event observer to the window to handle window events.
		// TODO: I don't like this very much.
		Renderer::GetInstance()->GetWindow().GetEventMessenger().AddObserver(*InputManager::GetInstance());

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
		ResourceLoader::SetSingleton(EXELIUS_NEW(ResourceLoader()));
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

		GameObjectSystem::SetSingleton(EXELIUS_NEW(GameObjectSystem()));
		EXE_ASSERT(GameObjectSystem::GetInstance());
		if (!GameObjectSystem::GetInstance()->Initialize(m_pComponentFactory))
		{
			m_pApplicationLog->Fatal("Exelius::GameObjectSystem failed to initialize.");
			return false;
		}

		return true;
	}
}