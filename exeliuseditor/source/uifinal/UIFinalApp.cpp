#include "UIFinalApp.h"

#include <include/Input.h>
#include <include/Log.h>

#include <source/utility/random/Random.h>

#include <source/os/threads/JobSystem.h>

#include <source/messages/Message.h>
#include <source/messages/MessageServer.h>

#include <source/networking/NetMessage.h>
#include <source/networking/Connection.h>
#include <source/networking/NetworkingManager.h>
#include <iostream>

class TestCustomMessage
	: public Exelius::Message
{
public:
	TestCustomMessage()
		: Message(DEFINE_MESSAGE(TestCustomMessage))
	{
		m_callback = [](Message*) { std::cout << "Test Complete\n"; };
	}
};

class NetTestCustomMessage
	: public Exelius::NetMessage<Exelius::NetProtocol::LocalOnly>
{
public:
	NetTestCustomMessage()
		: NetMessage(DEFINE_MESSAGE(NetTestCustomMessage))
	{
		m_callback = [](Message*) { std::cout << "Network Test Complete\n"; };
	}
};

class NetTestCustomConnectionHandler
	: public Exelius::Connection
{
public:
	NetTestCustomConnectionHandler(const Exelius::NetAddress& address)
		: Exelius::Connection(address)
	{
		//
	}

protected:

	// Called when a connection is successful to a remote host.
	virtual void OnConnected(Exelius::Message* pMsg) final override
	{
		std::cout << "Received OnConnected Message.\n";
		//Connection::OnConnected(pMsg);
	}

	// As "Client":
	//		Called after a "Host" sends a validation request message.
	// As "Host":
	//		Called after a "client sends a validation response.
	virtual void OnValidate(Exelius::Message* pMsg) final override
	{
		std::cout << "Received OnValidate Message.\n";
		//Connection::OnValidate(pMsg);
	}

	// Called when a validation accepted response is received.
	virtual void OnAccepted(Exelius::Message* pMsg) final override
	{
		std::cout << "Received OnAccepted Message.\n";
		//Connection::OnAccepted(pMsg);
	}

	// Called when a validation denied response is received.
	virtual void OnRejected(Exelius::Message* pMsg) final override
	{
		std::cout << "Received OnRejected Message.\n";
		//Connection::OnRejected(pMsg);
	}

	// Called when a send or receive attempt fails, thus signaling the connection is no longer valid.
	// Note: The connection will have already been closed and cleaned up upon this function call.
	virtual void OnDisconnected(Exelius::Message* pMsg) final override
	{
		std::cout << "Received OnDisconnected Message.\n";
		//Connection::OnDisconnected(pMsg);
	}

	// Called when an error or timeout occures.
	virtual void OnConnectionFailed(Exelius::Message* pMsg) final override
	{
		std::cout << "Received OnConnectionFailed Message.\n";
		//Connection::OnConnectionFailed(pMsg);
	}
};

