#pragma once
#include "source/networking/NetAddress.h"
#include "source/networking/NetHelpers.h"

#include <EASTL/array.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Message;
	class MessageReceiver;

	class Connection
	{
		NetAddress m_connectionAddress;
		PeerID m_id; // This connection is associated with a Peer (Self, not the remote Connection).

		eastl::array<eastl::shared_ptr<MessageReceiver>, 6> m_listeners;
	public:
		// Registers for all the appropriate callbacks.
		Connection(const NetAddress& address);

		// Closes the connection.
		virtual ~Connection();

		// Begin a connection with the address given in the constructor.
		void Connect();

		// Close the connection with the address.
		void Disconnect();

		PeerID GetPeerID() const { return m_id; }

		static void DisconnectPeer(PeerID idToDisconnect = PeerID_Invalid);
		static void DisconnectAll();

	protected:
		// Called when a connection is successful to a remote host.
		virtual void OnConnected(Message* pMsg) = 0;

		// As "Client":
		//		Called after a "Host" sends a validation request message.
		// As "Host":
		//		Called after a "client sends a validation response.
		virtual void OnValidate(Message* pMsg) = 0;

		// Called when a validation accepted response is received.
		virtual void OnAccepted(Message* pMsg) = 0;

		// Called when a validation denied response is received.
		virtual void OnRejected(Message* pMsg) = 0;

		// Called when a send or receive attempt fails, thus signaling the connection is no longer valid.
		// Note: The connection will have already been closed and cleaned up upon this function call.
		virtual void OnDisconnected(Message* pMsg) = 0;

		// Called when an error or timeout occures.
		virtual void OnConnectionFailed(Message* pMsg) = 0;
	};
}