#pragma once
#include "source/networking/NetAddress.h"
#include "source/networking/NetHelpers.h"

#include <EASTL/vector.h>

#include <mutex>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Message;

	class Socket
	{
		static constexpr uint16_t s_kMaxUDPDataPacketSize = 65507;
	public:
		enum class Status
		{
			Done,         // The socket has sent / received the data
			NotReady,     // The socket is not ready to send / receive data yet
			Partial,      // The socket sent a part of the data
			Disconnected, // The TCP socket has been disconnected
			Error         // An unexpected error happened
		};

		enum class SocketType
		{
			TCP,
			UDP
		};

	private:
		SocketType m_type;

	public:
		// The handle belonging to this actual socket.
		SocketHandle m_socket;

		// The address associated with this socket.
		NetAddress m_netAddress;

		// The peer id associated with this socket.
		PeerID m_peerID;

		// The data received by this socket.
		eastl::vector<char> m_receiveBuffer;

		eastl::vector<Message*> m_sendBuffer;

		std::mutex m_recvLock;
		std::mutex m_sendLock;

		// Is this socket currently blocking.
		bool m_isBlocking;

		Socket(SocketType type, const NetAddress& netAddress, PeerID id);

		Status Connect(float timeOut = 0.0f);

		Status ReceiveIncomingMessage();

		Status SendOutgoingMessages();

		void StoreMessage(Message* pMsg);

		void CloseSocket();

		bool IsValid() const { return m_socket != SocketHandle_Invalid; }

	private:
		void CreateSocket();

		void CreateSocket(SocketHandle sock);

		Status Listen();

		Status Accept(Socket& sock);

		Status Bind();

		Status Send(Message* pMsg);

		Status Send(const void* data, std::size_t size, std::size_t& sent);

		Status Receive();

		Status Receive(void* data, std::size_t size, std::size_t& received);

		Socket::Status GetErrorStatus();

		NetAddress GetRemoteAddress() const;

		unsigned short GetRemotePort() const;

		void SetBlocking(bool toSetBlocking);

		struct PendingMessage
		{
			uint32_t Size;
			std::size_t SizeReceived;
			std::vector<char> Data;

			PendingMessage();
		};

		PendingMessage m_pendingMessage;
	};
}
