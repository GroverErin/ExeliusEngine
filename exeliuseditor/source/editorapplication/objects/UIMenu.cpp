#include "UIMenu.h"

bool UIMenu::Initialize(const eastl::string& dataPath)
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	m_menuID = pGameObjectSystem->CreateGameObject(dataPath, Exelius::CreationMode::kQueueAndSignal);

	return true;
}

void UIMenu::SetEnabled(bool isEnabled)
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	pGameObjectSystem->GetGameObject(m_menuID)->SetEnabled(isEnabled);
}

void UIMenu::Destroy()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);
	pGameObjectSystem->DestroyGameObject(m_menuID);
}
