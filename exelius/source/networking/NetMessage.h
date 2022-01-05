#pragma once
#include "source/messages/Message.h"
#include "source/networking/NetHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	enum class NetProtocol
	{
		LocalOnly			= 0, // Only transmit within local client. This is not necessary, but useful for debugging.
		BroadcastUnreliable = 1, // Broadcast to all connected Peers via unreliable (UDP) sockets.
		BroadcastReliable	= 2, // Broadcast to all connected Peers via reliable (TCP) sockets.
		Unreliable			= 3, // Transmit to a single Peer via an unreliable (UDP) socket.
		Reliable			= 4  // Transmit to a single Peer via a reliable (TCP) socket.
	};

	template <NetProtocol Protocol>
	class NetMessage
		: public Message
	{
		static constexpr MessageID ComputeStaticID()
		{
			switch (Protocol)
			{
			case NetProtocol::LocalOnly:			return DEFINE_MESSAGE(NetMessage<NetProtocol::LocalOnly>);
			case NetProtocol::BroadcastUnreliable:	return DEFINE_MESSAGE(NetMessage<NetProtocol::BroadcastUnreliable>);
			case NetProtocol::BroadcastReliable:	return DEFINE_MESSAGE(NetMessage<NetProtocol::BroadcastReliable>);
			case NetProtocol::Unreliable:			return DEFINE_MESSAGE(NetMessage<NetProtocol::Unreliable>);
			case NetProtocol::Reliable:				return DEFINE_MESSAGE(NetMessage<NetProtocol::Reliable>);
			default:								return DEFINE_MESSAGE(NetMessage<NetProtocol::LocalOnly>);
			}
		}

		static constexpr MessageID m_netID = ComputeStaticID();
		MessageID m_childMessageID;
	public:
		static constexpr NetProtocol s_kProtocol = Protocol;

		PeerID m_toPeerID;

		NetMessage(MessageID id)
			: Message(m_netID)
			, m_childMessageID(id)
		{
			//
		}

		MessageID GetChildMessageID() { return m_childMessageID; }
	};
}