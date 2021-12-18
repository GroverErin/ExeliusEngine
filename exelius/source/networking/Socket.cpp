#include "EXEPCH.h"
#include "Socket.h"

#include "source/messages/Message.h"
#include "source/networking/NetMessage.h"

#ifdef EXE_WINDOWS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
#if EXE_WINDOWS
	typedef int AddrLength;
	static constexpr int flags = 0;
#else
	typedef socklen_t AddrLength;
	static constexpr int flags = MSG_NOSIGNAL;
#endif

	Socket::Socket(SocketType type, const NetAddress& netAddress, PeerID id)
		: m_socket(SocketHandle_Invalid)
		, m_isBlocking(true)
		, m_type(type)
		, m_netAddress(netAddress)
		, m_peerID(id)
	{
		if (type == SocketType::UDP)
			m_receiveBuffer.resize(s_kMaxUDPDataPacketSize);
	}
	
	void Socket::CreateSocket()
	{
		// Don't create the socket if it already exists
		if (m_socket == SocketHandle_Invalid)
		{
			SocketHandle handle = socket(PF_INET, m_type == SocketType::TCP ? SOCK_STREAM : SOCK_DGRAM, 0);

			if (handle == SocketHandle_Invalid)
			{
				// TODO: Log error.
				return;
			}

			CreateSocket(handle);
		}
	}
	
	void Socket::CreateSocket(SocketHandle sock)
	{
		// Don't create the socket if it already exists
		if (m_socket == SocketHandle_Invalid)
		{
			// Assign the new handle
			m_socket = sock;

			// Set the current blocking state
			SetBlocking(m_isBlocking);

			if (m_type == SocketType::TCP)
			{
				// Disable the Nagle algorithm (i.e. removes buffering of TCP packets)
				int yes = 1;
				if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
				{
					// TODO: log Error.
				}
			}
			else
			{
				// Enable broadcast by default for UDP sockets
				int yes = 1;
				if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
				{
					// TODO: log Error.
				}
			}
		}
	}
	
	NetAddress Socket::GetRemoteAddress() const
	{
		EXE_ASSERT(m_type == SocketType::TCP);

		if (m_socket != SocketHandle_Invalid)
		{
			// Retrieve informations about the remote end of the socket
			sockaddr_in address;
			AddrLength size = sizeof(address);
			if (getpeername(m_socket, reinterpret_cast<sockaddr*>(&address), &size) != -1)
			{
				return { inet_ntoa(address.sin_addr), ntohs(address.sin_port) };
			}
		}

		// We failed to retrieve the address
		return { NetAddressDefines::None, 0 };
	}

	unsigned short Socket::GetRemotePort() const
	{
		EXE_ASSERT(m_type == SocketType::TCP);

		if (m_socket != SocketHandle_Invalid)
		{
			// Retrieve informations about the remote end of the socket
			sockaddr_in address;
			AddrLength size = sizeof(address);
			if (getpeername(m_socket, reinterpret_cast<sockaddr*>(&address), &size) != -1)
			{
				return ntohs(address.sin_port);
			}
		}

		// We failed to retrieve the port
		return 0;
	}
	
	Socket::Status Socket::Listen()
	{
		// Check if the address is valid
		// TODO: Fix None case.
		if ((m_netAddress.ToString() == NetAddressDefines::None) || (m_netAddress.ToString() == NetAddressDefines::Broadcast_IPV4))
		{
			return Status::Error;
		}

		sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = htonl(m_netAddress.ToInteger());
		addr.sin_family = AF_INET;
		addr.sin_port = htons(m_netAddress.GetPort());

		// Bind the socket to the specified port
		if (bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
		{
			// TODO: Log error.
			return Status::Error;
		}

		// Listen to the bound port
		if (::listen(m_socket, SOMAXCONN) == -1)
		{
			// TODO: Log error.
			return Status::Error;
		}

		return Status::Done;
	}
	
	Socket::Status Socket::Accept(Socket& sock)
	{
		EXE_ASSERT(m_type == SocketType::TCP);

		// Make sure that we're listening
		if (m_socket == SocketHandle_Invalid)
		{
			// TODO: Log error.
			sock.CloseSocket();
			return Status::Error;
		}

		// Accept a new connection
		sockaddr_in address;
		AddrLength length = sizeof(address);
		SocketHandle remote = ::accept(m_socket, reinterpret_cast<sockaddr*>(&address), &length);

		// Check for errors
		if (remote == SocketHandle_Invalid)
		{
			sock.CloseSocket();
			return GetErrorStatus();
		}

		// Initialize the new connected socket
		sock.CloseSocket();
		sock.CreateSocket(remote);

		return Status::Done;
	}
	
	Socket::Status Socket::Connect(float timeOut)
	{
		// Disconnect the socket if it is already connected
		CloseSocket();

		// Create the internal socket if it doesn't exist
		CreateSocket();

		if (m_type == SocketType::UDP)
			return Bind();

		EXE_ASSERT(m_type == SocketType::TCP);

		if (m_netAddress.ToString() == NetAddressDefines::Any_IPV4 || m_netAddress.ToString() == NetAddressDefines::Any_IPV6 || m_netAddress.ToString() == NetAddressDefines::None)
		{
			Socket::Status status = Listen();
			return status;
		}

		// Create the remote address
		sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = htonl(m_netAddress.ToInteger());
		addr.sin_family = AF_INET;
		addr.sin_port = htons(m_netAddress.GetPort());

		if (timeOut <= 0.0f)
		{
			// ----- We're not using a timeout: just try to connect -----

			// Connect the socket
			if (::connect(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
			{
				return GetErrorStatus();
			}

			// Connection succeeded
			return Status::Done;
		}
		else
		{
			// ----- We're using a timeout: we'll need a few tricks to make it work -----

			// Save the previous blocking state
			bool blocking = m_isBlocking;

			// Switch to non-blocking to enable our connection timeout
			if (blocking)
				SetBlocking(false);

			// Try to connect to the remote address
			if (::connect(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) >= 0)
			{
				// We got instantly connected! (it may no happen a lot...)
				SetBlocking(blocking);
				return Status::Done;
			}

			// Get the error status
			Status status = GetErrorStatus();

			// If we were in non-blocking mode, return immediately
			if (!blocking)
			{
				return status;
			}

			// Otherwise, wait until something happens to our socket (success, timeout or error)
			if (status == Status::NotReady)
			{
				// Setup the selector
				fd_set selector;
				FD_ZERO(&selector);
				FD_SET(m_socket, &selector);

				// Setup the timeout
				timeval time;
				time.tv_sec = static_cast<long>(timeOut / 1000000);
				time.tv_usec = static_cast<long>(static_cast<unsigned long long>(timeOut) % 1000000);

				// Wait for something to write on our socket (which means that the connection request has returned)
				if (select(static_cast<int>(m_socket + 1), NULL, &selector, NULL, &time) > 0)
				{
					// At this point the connection may have been either accepted or refused.
					// To know whether it's a success or a failure, we must check the address of the connected peer
					if (GetRemoteAddress().ToString() != NetAddressDefines::None)
					{
						// Connection accepted
						status = Status::Done;
					}
					else
					{
						// Connection refused
						status = GetErrorStatus();
					}
				}
				else
				{
					// Failed to connect before timeout is over
					status = GetErrorStatus();
				}
			}

			// Switch back to blocking mode
			SetBlocking(true);

			return status;
		}
	}

	Socket::Status Socket::ReceiveIncomingMessage()
	{
		// call recv and store the data
		return Receive();
	}

	Socket::Status Socket::SendOutgoingMessages()
	{
		// call send and send the stored out data
		m_sendLock.lock();
		for (auto* pMsg : m_sendBuffer)
		{
			Status status = Send(pMsg);
			if (status == Status::Disconnected || status == Status::Error)
			{
				m_sendLock.unlock();
				EXELIUS_DELETE(pMsg);
				return status;
			}
			EXELIUS_DELETE(pMsg);
		}
		m_sendBuffer.clear();
		m_sendLock.unlock();
		return Status::Done;
	}

	void Socket::StoreMessage(Message* pMsg)
	{
		m_sendLock.lock();
		m_sendBuffer.emplace_back(pMsg);
		m_sendLock.unlock();
	}

	Socket::Status Socket::Bind()
	{
		EXE_ASSERT(m_type == SocketType::UDP);

		// Check if the address is valid
		// TODO: Fix these address cases.
		if ((m_netAddress.ToString() == NetAddressDefines::None) || (m_netAddress.ToString() == NetAddressDefines::Broadcast_IPV4))
		{
			return Status::Error;
		}

		// Bind the socket
		sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = htonl(m_netAddress.ToInteger());
		addr.sin_family = AF_INET;
		addr.sin_port = htons(m_netAddress.GetPort() + 1);
		if (::bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1)
		{
			// TODO: Log error
			return GetErrorStatus();
		}

		return Status::Done;
	}

	Socket::Status Socket::Send(Message* pMsg)
	{
		// Get the data to send from the packet
		std::size_t size = 0;
		const void* data = pMsg->GetDataPacket().data();

		// First convert the packet size to network byte order
		uint32_t packetSize = htonl(static_cast<uint32_t>(size));

		// Allocate memory for the data block to send
		std::vector<char> blockToSend(sizeof(packetSize) + size);

		// Copy the packet size and data into the block to send
		std::memcpy(&blockToSend[0], &packetSize, sizeof(packetSize));
		if (size > 0)
			std::memcpy(&blockToSend[0] + sizeof(packetSize), data, size);

		// Send the data block
		std::size_t sent;
		Status status = Send(&blockToSend[0] + pMsg->m_sendPos, blockToSend.size() - pMsg->m_sendPos, sent);

		// In the case of a partial send, record the location to resume from
		if (status == Status::Partial)
		{
			pMsg->m_sendPos += sent;
		}
		else if (status == Status::Done)
		{
			pMsg->m_sendPos = 0;
		}

		return status;
	}

	Socket::Status Socket::Send(const void* data, std::size_t size, std::size_t& sent)
	{
		// Check the parameters
		if (!data || (size == 0))
		{
			printf("Cannot send data over the network (no data to send)");
			return Status::Error;
		}

		// Loop until every byte has been sent
		int result = 0;
		for (sent = 0; sent < size; sent += result)
		{
			// Send a chunk of data
			result = ::send(m_socket, static_cast<const char*>(data) + sent, static_cast<int>(size - sent), 0);

			// Check for errors
			if (result < 0)
			{
				Status status = GetErrorStatus();

				if ((status == Status::NotReady) && sent)
					return Status::Partial;

				return status;
			}
		}

		return Status::Done;
	}

	Socket::Status Socket::Receive()
	{
		// First clear the variables to fill
		//packet.Clear();

		// We start by getting the size of the incoming packet
		uint32_t packetSize = 0;
		std::size_t received = 0;
		if (m_pendingMessage.SizeReceived < sizeof(m_pendingMessage.Size))
		{
			// Loop until we've received the entire size of the packet
			// (even a 4 byte variable may be received in more than one call)
			while (m_pendingMessage.SizeReceived < sizeof(m_pendingMessage.Size))
			{
				char* data = reinterpret_cast<char*>(&m_pendingMessage.Size) + m_pendingMessage.SizeReceived;
				Status status = Receive(data, sizeof(m_pendingMessage.Size) - m_pendingMessage.SizeReceived, received);
				m_pendingMessage.SizeReceived += received;

				if (status != Status::Done)
					return status;
			}

			// The packet size has been fully received
			packetSize = ntohl(m_pendingMessage.Size);
		}
		else
		{
			// The packet size has already been received in a previous call
			packetSize = ntohl(m_pendingMessage.Size);
		}

		// Loop until we receive all the packet data
		char buffer[1024];
		while (m_pendingMessage.Data.size() < packetSize)
		{
			// Receive a chunk of data
			std::size_t sizeToGet = eastl::min(static_cast<std::size_t>(packetSize - m_pendingMessage.Data.size()), sizeof(buffer));
			Status status = Receive(buffer, sizeToGet, received);
			if (status != Status::Done)
				return status;

			// Append it into the packet
			if (received > 0)
			{
				m_pendingMessage.Data.resize(m_pendingMessage.Data.size() + received);
				char* begin = &m_pendingMessage.Data[0] + m_pendingMessage.Data.size() - received;
				std::memcpy(begin, buffer, received);
			}
		}

		// We have received all the packet data: we can copy it to the user packet
		//if (!m_pendingMessage.Data.empty())
			//packet.OnReceive(&m_pendingMessage.Data[0], m_pendingMessage.Data.size());

		// Clear the pending packet data
		m_pendingMessage = PendingMessage();

		return Status::Done;
	}

	Socket::Status Socket::Receive(void* data, std::size_t size, std::size_t& received)
	{
		// First clear the variables to fill
		received = 0;

		// Check the destination buffer
		if (!data)
		{
			printf("Cannot receive data from the network (the destination buffer is invalid)");
			return Status::Error;
		}

		// Receive a chunk of bytes
		int sizeReceived = recv(m_socket, static_cast<char*>(data), static_cast<int>(size), 0);

		// Check the number of bytes received
		if (sizeReceived > 0)
		{
			received = static_cast<std::size_t>(sizeReceived);
			return Status::Done;
		}
		else if (sizeReceived == 0)
		{
			return Status::Disconnected;
		}
		else
		{
			return GetErrorStatus();
		}
	}

	void Socket::CloseSocket()
	{
		if (m_socket == SocketHandle_Invalid)
			return;

		#if EXE_WINDOWS
		closesocket(m_socket);
		#else
		::close(m_socket);
		#endif

		m_socket = SocketHandle_Invalid;
	}

	void Socket::SetBlocking(bool toSetBlocking)
	{
		#if EXE_WINDOWS
		u_long blocking = toSetBlocking ? 0 : 1;
		ioctlsocket(m_socket, FIONBIO, &blocking);
		#else
		int status = fcntl(m_socket, F_GETFL);
		if (toSetBlocking)
		{
			if (fcntl(m_socket, F_SETFL, status & ~O_NONBLOCK) == -1)
				return; // TODO: Log an error here.
		}
		else
		{
			if (fcntl(m_socket, F_SETFL, status | O_NONBLOCK) == -1)
				return; // TODO: Log an error here.
		}
		#endif
	}

	Socket::Status Socket::GetErrorStatus()
	{
		#if EXE_WINDOWS
		switch (int val = WSAGetLastError())
		{
		case WSAEWOULDBLOCK:  return Socket::Status::NotReady;
		case WSAEALREADY:     return Socket::Status::NotReady;
		case WSAECONNABORTED: return Socket::Status::Disconnected;
		case WSAECONNRESET:   return Socket::Status::Disconnected;
		case WSAETIMEDOUT:    return Socket::Status::Disconnected;
		case WSAENETRESET:    return Socket::Status::Disconnected;
		case WSAENOTCONN:     return Socket::Status::Disconnected;
		case WSAEISCONN:      return Socket::Status::Done; // when connecting a non-blocking socket
		default:              return Socket::Status::Error;
		}
		#else
		if ((errno == EAGAIN) || (errno == EINPROGRESS))
			return Socket::NotReady;

		switch (errno)
		{
		case EWOULDBLOCK:  return Socket::Status::NotReady;
		case ECONNABORTED: return Socket::Status::Disconnected;
		case ECONNRESET:   return Socket::Status::Disconnected;
		case ETIMEDOUT:    return Socket::Status::Disconnected;
		case ENETRESET:    return Socket::Status::Disconnected;
		case ENOTCONN:     return Socket::Status::Disconnected;
		case EPIPE:        return Socket::Status::Disconnected;
		default:           return Socket::Status::Error;
		}
		#endif
	}

	Socket::PendingMessage::PendingMessage()
		: Size(0)
		, SizeReceived(0)
		, Data()
	{
		//
	}
}
