#include "EXEPCH.h"

#include "source/engine/Application.h"

#include "source/engine/settings/ConfigFile.h"

#include "source/engine/resources/ExeliusResourceFactory.h"

#include "source/messages/MessageServer.h"

#include "source/messages/exeliusmessages/ExeliusMessageFactory.h"

#include "source/networking/NetworkingManager.h"

#include "source/debug/LogManager.h"

#include "source/os/threads/JobSystem.h"

#include "source/resource/ResourceLoader.h"

#include "source/engine/renderer/Renderer2D.h"

#include "source/os/input/InputManager.h"

#include "source/engine/layers/Layer.h"

#include "source/engine/layers/LayerStack.h"

#include "source/os/events/ApplicationEvents.h"

#include "source/utility/generic/Timing.h"
#include "include/Time.h"

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
		: m_pResourceFactory(nullptr)
		, m_pMessageFactory(nullptr)
		, m_pLayerStack(nullptr)
		, m_pImGuiLayer(nullptr)
		, m_lastFrameTime(0.0f)
		, m_isRunning(true)
		, m_hasLostFocus(false)
	{
		//
	}

	Application::~Application()
	{
		EXELIUS_DELETE(m_pResourceFactory);
		EXELIUS_DELETE(m_pMessageFactory);

		EXE_ASSERT(m_pImGuiLayer);
		m_pLayerStack->PopOverlayLayer(m_pImGuiLayer);
		EXELIUS_DELETE(m_pImGuiLayer);

		EXELIUS_DELETE(m_pLayerStack);

		Renderer2D::GetInstance()->GetWindow().GetEventMessenger().RemoveObserver(*InputManager::GetInstance());
		InputManager::DestroySingleton();

		Renderer2D::GetInstance()->GetWindow().GetEventMessenger().RemoveObserver(*this);
		Renderer2D::DestroySingleton();

		ResourceLoader::DestroySingleton();

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
		//-----------------------------------------------
		// Config File - Open & Parse
		//-----------------------------------------------
		
		//PoolTester* pTest = new PoolTester();																				<== Test code... to be removed

		// Read in the config file. This uses the logging system,
		// which is why the PreInit exists for the LoggingManager.
		ConfigFile configFile;
		if (!configFile.OpenConfigFile())
		{
			EXE_LOG_CATEGORY_WARN("Application", "Failed to open config file. Exelius will initialize with default settings.");
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

		/*NetworkingManager::SetSingleton(EXELIUS_NEW(NetworkingManager()));
		EXE_ASSERT(NetworkingManager::GetInstance());
		if (!NetworkingManager::GetInstance()->Initialize(m_pMessageFactory))
			return false;*/

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
		// Rendering - Initialization
		//-----------------------------------------------

		if (!InitializeRenderManager(configFile))
			return false;

		// TODO: This SUCKS
		Renderer2D::GetInstance()->GetWindow().GetEventMessenger().AddObserver(*this);

		//-----------------------------------------------
		// Dear ImGui Layer - Initialization
		//-----------------------------------------------

		m_pLayerStack = EXELIUS_NEW(LayerStack());

		m_pImGuiLayer = EXELIUS_NEW(ImGuiLayer());
		m_pLayerStack->PushOverlayLayer(m_pImGuiLayer);

		//-----------------------------------------------
		// Input - Initialization
		//-----------------------------------------------

		if (!InitializeInputManager(configFile))
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
		while (m_isRunning)
		{
			Time.RestartDeltaTime();

			if (!m_hasLostFocus)
			{
				// Dispatch Messages
				MessageServer::GetInstance()->DispatchMessages();

				// Handle Layers.
				for (Layer* pLayer : *m_pLayerStack)
					pLayer->OnUpdate();

				// Deallocate any resources necessary.
				ResourceLoader::GetInstance()->ProcessUnloadQueue();
			}

			// TODO: Move to render thread?
			EXE_ASSERT(m_pImGuiLayer);
			if (m_pImGuiLayer->Begin())
			{
				{
					for (Layer* pLayer : *m_pLayerStack)
						pLayer->OnImGuiRender();
				}
				m_pImGuiLayer->End();
			}
			// Refresh Input State.
			InputManager::GetInstance()->NextFrame();

			// Poll Window Events
			Renderer2D::GetInstance()->Update();

		}
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
		m_pLayerStack->PushLayer(pLayer);
	}

	void Application::PushOverlayLayer(Layer* pOverlayLayer)
	{
		m_pLayerStack->PushOverlayLayer(pOverlayLayer);
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
			// Refresh input to prevent carried over inputs from out of focus events.
			// This is because input polling still exists when the window is minimized
			// even though the engine does nothing with that input. Upon return, we want
			// to make sure that we begin with a clean state.
			// This is caused by the ordering of Input->NextFrame and Window->Update in
			// the application Update loop.
			InputManager::GetInstance()->NextFrame();
			Renderer2D::GetInstance()->OnWindowResize(pWinResized->GetWidth(), pWinResized->GetHeight());
		}

		for (auto it = m_pLayerStack->rbegin(); it != m_pLayerStack->rend(); ++it)
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
		FileLogDefinition fileDefinition;
		ConsoleLogDefinition consoleDefinition;
		eastl::vector<LogData> logData;

		if (!configFile.PopulateLogData(fileDefinition, consoleDefinition, logData))
		{
			EXE_LOG_CATEGORY_WARN("Application", "Failed to populate log data correctly. Please verify config file.");
		}

		if (!LogManager::GetInstance()->Initialize(fileDefinition, consoleDefinition, logData))
		{
			EXE_LOG_CATEGORY_FATAL("Application", "Exelius::LogManager::Initialize Failed.");
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
		WindowProperties windowProperties;

		if (!configFile.PopulateWindowData(windowProperties))
		{
			EXE_LOG_CATEGORY_WARN("Application", "Failed to populate window data correctly. Please verify config file.");
		}

		Renderer2D::SetSingleton(EXELIUS_NEW(Renderer2D(windowProperties)));
		if (!Renderer2D::GetInstance())
		{
			EXE_LOG_CATEGORY_FATAL("Application", "Exelius::Renderer failed to initialize.");
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
		InputManager::SetSingleton(EXELIUS_NEW(InputManager()));
		EXE_ASSERT(InputManager::GetInstance());
		if (!InputManager::GetInstance()->Initialize())
		{
			EXE_LOG_CATEGORY_FATAL("Application", "Exelius::InputManager failed to initialize.");
			return false;
		}

		// Attach the event observer to the window to handle window events.
		// TODO: I don't like this very much.
		Renderer2D::GetInstance()->GetWindow().GetEventMessenger().AddObserver(*InputManager::GetInstance());

		return true;
	}

	/// <summary>
	/// Initialize the ResourceLoader using the config file data if necessary.
	/// </summary>
	/// <param name="configFile">- The pre-parsed config file.</param>
	/// <returns>True on success, false otherwise.</returns>
	bool Application::InitializeResourceLoader([[maybe_unused]] const ConfigFile& configFile) const
	{
		// Create Resource Manager Singleton.
		ResourceLoader::SetSingleton(EXELIUS_NEW(ResourceLoader()));
		EXE_ASSERT(ResourceLoader::GetInstance());
		if (!ResourceLoader::GetInstance()->Initialize(m_pResourceFactory, "EngineResources/", true))
		{
			EXE_LOG_CATEGORY_FATAL("Application", "Exelius::ResourceLoader failed to initialize.");
			return false;
		}

		return true;
	}
}