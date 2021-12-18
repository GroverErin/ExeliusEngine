#include "EXEPCH.h"
#include "Connection.h"

#include "source/messages/MessageServer.h"
#include "source/messages/MessageReceiver.h"
#include "source/networking/NetworkingManager.h"

#include "source/messages/exeliusmessages/networking/ConnectionMessages.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	Connection::Connection(const NetAddress& address)
		: m_connectionAddress(address)
		, m_id(-1)
	{
		MessageServer* pMessageServer = MessageServer::GetInstance();
		EXE_ASSERT(pMessageServer);

		// Begin recieving all networked messages we are responsible for.
		m_listeners[0] = eastl::make_shared<MessageReceiver>([this](Message* pMsg) { OnConnected(pMsg); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(ConnectedMessage), m_listeners[0]);

		m_listeners[1] = eastl::make_shared<MessageReceiver>([this](Message* pMsg) { OnValidate(pMsg); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(ValidateMessage), m_listeners[1]);

		m_listeners[2] = eastl::make_shared<MessageReceiver>([this](Message* pMsg) { OnAccepted(pMsg); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(AcceptedMessage), m_listeners[2]);

		m_listeners[3] = eastl::make_shared<MessageReceiver>([this](Message* pMsg) { OnRejected(pMsg); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(RejectedMessage), m_listeners[3]);

		m_listeners[4] = eastl::make_shared<MessageReceiver>([this](Message* pMsg) { OnDisconnected(pMsg); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(DisconnectedMessage), m_listeners[4]);

		m_listeners[5] = eastl::make_shared<MessageReceiver>([this](Message* pMsg) { OnConnectionFailed(pMsg); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(ConnectionFailedMessage), m_listeners[5]);
	}

	Connection::~Connection()
	{
		Disconnect();

		for (auto& listener : m_listeners)
		{
			listener->ClearCallback();
		}
	}

	void Connection::Connect()
	{
		NetworkingManager* pNetManager = NetworkingManager::GetInstance();
		EXE_ASSERT(pNetManager);

		EXE_ASSERT(m_id <= -1); // The ID must not be set, the user is expected to call disconnect before trying to call Connect again.

		m_id = pNetManager->Connect(m_connectionAddress);
	}

	void Connection::DisconnectPeer(PeerID idToDisconnect)
	{
		NetworkingManager* pNetManager = NetworkingManager::GetInstance();
		EXE_ASSERT(pNetManager);

		pNetManager->DisconnectPeer(idToDisconnect);
	}

	void Connection::DisconnectAll()
	{
		NetworkingManager* pNetManager = NetworkingManager::GetInstance();
		EXE_ASSERT(pNetManager);

		pNetManager->DisconnectAll();
	}
	
	void Connection::Disconnect()
	{
		NetworkingManager* pNetManager = NetworkingManager::GetInstance();
		EXE_ASSERT(pNetManager);

		pNetManager->DisconnectPeer(m_id);
		m_id = PeerID_Invalid;
	}
}
