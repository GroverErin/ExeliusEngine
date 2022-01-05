#include "Paddle.h"

#include <include/Input.h>

bool Paddle::Initialize(const eastl::string& dataPath)
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	m_paddleID = pGameObjectSystem->CreateGameObject(dataPath, Exelius::CreationMode::kQueueAndSignal);
	return true;
}

void Paddle::SetEnabled(bool isEnabled)
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	pGameObjectSystem->GetGameObject(m_paddleID)->SetEnabled(isEnabled);
}

void Paddle::Update(bool isLeft)
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	const auto& pObject = pGameObjectSystem->GetGameObject(m_paddleID);
	if (pObject->GetComponent<Exelius::TransformComponent>())
	{
		auto transformComp = pObject->GetComponent<Exelius::TransformComponent>();

		if (isLeft)
		{
			if (Exelius::IsKeyDown(Exelius::KeyCode::W))
				transformComp->SetY(transformComp->GetY() - kPaddleSpeed * Exelius::Time::DeltaTime.GetAsSeconds());
			else if (Exelius::IsKeyDown(Exelius::KeyCode::S))
				transformComp->SetY(transformComp->GetY() + kPaddleSpeed * Exelius::Time::DeltaTime.GetAsSeconds());
		}
		else
		{
			if (Exelius::IsKeyDown(Exelius::KeyCode::Up))
				transformComp->SetY(transformComp->GetY() - kPaddleSpeed * Exelius::Time::DeltaTime.GetAsSeconds());
			else if (Exelius::IsKeyDown(Exelius::KeyCode::Down))
				transformComp->SetY(transformComp->GetY() + kPaddleSpeed * Exelius::Time::DeltaTime.GetAsSeconds());
		}

		if (transformComp->GetY() < 0.0f)
			transformComp->SetY(0.0f);

		if (transformComp->GetY() > 440.0f)
			transformComp->SetY(440.0f);
	}
}

void Paddle::Destroy()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);
	pGameObjectSystem->DestroyGameObject(m_paddleID);
}
