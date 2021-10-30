#include "UIResizeApp.h"

#include <include/Input.h>
#include <include/Log.h>

#include <source/os/threads/JobSystem.h>

bool UIResizeApp::Initialize()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	eastl::string uiObj = "assets/gameobjects/test_ui_no_layout.json";
	eastl::string uiObjB = "assets/gameobjects/test_ui_wrap_v.json";
	eastl::string uiObjC = "assets/gameobjects/test_ui_wrap_h.json";
	eastl::string uiObjD = "assets/gameobjects/test_ui_stack_v.json";
	eastl::string uiObjE = "assets/gameobjects/test_ui_stack_h.json";

	m_uiTests[0] = pGameObjectSystem->CreateGameObject(uiObj, Exelius::CreationMode::kQueueAndSignal);
	m_uiTests[1] = pGameObjectSystem->CreateGameObject(uiObjB, Exelius::CreationMode::kQueueAndSignal);
	m_uiTests[2] = pGameObjectSystem->CreateGameObject(uiObjC, Exelius::CreationMode::kQueueAndSignal);
	m_uiTests[3] = pGameObjectSystem->CreateGameObject(uiObjD, Exelius::CreationMode::kQueueAndSignal);
	m_uiTests[4] = pGameObjectSystem->CreateGameObject(uiObjE, Exelius::CreationMode::kQueueAndSignal);

	for (auto objID : m_uiTests)
	{
		const auto& pObj = pGameObjectSystem->GetGameObject(objID);
		pObj->SetEnabled(false);
	}

	m_activeUIIndex = 0;

	const auto& pActiveObject = pGameObjectSystem->GetGameObject(m_uiTests[m_activeUIIndex]);

	pActiveObject->SetEnabled(true);
	return true;
}

void UIResizeApp::Update()
{
	if (Exelius::IsKeyReleased(Exelius::KeyCode::Space))
	{
		SwapImagesAndReset();
	}

	MoveImage();
	StretchImage();
}

void UIResizeApp::ShutDown()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	for (auto objID : m_uiTests)
	{
		pGameObjectSystem->DestroyGameObject(objID);
	}
}

void UIResizeApp::SwapImagesAndReset()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	// Swap the images.
	Exelius::GameObjectID oldActive = m_uiTests[m_activeUIIndex];
	++m_activeUIIndex;
	if ((size_t)m_activeUIIndex >= m_uiTests.size())
		m_activeUIIndex = 0;

	const auto& pInactiveObject = pGameObjectSystem->GetGameObject(oldActive);
	pInactiveObject->SetEnabled(false);

	const auto& pActiveObject = pGameObjectSystem->GetGameObject(m_uiTests[m_activeUIIndex]);
	pActiveObject->SetEnabled(true);

	// Reset the transform of the inactive object.
	if (auto tranform = pInactiveObject->GetComponent<Exelius::TransformComponent>())
	{
		tranform->SetX(0.0f);
		tranform->SetY(0.0f);
	}

	if (auto ui = pInactiveObject->GetComponent<Exelius::UIComponent>())
	{
		//ui->Reset();
	}
}

void UIResizeApp::MoveImage()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	const auto& pObject = pGameObjectSystem->GetGameObject(m_uiTests[m_activeUIIndex]);
	if (auto transform = pObject->GetComponent<Exelius::TransformComponent>())
	{
		if (Exelius::IsKeyDown(Exelius::KeyCode::W))
			transform->SetY(transform->GetY() - kPictureSpeed * Exelius::Time::DeltaTime.GetAsSeconds());
		else if (Exelius::IsKeyDown(Exelius::KeyCode::S))
			transform->SetY(transform->GetY() + kPictureSpeed * Exelius::Time::DeltaTime.GetAsSeconds());

		if (transform->GetY() < 0.0f)
			transform->SetY(0.0f);

		if (transform->GetY() > 384.0f)
			transform->SetY(384.0f);

		if (Exelius::IsKeyDown(Exelius::KeyCode::A))
			transform->SetX(transform->GetX() - kPictureSpeed * Exelius::Time::DeltaTime.GetAsSeconds());
		else if (Exelius::IsKeyDown(Exelius::KeyCode::D))
			transform->SetX(transform->GetX() + kPictureSpeed * Exelius::Time::DeltaTime.GetAsSeconds());

		if (transform->GetX() < 0.0f)
			transform->SetX(0.0f);

		if (transform->GetX() > 384.0f)
			transform->SetX(384.0f);
	}
}

void UIResizeApp::StretchImage()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);
}
