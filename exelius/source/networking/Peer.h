#pragma once
#include "source/networking/NetAddress.h"
#include "source/networking/NetHelpers.h"

#include <EASTL/shared_ptr.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Message;
	class Socket;

	class Peer
	{
		enum class ConnectionState
		{
			NoConnection,			// This Peer has either been disconnect, not initialized, or was unable to connnect.
			UnvalidatedConnection,	// Peer has been connected, but has not yet been validated with the "host" connection.
			ValidConnection			// Peer is connected and has been successfully validated.
		};

	private:
		friend bool operator ==(const Peer& left, const Peer& right);

		eastl::shared_ptr<Socket> m_pReliableSocket;
		eastl::shared_ptr<Socket> m_pUnreliableSocket;

		// The current state of this peers connection.
		ConnectionState m_connectionState;

		// The peers unique address.
		NetAddress m_netAddress;

		// The unique peer ID.
		PeerID m_id;
	public:
		Peer(PeerID id, const NetAddress& netAddress);
		Peer(const Peer& other) = default;
		Peer(Peer&&) = default;
		Peer& operator=(const Peer&) = delete;
		Peer& operator=(Peer&&) = delete;

		void InitializePeer();

		void SendReliableMessage(Message* pMsg);

		void SendUnreliableMessage(Message* pMsg);

		const eastl::shared_ptr<Socket>& GetReliableSocket() const { return m_pReliableSocket; }
		const eastl::shared_ptr<Socket>& GetUnreliableSocket() const { return m_pUnreliableSocket; }
		PeerID GetPeerID() const { return m_id; }
		void SetPeerID(PeerID newID) { m_id = newID; }
	};

	bool operator ==(const Peer& left, const Peer& right);
}