bool UIFinalApp::Initialize()
{
	{ // Test Local Message
		TestCustomMessage* pMsg = EXELIUS_NEW(TestCustomMessage());
		auto id = pMsg->GetMessageID();
		if (Exelius::MessageServer::GetInstance())
			Exelius::MessageServer::GetInstance()->PushMessage(pMsg);
	}

	{ // Test Networking

		// Create an address object. Using Exelius::NetAddressDefines::Any_IPV4 will make this connection a TCP listener.
		Exelius::NetAddress address(Exelius::NetAddressDefines::Any_IPV4, 6565);

		// Create a connection object, with custom defined handler functions.
		// Creating a custom connection class is not necessary, but the client
		// would need to listen for the appropriate Messages in order to process
		// a NetMessage received properly. Although, the default connection will
		// handle the validation properly by default, it is much safer to do it
		// yourself (Public repository and all that).
		NetTestCustomConnectionHandler testConnection(address);

		// Attempt to connect. This will likely NOT happen in this frame.
		// The connection callback OnConnected will trigger on a successful connection.
		// OnConnectionFailed will trigger otherwise.
		testConnection.Connect();

		// Send a networked message. This message will not be sent if a connection
		// has not yet been validated.
		NetTestCustomMessage* pNetMsg = EXELIUS_NEW(NetTestCustomMessage());
		auto netID = pNetMsg->GetMessageID();
		pNetMsg->m_toPeerID = Exelius::PeerID_Invalid;

		if (Exelius::MessageServer::GetInstance())
			Exelius::MessageServer::GetInstance()->PushMessage(pNetMsg);
	}

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

	Exelius::Random rand;
	m_ballVelocity.x = (float)rand.IRandomRange(-100, 100) / 100.0f;
	m_ballVelocity.y = (float)rand.IRandomRange(-100, 100) / 100.0f;

	eastl::string uiObj = "assets/gameobjects/main_menu.json";
	eastl::string uiObjB = "assets/gameobjects/game_hud.json";

	m_uiMenues[0] = pGameObjectSystem->CreateGameObject(uiObj, Exelius::CreationMode::kQueueAndSignal);
	m_uiMenues[1] = pGameObjectSystem->CreateGameObject(uiObjB, Exelius::CreationMode::kQueueAndSignal);

	for (auto objID : m_uiMenues)
	{
		const auto& pObj = pGameObjectSystem->GetGameObject(objID);
		pObj->SetEnabled(false);
	}

	m_activeUIIndex = 0;

	const auto& pActiveObject = pGameObjectSystem->GetGameObject(m_uiMenues[m_activeUIIndex]);

	pActiveObject->SetEnabled(true);

	return true;
}

void UIFinalApp::Update()
{
	if (Exelius::IsKeyReleased(Exelius::KeyCode::Space) && !m_gameStarted)
	{
		StartGame();
		m_gameStarted = true;

		auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
		EXE_ASSERT(pGameObjectSystem);

		pGameObjectSystem->GetGameObject(m_playerPaddleID)->SetEnabled(true);
		pGameObjectSystem->GetGameObject(m_otherPaddleID)->SetEnabled(true);
		pGameObjectSystem->GetGameObject(m_ballID)->SetEnabled(true);
	}

	if (m_gameStarted)
	{
		//Update Paddles.
		UpdatePlayerPaddle();
		UpdateOtherPaddle();

		// Update Ball.
		UpdateBall();
	}
}

void UIFinalApp::ShutDown()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	for (auto objID : m_uiMenues)
	{
		pGameObjectSystem->DestroyGameObject(objID);
	}

	pGameObjectSystem->DestroyGameObject(m_playerPaddleID);
	pGameObjectSystem->DestroyGameObject(m_otherPaddleID);
	pGameObjectSystem->DestroyGameObject(m_ballID);
}

void UIFinalApp::StartGame()
{
	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);

	// Swap the images.
	Exelius::GameObjectID oldActive = m_uiMenues[m_activeUIIndex];
	++m_activeUIIndex;
	if ((size_t)m_activeUIIndex >= m_uiMenues.size())
		m_activeUIIndex = 0;

	const auto& pInactiveObject = pGameObjectSystem->GetGameObject(oldActive);
	pInactiveObject->SetEnabled(false);

	const auto& pActiveObject = pGameObjectSystem->GetGameObject(m_uiMenues[m_activeUIIndex]);
	pActiveObject->SetEnabled(true);

	// Reset the transform of the inactive object.
	if (auto tranform = pInactiveObject->GetComponent<Exelius::TransformComponent>())
	{
		tranform->SetX(0.0f);
		tranform->SetY(0.0f);
	}
}

void UIFinalApp::UpdatePlayerPaddle()
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

void UIFinalApp::UpdateOtherPaddle()
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

void UIFinalApp::UpdateBall()
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

		Exelius::FRectangle intersectRect;
		if (ballRect.Intersects(playerPaddleRect, intersectRect))
		{
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