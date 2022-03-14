#include "EXEPCH.h"
#include "Peer.h"

#include "source/messages/Message.h"
#include "source/networking/Socket.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	bool operator==(const Peer& left, const Peer& right)
	{
		if (left.m_pReliableSocket->m_netAddress == right.m_netAddress)
			return true;
		else if (left.m_pUnreliableSocket->m_netAddress == right.m_netAddress)
			return true;

		return false;
	}

	Peer::Peer(PeerID id, const NetAddress& netAddress)
		: m_connectionState(ConnectionState::NoConnection)
		, m_netAddress(netAddress)
		, m_id(id)
		, m_pReliableSocket(nullptr)
		, m_pUnreliableSocket(nullptr)
	{
		//
	}

	void Peer::InitializePeer()
	{
		m_pReliableSocket = MakeShared<Socket>(Socket::SocketType::TCP, m_netAddress, m_id);
		NetAddress unreliableAddr(m_netAddress.ToString(), m_netAddress.GetPort() + 1);
		m_pUnreliableSocket = MakeShared<Socket>(Socket::SocketType::UDP, unreliableAddr, m_id);
	}

	void Peer::SendReliableMessage(Message* pMsg)
	{
		m_pReliableSocket->StoreMessage(pMsg);
	}

	void Peer::SendUnreliableMessage(Message* pMsg)
	{
		m_pUnreliableSocket->StoreMessage(pMsg);
	}
}
