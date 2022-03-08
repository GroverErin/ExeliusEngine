#include "EditorApplication.h"
#include <include/Input.h>

// -----------------------------------------//
// Test Headers								//
// -----------------------------------------//
#include "TestClientMessage.h"				//
#include "NetTestCustomConnectionHandler.h" //
#include <source/messages/MessageServer.h>	//
// -----------------------------------------//

EditorApplication::EditorApplication()
	: m_uiMenues()
	, m_leftPaddle()
	, m_rightPaddle()
	, m_ball()
	, m_pConnection(nullptr)
	, m_gameStarted(false)
{
}

bool EditorApplication::Initialize()
{
	//RunTestCode();

	auto* pGameObjectSystem = Exelius::GameObjectSystem::GetInstance();
	EXE_ASSERT(pGameObjectSystem);


	m_leftPaddle.Initialize("assets/GameObjects/player_paddle.json");
	m_rightPaddle.Initialize("assets/GameObjects/other_paddle.json");
	m_ball.Initialize("assets/GameObjects/ball.json");
	m_uiMenues[0].Initialize("assets/gameobjects/main_menu.json");
	m_uiMenues[1].Initialize("assets/gameobjects/game_hud.json");
	m_uiMenues[0].SetEnabled(true);

	auto id = pGameObjectSystem->CreateGameObject("assets/gameobjects/tilemap_test.json", Exelius::CreationMode::kQueueAndSignal);

	//pGameObjectSystem->GetGameObject(id)->SetEnabled(true);
	return true;
}

void EditorApplication::Update()
{
	if (!m_gameStarted && Exelius::IsKeyReleased(Exelius::KeyCode::H))
	{
		StartGame();
		StartHost();
	}
	else if (!m_gameStarted && Exelius::IsKeyReleased(Exelius::KeyCode::C))
	{
		StartGame();
		StartClient();
	}

	if (!m_gameStarted)
		return;

	m_leftPaddle.Update(true);
	m_rightPaddle.Update(false);
	m_ball.Update(m_leftPaddle.GetPaddleID(), m_rightPaddle.GetPaddleID());
}

void EditorApplication::ShutDown()
{
	m_uiMenues[0].Destroy();
	m_uiMenues[1].Destroy();
	m_leftPaddle.Destroy();
	m_rightPaddle.Destroy();
	m_ball.Destroy();

	EXELIUS_DELETE(m_pConnection);
}

void EditorApplication::StartHost()
{
	// Create an address object. Using Exelius::NetAddressDefines::Any_IPV4 will make this connection a TCP listener.
	Exelius::NetAddress address(Exelius::NetAddressDefines::Any_IPV4, 6565);

	// Create a connection object, with custom defined handler functions.
		// Creating a custom connection class is not necessary, but the client
		// would need to listen for the appropriate Messages in order to process
		// a NetMessage received properly. Although, the default connection will
		// handle the validation properly by default, it is much safer to do it
		// yourself (Public repository and all that).
	m_pConnection = EXELIUS_NEW(NetTestCustomConnectionHandler(address));
	EXE_ASSERT(m_pConnection);

	// Attempt to connect. This will likely NOT happen in this frame.
		// The connection callback OnConnected will trigger on a successful connection.
		// OnConnectionFailed will trigger otherwise.
	m_pConnection->Open();
}

void EditorApplication::StartClient()
{
	// Create an address object. Using Exelius::NetAddressDefines::LocalHost_IPV4 will make this connection a TCP connection locally.
	Exelius::NetAddress address(Exelius::NetAddressDefines::LocalHost_IPV4, 6565);

	// Create a connection object, with custom defined handler functions.
		// Creating a custom connection class is not necessary, but the client
		// would need to listen for the appropriate Messages in order to process
		// a NetMessage received properly. Although, the default connection will
		// handle the validation properly by default, it is much safer to do it
		// yourself (Public repository and all that).
	m_pConnection = EXELIUS_NEW(NetTestCustomConnectionHandler(address));
	EXE_ASSERT(m_pConnection);

	// Attempt to connect. This will likely NOT happen in this frame.
		// The connection callback OnConnected will trigger on a successful connection.
		// OnConnectionFailed will trigger otherwise.
	m_pConnection->Open();

}

void EditorApplication::StartGame()
{
	m_uiMenues[0].SetEnabled(false);
	m_uiMenues[1].SetEnabled(true);
	m_leftPaddle.SetEnabled();
	m_rightPaddle.SetEnabled();
	m_ball.SetEnabled();

	m_gameStarted = true;
}

void EditorApplication::RunTestCode()
{
	{ // Test Local Message
		TestCustomMessage* pMsg = EXELIUS_NEW(TestCustomMessage());
		auto id = pMsg->GetMessageID();
		if (Exelius::MessageServer::GetInstance())
			Exelius::MessageServer::GetInstance()->PushMessage(pMsg);
	}

	{ // Test Networking
		// Send a networked message. This message will not be sent if a connection
		// has not yet been validated, unless it is a local message.
		NetTestCustomMessage* pNetMsg = EXELIUS_NEW(NetTestCustomMessage());
		auto netID = pNetMsg->GetMessageID();
		pNetMsg->m_toPeerID = Exelius::PeerID_Invalid;

		if (Exelius::MessageServer::GetInstance())
			Exelius::MessageServer::GetInstance()->PushMessage(pNetMsg);
	}
}
