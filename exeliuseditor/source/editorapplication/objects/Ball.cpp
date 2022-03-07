#include "Ball.h"
#include <source/utility/random/Random.h>

bool Ball::Initialize(const eastl::string& dataPath)
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	m_ballID = pGameObjectSystem->CreateGameObject(dataPath, Exelius::CreationMode::kQueueAndSignal);

	Exelius::Random rand;
	m_ballVelocity.x = (float)rand.IRandomRange(-100, 100) / 100.0f;
	m_ballVelocity.y = (float)rand.IRandomRange(-100, 100) / 100.0f;

	return true;
}

void Ball::SetEnabled(bool isEnabled)
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	pGameObjectSystem->GetGameObject(m_ballID)->SetEnabled(isEnabled);
}

void Ball::Update(Exelius::GameObjectID leftPaddleID, Exelius::GameObjectID rightPaddleID)
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	const auto& pObject = pGameObjectSystem->GetGameObject(m_ballID);
	if (pObject->GetComponent<Exelius::TransformComponent>())
	{
		auto transformComp = pObject->GetComponent<Exelius::TransformComponent>();
		m_ballVelocity = m_ballVelocity.Normalize();
		transformComp->Move(m_ballVelocity.x * kBallSpeed * Exelius::Time::DeltaTime.GetAsSeconds(), m_ballVelocity.y * kBallSpeed * Exelius::Time::DeltaTime.GetAsSeconds());

		if (transformComp->GetY() < -10.0f)
		{
			transformComp->SetY(0.0f);
			m_ballVelocity.y *= -1.0f;
		}
		if (transformComp->GetY() > 620.0f)
		{
			transformComp->SetY(600.0f);
			m_ballVelocity.y *= -1.0f;
		}

		if (transformComp->GetX() < 0.0f || transformComp->GetX() > 720.0f)
		{
			transformComp->SetX(360.0f);
			transformComp->SetY(300.0f);

			Exelius::Random rand;
			m_ballVelocity.x = (float)rand.IRandomRange(-100, 100) / 100.0f;
			m_ballVelocity.y = (float)rand.IRandomRange(-100, 100) / 100.0f;
		}

		// Check if we are intersecting.
		Exelius::FRectangle ballRect;
		ballRect.m_left = transformComp->GetX();
		ballRect.m_top = transformComp->GetY();
		ballRect.m_width = transformComp->GetW();
		ballRect.m_height = transformComp->GetH();

		const auto& pPPObject = pGameObjectSystem->GetGameObject(leftPaddleID);
		auto ppTransformComp = pPPObject->GetComponent<Exelius::TransformComponent>();

		Exelius::FRectangle playerPaddleRect;
		playerPaddleRect.m_left = ppTransformComp->GetX();
		playerPaddleRect.m_top = ppTransformComp->GetY();
		playerPaddleRect.m_width = ppTransformComp->GetW();
		playerPaddleRect.m_height = ppTransformComp->GetH();

		const auto& pOPObject = pGameObjectSystem->GetGameObject(rightPaddleID);
		auto opTransformComp = pOPObject->GetComponent<Exelius::TransformComponent>();

		Exelius::FRectangle otherPaddleRect;
		otherPaddleRect.m_left = opTransformComp->GetX();
		otherPaddleRect.m_top = opTransformComp->GetY();
		otherPaddleRect.m_width = opTransformComp->GetW();
		otherPaddleRect.m_height = opTransformComp->GetH();

		// Get the audio
		auto audioComp = pObject->GetComponent<Exelius::AudioComponent>();

		Exelius::FRectangle intersectRect;
		if (ballRect.Intersects(playerPaddleRect, intersectRect))
		{
			audioComp->Play();
			if (intersectRect.w < intersectRect.h)
			{
				// We need to move left or right.
				if (intersectRect.x > ballRect.x)
				{
					// Move left
					transformComp->Move(-intersectRect.w, 0.0f);
				}
				else
				{
					// move right
					transformComp->Move(intersectRect.w, 0.0f);
				}
				m_ballVelocity.x *= -1.0f;
			}
			else
			{
				// We need to move up or down.
				if (intersectRect.y > ballRect.y)
				{
					// Move up
					transformComp->Move(0.0f, -intersectRect.h);
				}
				else
				{
					// move down
					transformComp->Move(0.0f, intersectRect.h);
				}
				m_ballVelocity.y *= -1.0f;
			}
		}
		else if (ballRect.Intersects(otherPaddleRect, intersectRect))
		{
			audioComp->Play();
			if (intersectRect.w < intersectRect.h)
			{
				// We need to move left or right.
				if (intersectRect.x > ballRect.x)
				{
					// Move left
					transformComp->Move(-intersectRect.w, 0.0f);
				}
				else
				{
					// move right
					transformComp->Move(intersectRect.w, 0.0f);
				}
				m_ballVelocity.x *= -1.0f;
			}
			else
			{
				// We need to move up or down.
				if (intersectRect.y > ballRect.y)
				{
					// Move up
					transformComp->Move(0.0f, -intersectRect.h);
				}
				else
				{
					// move down
					transformComp->Move(0.0f, intersectRect.h);
				}
				m_ballVelocity.y *= -1.0f;
			}
		}
	}
}

void Ball::Destroy()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);
	pGameObjectSystem->DestroyGameObject(m_ballID);
}
