#include "EXEPCH.h"

#include "Source/Engine/Application.h"
#include "Source/OS/Events/ApplicationEvents.h"

#include "Source/Render/RenderManager.h"

#include "Source/OS/Input/InputManager.h"

#include "Source/Resource/ResourceManager.h"
#include "Source/Engine/Game/GameObjectSystem/GameObjectSystem.h"

#include "Source/Engine/Resources/ExeliusResourceFactory.h"
#include "Source/Engine/Game/GameObjectSystem/Components/ExeliusComponentFactory.h"

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
		, m_pResourceFactory(nullptr)
		, m_pComponentFactory(nullptr)
		, m_lastFrameTime(0.0f)
		, m_isRunning(true)
		, m_hasLostFocus(false)
	{
		m_window.GetEventMessenger().AddObserver(*this);

		InputManager::SetSingleton(new InputManager());
		EXE_ASSERT(InputManager::GetInstance());

		m_window.GetEventMessenger().AddObserver(*InputManager::GetInstance());
	}

	Application::~Application()
	{
		GameObjectSystem::DestroySingleton();

		delete m_pComponentFactory;
		m_pComponentFactory = nullptr;

		ResourceManager::DestroySingleton();

		m_window.GetEventMessenger().RemoveObserver(*InputManager::GetInstance());

		InputManager::DestroySingleton();

		RenderManager::DestroySingleton();

		m_window.GetEventMessenger().RemoveObserver(*this);
	}

	bool Application::InitializeExelius()
	{
		RenderManager::SetSingleton(new RenderManager());
		EXE_ASSERT(RenderManager::GetInstance());

		if (!RenderManager::GetInstance()->Initialize())
		{
			EXELOG_ENGINE_FATAL("Exelius::RenderManager failed to initialize.");
			return false;
		}

		//m_window.SetVSync(false);

		if (!InputManager::GetInstance()->Initialize())
		{
			EXELOG_ENGINE_FATAL("Exelius::InputManager failed to initialize.");
			return false;
		}

		SetResourceFactory();
		EXE_ASSERT(m_pResourceFactory);

		// Create Resource Manager Singleton.
		ResourceManager::SetSingleton(new ResourceManager());
		EXE_ASSERT(ResourceManager::GetInstance());
		if (!ResourceManager::GetInstance()->Initialize(m_pResourceFactory, "EngineResources/", true))
		{
			EXELOG_ENGINE_FATAL("Exelius::ResourceManager failed to initialize.");
			return false;
		}

		// This will call either the default (Exelius) version, or the client's if defined.
		SetComponentFactory();
		EXE_ASSERT(m_pComponentFactory);

		GameObjectSystem::SetSingleton(new GameObjectSystem());
		EXE_ASSERT(GameObjectSystem::GetInstance());
		if (!GameObjectSystem::GetInstance()->Initialize(m_pComponentFactory))
		{
			EXELOG_ENGINE_FATAL("Exelius::GameObjectSystem failed to initialize.");
			return false;
		}

		return Initialize();
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
			// EXELOG_ENGINE_TRACE("DeltaTime: {}", deltaTime.count());

			// Poll Window Events.
			m_window.OnUpdate();
			
			// Update Components.
			GameObjectSystem::GetInstance()->Update();

			// Client Update.
			Update();

			m_window.GetNativeWindow().Clear();
			GameObjectSystem::GetInstance()->Render();
			m_window.OnRender();

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
}