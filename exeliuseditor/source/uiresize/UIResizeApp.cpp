#include "UIResizeApp.h"

#include <include/Exelius.h>
#include <include/Log.h>

bool UIResizeApp::Initialize()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	eastl::string uiImage = "assets/ui/gameobjects/uiTestA.json";
	eastl::string uiImageB = "assets/ui/gameobjects/uiTestB.json";

	pGameObjectSystem->CreateGameObject(uiImage, Exelius::CreationMode::kQueueAndSignal);
	pGameObjectSystem->CreateGameObject(uiImageB, Exelius::CreationMode::kQueueAndSignal);

	return true;
}

void UIResizeApp::Update()
{
}

void UIResizeApp::ShutDown()
{
}
