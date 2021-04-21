#include "Pong/PongGame.h"

#include <Include/EntryPoint.h>

#include <Include/Exelius.h>
#include <Include/Input.h>
#include <Include/Log.h>

class Sandbox final
	: public Exelius::Application
{
	PongGame m_pongGame;

public:
	Sandbox()
		: Application("ClientTitle", 720, 640)
	{

	}

	virtual ~Sandbox()
	{
		m_pongGame.ShutDown();
		//Exelius::GameObjectSystem::GetInstance()->DestroyGameObject(0);
	}

	virtual bool Initialize() final override
	{
		if (!m_pongGame.Initialize())
		{
			EXELOG_FATAL("Pong failed to Initialize.");
			return false;
		}
		//auto* pResourceManagerTester = Exelius::ResourceManager::GetInstance();
		//EXE_ASSERT(pResourceManagerTester);
		//auto* pGameObjectSystemTester = Exelius::GameObjectSystem::GetInstance();
		//EXE_ASSERT(pGameObjectSystemTester);

		//EXELOG_ERROR("Resource Manager test code found in Sandbox::Initialize().");

		//eastl::string str = "../Assets/GameObjects/GameObjectTest.json";

		//auto& id = pResourceManagerTester->LoadNow(str); //TODO: QueueLoad will run forever on release mode.

		//auto* pResource = static_cast<Exelius::TextFileResource*>(pResourceManagerTester->GetResource(id, false));
		///*while (!pResource)
		//{
		//	EXELOG_ENGINE_FATAL("Failed to retrieve resource.");
		//	pResource = static_cast<TextFileResource*>(pResourceManagerTester->GetResource(id));
		//}*/

		//EXELOG_INFO("{}:\n{}", str.c_str(), pResource->GetRawText().c_str());

		//auto objectID = pGameObjectSystemTester->CreateGameObject(pResource);

		//auto* pObject = pGameObjectSystemTester->GetGameObject(objectID);

		//if (pObject->GetComponent<Exelius::TransformComponent>())
		//{
		//	auto transformComp = pObject->GetComponent<Exelius::TransformComponent>();
		//	float y = transformComp->GetY();
		//	EXELOG_INFO("PRINTING pObject: {}, {}, ({}, {})", pObject->GetName().c_str(), pObject->GetId(), transformComp.Get().GetX(), y);
		//}

		//pObject = nullptr; // TODO: I don't know if I like that it returns a pointer. Maybe a handle?

		//// Release the GameObjectJSON. We no longer need it. TODO: this should be done automatically by GameObject, maybe?
		//pResourceManagerTester->ReleaseResource(id);

		//// Just a precaution.
		//pGameObjectSystemTester = nullptr;
		//pResourceManagerTester = nullptr;
		//
		return true;
	}

	virtual void Update() final override
	{
		m_pongGame.Update();
		//auto* pGameObjectSystemTester = Exelius::GameObjectSystem::GetInstance();
		//auto* pObject = pGameObjectSystemTester->GetGameObject(0);
		//if (pObject->GetComponent<Exelius::TransformComponent>())
		//{
		//	auto transformComp = pObject->GetComponent<Exelius::TransformComponent>();

		//	if (Exelius::IsKeyDown(Exelius::KeyCode::W))
		//		transformComp->SetY(transformComp->GetY() - 1.0f);
		//	else if (Exelius::IsKeyDown(Exelius::KeyCode::S))
		//		transformComp->SetY(transformComp->GetY() + 1.0f);

		//	if (Exelius::IsKeyDown(Exelius::KeyCode::A))
		//		transformComp->SetX(transformComp->GetX() - 1.0f);
		//	else if (Exelius::IsKeyDown(Exelius::KeyCode::D))
		//		transformComp->SetX(transformComp->GetX() + 1.0f);
		//	//float y = transformComp->GetY();
		//	//EXELOG_INFO("Transform X: {})", transformComp.Get().GetX());
		//}
	}
};

EXELIUS_MAIN(Sandbox)