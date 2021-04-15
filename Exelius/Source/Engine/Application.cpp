#include "EXEPCH.h"

#include "Source/Engine/Application.h"
#include "Source/OS/Events/ApplicationEvents.h"
#include "Source/Engine/Resources/ResourceTypes/TextFileResource.h"

#include "Source/Engine/Game/GameObjectSystem/GameObject.h"
#include "Source/Engine/Game/GameObjectSystem/GameObjectSystem.h"

#include "Source/Engine/Game/GameObjectSystem/Components/ComponentHandle.h"
#include "Source/Engine/Game/GameObjectSystem/Components/ComponentTypes/TransformComponent.h"
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
		ResourceManager::DestroySingleton();
		m_window.GetEventMessenger().RemoveObserver(*this);
	}

	bool Application::Initialize()
	{
		// Create Resource Manager Singleton.
		ResourceManager::SetSingleton(new ResourceManager());
		auto* pResourceManagerTester = ResourceManager::GetInstance();
		pResourceManagerTester->Initialize(&m_resourceFactory, "EngineResources/",  true);

		GameObjectSystem::SetSingleton(new GameObjectSystem());
		auto* pGameObjectSystemTester = GameObjectSystem::GetInstance();
		pGameObjectSystemTester->Initialize(&m_componentFactory);

		EXELOG_ENGINE_ERROR("Resource Manager test code found in Application::Initialize().");

		eastl::string str = "../GameObjectTest.json";

		auto& id = pResourceManagerTester->QueueLoad(str, true);

		auto* pResource = static_cast<TextFileResource*>(pResourceManagerTester->GetResource(id));
		while (!pResource)
		{
			EXELOG_ENGINE_FATAL("Failed to retrieve resource.");
			pResource = static_cast<TextFileResource*>(pResourceManagerTester->GetResource(id));
		}

		EXELOG_ENGINE_INFO("{}:\n{}", str.c_str(), pResource->GetRawText().c_str());

		auto objectID = pGameObjectSystemTester->CreateGameObject(pResource);

		auto* pObject = pGameObjectSystemTester->GetGameObject(objectID);

		if (pObject->GetComponent<TransformComponent>())
		{
			auto transformComp = pObject->GetComponent<TransformComponent>();
			float y = transformComp->GetY();
			EXELOG_ENGINE_INFO("PRINTING pObject: {}, {}, ({}, {})", pObject->GetName().c_str(), pObject->GetId(), transformComp.Get().GetX(), y);
		}

		pGameObjectSystemTester->DestroyGameObject(objectID);
		pObject = nullptr; // THIS IS NOT IDEAL
		
		pResourceManagerTester->ReleaseResource(id);

		return true;
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