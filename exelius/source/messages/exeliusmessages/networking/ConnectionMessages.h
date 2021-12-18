#pragma once
#include "source/messages/Message.h"
#include "source/networking/NetHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class ConnectedMessage
		: public Message
	{
	public:
		PeerID m_id;

		ConnectedMessage(PeerID id)
			: Message(DEFINE_MESSAGE(ConnectedMessage))
			, m_id(id)
		{
			//
		}
	};

	class ValidateMessage
		: public Message
	{
	public:
		ValidateMessage()
			: Message(DEFINE_MESSAGE(ValidateMessage))
		{
			//
		}
	};

	class AcceptedMessage
		: public Message
	{
	public:
		AcceptedMessage()
			: Message(DEFINE_MESSAGE(AcceptedMessage))
		{
			//
		}
	};

	class RejectedMessage
		: public Message
	{
	public:
		RejectedMessage()
			: Message(DEFINE_MESSAGE(RejectedMessage))
		{
			//
		}
	};

	class DisconnectedMessage
		: public Message
	{
	public:
		PeerID m_id;

		DisconnectedMessage(PeerID id)
			: Message(DEFINE_MESSAGE(DisconnectedMessage))
			, m_id(id)
		{
			//
		}
	};

	class ConnectionFailedMessage
		: public Message
	{
	public:
		PeerID m_id;

		ConnectionFailedMessage(PeerID id)
			: Message(DEFINE_MESSAGE(ConnectionFailedMessage))
			, m_id(id)
		{
			//
		}
	};
}