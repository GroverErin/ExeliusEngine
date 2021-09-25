#include "PongGame.h"

//#include <Include/Exelius.h>
#include <Include/Log.h>
#include <Include/Input.h>

bool PongGame::Initialize()
{
    auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
    EXE_ASSERT(pGameObjectSystem);

    eastl::string playerPaddle = "assets/GameObjects/player_paddle.json";
    eastl::string otherPaddle = "assets/GameObjects/other_paddle.json";
    eastl::string ball = "assets/GameObjects/ball.json";

    // Create Paddles.
    m_playerPaddleID = pGameObjectSystem->CreateGameObject(playerPaddle, Exelius::CreationMode::kQueueAndSignal);
    m_otherPaddleID = pGameObjectSystem->CreateGameObject(otherPaddle, Exelius::CreationMode::kQueueAndSignal);

    // Create Ball.
    m_ballID = pGameObjectSystem->CreateGameObject(ball, Exelius::CreationMode::kQueueAndSignal);
    m_ballVelocity.x = 1.0f;
    m_ballVelocity.y = 1.0f;

    return true;
}

void PongGame::Update()
{
    //Update Paddles.
    UpdatePlayerPaddle();
    UpdateOtherPaddle();
    
    // Update Ball.
    UpdateBall();
}

void PongGame::ShutDown()
{
    auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
    EXE_ASSERT(pGameObjectSystem);

    pGameObjectSystem->DestroyGameObject(m_playerPaddleID);
    pGameObjectSystem->DestroyGameObject(m_otherPaddleID);
    pGameObjectSystem->DestroyGameObject(m_ballID);
}

void PongGame::UpdatePlayerPaddle()
{
    auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
    EXE_ASSERT(pGameObjectSystem);

    const auto& pObject = pGameObjectSystem->GetGameObject(m_playerPaddleID);
    if (pObject->GetComponent<Exelius::TransformComponent>())
    {
        auto transformComp = pObject->GetComponent<Exelius::TransformComponent>();

        if (Exelius::IsKeyDown(Exelius::KeyCode::W))
            transformComp->SetY(transformComp->GetY() - kPaddleSpeed * Exelius::Time::DeltaTime.GetAsSeconds());
        else if (Exelius::IsKeyDown(Exelius::KeyCode::S))
            transformComp->SetY(transformComp->GetY() + kPaddleSpeed * Exelius::Time::DeltaTime.GetAsSeconds());

        if (transformComp->GetY() < 0.0f)
            transformComp->SetY(0.0f);

        if (transformComp->GetY() > 440.0f)
            transformComp->SetY(440.0f);
    }
}

void PongGame::UpdateOtherPaddle()
{
    auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
    EXE_ASSERT(pGameObjectSystem);

    const auto& pObject = pGameObjectSystem->GetGameObject(m_otherPaddleID);
    if (pObject->GetComponent<Exelius::TransformComponent>())
    {
        auto transformComp = pObject->GetComponent<Exelius::TransformComponent>();

        if (Exelius::IsKeyDown(Exelius::KeyCode::Up))
            transformComp->SetY(transformComp->GetY() - kPaddleSpeed * Exelius::Time::DeltaTime.GetAsSeconds());
        else if (Exelius::IsKeyDown(Exelius::KeyCode::Down))
            transformComp->SetY(transformComp->GetY() + kPaddleSpeed * Exelius::Time::DeltaTime.GetAsSeconds());

        if (transformComp->GetY() < 0.0f)
            transformComp->SetY(0.0f);

        if (transformComp->GetY() > 440.0f)
            transformComp->SetY(440.0f);
    }
}

void PongGame::UpdateBall()
{
    auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
    EXE_ASSERT(pGameObjectSystem);

    const auto& pObject = pGameObjectSystem->GetGameObject(m_ballID);
    if (pObject->GetComponent<Exelius::TransformComponent>())
    {
        auto transformComp = pObject->GetComponent<Exelius::TransformComponent>();
        
        transformComp->Move(m_ballVelocity.x * kBallSpeed * Exelius::Time::DeltaTime.GetAsSeconds(), m_ballVelocity.y * kBallSpeed * Exelius::Time::DeltaTime.GetAsSeconds());

        if (transformComp->GetY() < -10.0f)
            m_ballVelocity.y = -m_ballVelocity.y;
        if (transformComp->GetY() > 620.0f)
            m_ballVelocity.y = -m_ballVelocity.y;


        if (transformComp->GetX() < 0.0f || transformComp->GetX() > 720.0f)
        {
            transformComp->SetX(360.0f);
            transformComp->SetY(300.0f);
            m_ballVelocity.x = 1.0f;
            m_ballVelocity.y = 1.0f;
        }

        // Check if we are intersecting.
        Exelius::FRectangle ballRect;
        ballRect.m_left = transformComp->GetX();
        ballRect.m_top = transformComp->GetY();
        ballRect.m_width = transformComp->GetW();
        ballRect.m_height = transformComp->GetH();

        const auto& pPPObject = pGameObjectSystem->GetGameObject(m_playerPaddleID);
        auto ppTransformComp = pPPObject->GetComponent<Exelius::TransformComponent>();

        Exelius::FRectangle playerPaddleRect;
        playerPaddleRect.m_left = ppTransformComp->GetX();
        playerPaddleRect.m_top = ppTransformComp->GetY();
        playerPaddleRect.m_width = ppTransformComp->GetW();
        playerPaddleRect.m_height = ppTransformComp->GetH();

        const auto& pOPObject = pGameObjectSystem->GetGameObject(m_otherPaddleID);
        auto opTransformComp = pOPObject->GetComponent<Exelius::TransformComponent>();

        Exelius::FRectangle otherPaddleRect;
        otherPaddleRect.m_left = opTransformComp->GetX();
        otherPaddleRect.m_top = opTransformComp->GetY();
        otherPaddleRect.m_width = opTransformComp->GetW();
        otherPaddleRect.m_height = opTransformComp->GetH();

        if (ballRect.Intersects(playerPaddleRect) || ballRect.Intersects(otherPaddleRect))
        {
            m_ballVelocity.x = -m_ballVelocity.x;
        }
    }
}

void PongGame::ReboundWalls()
{
}

void PongGame::HitGoal()
{
}

void PongGame::VerifyBounds()
{
}

void PongGame::CheckBallCollision()
{
}
