#pragma once
#include "source/utility/generic/Singleton.h"
#include "source/networking/NetMessage.h"
#include "source/networking/Peer.h"

#include "source/messages/MessageServer.h"
#include "source/messages/MessageFactory.h"

#include "source/networking/NetAddress.h"

#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class MessageReceiver;
	class NetworkingManager
		: public Singleton<NetworkingManager>
	{
		eastl::vector<Peer> m_connectedPeers;
		MessageFactory* m_pMessageFactory;
		eastl::array<eastl::shared_ptr<MessageReceiver>, 5> m_listeners;

		PeerID m_peerIDCounter;
	public:
		NetworkingManager();

		~NetworkingManager();

		bool Initialize(MessageFactory* pMessageFactory);

		PeerID Connect(const NetAddress& netAddress);

		// Disconnect other
		void DisconnectPeer(PeerID peerIdToDisconnect);

		// Disconnect all current peers.
		void DisconnectAll();

	private:
		PeerID GetNextAvailablePeerID();

		// Called by MessageServer on NetMessage Dispatch
		template <NetProtocol Protocol>
		void HandleNetMessage(NetMessage<Protocol>* pMessage)
		{
			// Extract and construct the child message.
			// If this crashes, it is probably due to a hash collision and an incorrect pointer reinterpretation. (undefined behavior)
			Message* pEmbeddedMessage = m_pMessageFactory->CreateMessage(pMessage->GetMessageID(), pMessage->GetDataPacket());
			switch (Protocol)
			{
			case NetProtocol::LocalOnly:			HandleLocalOnlyMessage(pEmbeddedMessage); break;
			case NetProtocol::BroadcastUnreliable:	HandleUnreliableBroadcast(pEmbeddedMessage); break;
			case NetProtocol::BroadcastReliable:	HandleReliableBroadcast(pEmbeddedMessage); break;
			case NetProtocol::Unreliable:			HandleUnreliableMessage(pEmbeddedMessage, pMessage->m_toPeerID); break;
			case NetProtocol::Reliable:				HandleReliableMessage(pEmbeddedMessage, pMessage->m_toPeerID); break;
			default:								HandleLocalOnlyMessage(pEmbeddedMessage); break;
			}
		}

		// TODO: This *might* present an issue with this call being made during the MessageServer::DispatchMessages function ("iterator invalidation").
		// It might also be a really awesome feature.
		static void HandleLocalOnlyMessage(Message* pMsg);

		void HandleUnreliableBroadcast(Message* pMsg);

		void HandleReliableBroadcast(Message* pMsg);

		void HandleUnreliableMessage(Message* pMsg, PeerID toID);

		void HandleReliableMessage(Message* pMsg, PeerID toID);
	};
}