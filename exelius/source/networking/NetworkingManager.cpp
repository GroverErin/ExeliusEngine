#include "EXEPCH.h"
#include "source/networking/NetworkingManager.h"

#include "source/messages/MessageServer.h"
#include "source/messages/MessageReceiver.h"
#include "source/networking/SocketManager.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	NetworkingManager::NetworkingManager()
		: m_pMessageFactory(nullptr)
		, m_peerIDCounter(PeerID_Invalid)
	{
		//
	}

	NetworkingManager::~NetworkingManager()
	{
		DisconnectAll();
		SocketManager::DestroyInstance();
	}
	
	bool NetworkingManager::Initialize(MessageFactory* pMessageFactory)
	{
		EXE_ASSERT(pMessageFactory);
		m_pMessageFactory = pMessageFactory;

		EXE_ASSERT(SocketManager::GetInstance());

		MessageServer* pMessageServer = MessageServer::GetInstance();
		EXE_ASSERT(pMessageServer);

		// Begin recieving all networked messages.
		m_listeners[0] = eastl::make_shared<MessageReceiver>([this](Message* pMsg)
			{ HandleNetMessage<NetProtocol::LocalOnly>(reinterpret_cast<NetMessage<NetProtocol::LocalOnly>*>(pMsg)); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(NetMessage<NetProtocol::LocalOnly>), m_listeners[0]);

		m_listeners[1] = eastl::make_shared<MessageReceiver>([this](Message* pMsg)
			{ HandleNetMessage<NetProtocol::BroadcastUnreliable>(reinterpret_cast<NetMessage<NetProtocol::BroadcastUnreliable>*>(pMsg)); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(NetMessage<NetProtocol::BroadcastUnreliable>), m_listeners[1]);

		m_listeners[2] = eastl::make_shared<MessageReceiver>([this](Message* pMsg)
			{ HandleNetMessage<NetProtocol::BroadcastReliable>(reinterpret_cast<NetMessage<NetProtocol::BroadcastReliable>*>(pMsg)); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(NetMessage<NetProtocol::BroadcastReliable>), m_listeners[2]);

		m_listeners[3] = eastl::make_shared<MessageReceiver>([this](Message* pMsg)
			{ HandleNetMessage<NetProtocol::Unreliable>(reinterpret_cast<NetMessage<NetProtocol::Unreliable>*>(pMsg)); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(NetMessage<NetProtocol::Unreliable>), m_listeners[3]);

		m_listeners[4] = eastl::make_shared<MessageReceiver>([this](Message* pMsg)
			{ HandleNetMessage<NetProtocol::Reliable>(reinterpret_cast<NetMessage<NetProtocol::Reliable>*>(pMsg)); });
		pMessageServer->AddMessageReceiver(DEFINE_MESSAGE(NetMessage<NetProtocol::Reliable>), m_listeners[4]);

		// Begin listening for other network messages too. (Connect, Disconnect, Validate, etc)
		return true;
	}

	PeerID NetworkingManager::Connect(const NetAddress& netAddress)
	{
		// Create new potential peer.
		Peer newPeer(GetNextAvailablePeerID(), netAddress);

		// Make sure we don't have a duplicate peer looking at this same address.
		for (const auto& peer : m_connectedPeers)
		{
			// Compare Peer addresses and ports (Not PeerID's).
			if (newPeer == peer)
				return PeerID_Invalid; // This peer already exists, so just bail. TODO: Send Connect failed.
		}

		SocketManager* pSocketManager = SocketManager::GetInstance();
		EXE_ASSERT(pSocketManager);

		if (!pSocketManager->ConnectPeer(newPeer))
		{
			// If we get here, an error has occurred. Just bail completely. An on connection failed message as already been sent.
			// TODO: Log error.
			return newPeer.m_id; // We don't return an invalid Id because we wouldn't catch the connect failed message.
		}

		// We add this peer to the list, since we are going to wait for a connection to occur.
		// We need to make sure that we don't wait on too many connections at once,
		// and they need to be "cleaned" out if they have been around too long without being accepted.
		m_connectedPeers.emplace_back(newPeer); // std::move here?

		return newPeer.m_id;
		// If success, then we must attempt to validate ourselves.
			// "Host" will send an "encripted" message that we will receive,
			// translate, and send an appropriate response back, which the "Host" with then send
			// an Accepted (with hosts ID) or Rejected response with reason for reject. (with their peer id and my new one).
			// Peer State == Unvalidated until an Accepted message is received (may contain new peer ID as dictated by the host).
			// Which will set state to Valid and open UDP connection on newPeer.port + 1 with same newPeer.address.
		// Else, cancel new peer, and open a listen tcp socket on given address and port and udp on given address and port + 1.
	}

	void NetworkingManager::DisconnectPeer(PeerID peerIdToDisconnect)
	{
		// If ID is invalid, then we dont need to do anything.
		if (peerIdToDisconnect == PeerID_Invalid)
			return;

		for (auto& peer : m_connectedPeers)
		{
			if (peer.m_id == peerIdToDisconnect)
			{
				SocketManager* pSocketManager = SocketManager::GetInstance();
				EXE_ASSERT(pSocketManager);

				pSocketManager->DisconnectPeer(peer);
			}
		}
	}

	void NetworkingManager::DisconnectAll()
	{
		SocketManager* pSocketManager = SocketManager::GetInstance();
		EXE_ASSERT(pSocketManager);
		
		pSocketManager->DisconnectAll();
		m_connectedPeers.clear();
	}

	PeerID NetworkingManager::GetNextAvailablePeerID()
	{
		return ++m_peerIDCounter;
	}

	void NetworkingManager::HandleLocalOnlyMessage(Message* pMsg)
	{
		// Send child message to the message server.
		if (pMsg)
			MessageServer::GetInstance()->PushMessage(pMsg);
	}

	void NetworkingManager::HandleUnreliableBroadcast(Message* pMsg)
	{
		for (auto& peer : m_connectedPeers)
		{
			peer.SendUnreliableMessage(pMsg);
		}
	}

	void NetworkingManager::HandleReliableBroadcast(Message* pMsg)
	{
		for (auto& peer : m_connectedPeers)
		{
			peer.SendReliableMessage(pMsg);
		}
	}

	void NetworkingManager::HandleUnreliableMessage(Message* pMsg, PeerID toID)
	{
		for (auto& peer : m_connectedPeers)
		{
			if (peer.m_id == toID)
			{
				peer.SendUnreliableMessage(pMsg);
				break; // There should never be 2 peers with the same ID.
			}
		}
	}

	void NetworkingManager::HandleReliableMessage(Message* pMsg, PeerID toID)
	{
		for (auto& peer : m_connectedPeers)
		{
			if (peer.m_id == toID)
			{
				peer.SendReliableMessage(pMsg);
				break; // There should never be 2 peers with the same ID.
			}
		}
	}
}
