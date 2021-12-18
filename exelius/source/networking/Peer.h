#pragma once
#include "source/networking/NetAddress.h"
#include "source/networking/NetHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Message;
	class Socket;

	class Peer
	{
		friend bool operator ==(const Peer& left, const Peer& right);
	public:

		enum class ConnectionState
		{
			NoConnection,			// This Peer has either been disconnect, not initialized, or was unable to connnect.
			UnvalidatedConnection,	// Peer has been connected, but has not yet been validated with the "host" connection.
			ValidConnection			// Peer is connected and has been successfully validated.
		};

		// The current state of this peers connection.
		ConnectionState m_connectionState;

		// The peers unique address.
		NetAddress m_netAddress;

		Socket* m_pReliableSocket;
		Socket* m_pUnreliableSocket;

		// The unique peer ID.
		PeerID m_id;

		Peer(PeerID id, const NetAddress& netAddress);
		Peer(const Peer& other) = default;
		Peer(Peer&&) = default;
		Peer& operator=(const Peer&) = delete;
		Peer& operator=(Peer&&) = delete;

		void SendReliableMessage(Message* pMsg);

		void SendUnreliableMessage(Message* pMsg);
	};

	bool operator ==(const Peer& left, const Peer& right);
}