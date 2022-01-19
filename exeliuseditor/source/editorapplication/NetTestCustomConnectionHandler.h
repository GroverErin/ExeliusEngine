#pragma once
#include <source/networking/Connection.h>
#include <source/messages/exeliusmessages/networking/ConnectionMessages.h>
#include <iostream>

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


		Exelius::ConnectedMessage* pConMsg = reinterpret_cast<Exelius::ConnectedMessage*>(pMsg);
		eastl::string str = pConMsg->m_address.ToString(true);
		if (pConMsg)
			std::cout << "Connected PeerID: " << str.c_str() << "\n";

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