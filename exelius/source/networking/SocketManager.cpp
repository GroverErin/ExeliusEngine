#include "EXEPCH.h"
#include "SocketManager.h"
#include "Socket.h"

#include "source/networking/Peer.h"

#include "source/messages/MessageServer.h"
#include "source/messages/exeliusmessages/networking/ConnectionMessages.h"

#ifdef EXE_WINDOWS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class SocketManager::SocketData
	{
	public:
		fd_set m_allSockets;	// Set containing all the sockets handles
		fd_set m_readSockets;	// Set containing handles of the sockets that are ready for reading.
		fd_set m_writeSockets;	// Set containing handles of the sockets that are ready for writing.
		fd_set m_excepts;		// Caught errors
		int    m_maxSocket;		// Maximum socket handle
		int    m_socketCount;	// Number of socket handles

		SocketData()
			: m_maxSocket(0)
			, m_socketCount(0)
		{
			FD_ZERO(&m_allSockets);
			FD_ZERO(&m_readSockets);
			FD_ZERO(&m_writeSockets);
			FD_ZERO(&m_excepts);
		}

		void AddSocketData(SocketHandle sockHandle)
		{
			if (sockHandle != SocketHandle_Invalid)
			{
				#if EXE_WINDOWS
				if (m_socketCount >= FD_SETSIZE)
				{
					// TODO: Log Error.
					return;
				}

				if (FD_ISSET(sockHandle, &m_allSockets))
					return;

				m_socketCount++;
				#else
				if (sockHandle >= FD_SETSIZE)
				{
					// TODO: Log Error.
					return;
				}

				// SocketHandle is an int in POSIX
				m_maxSocket = std::max(m_maxSocket, sockHandle);
				#endif

				FD_SET(sockHandle, &m_allSockets);
			}
		}

		void RemoveSocketData(SocketHandle sockHandle)
		{
			if (sockHandle != SocketHandle_Invalid)
			{
				#if defined(EXE_WINDOWS)
				if (!FD_ISSET(sockHandle, &m_allSockets))
					return;
				m_socketCount--;
				#else
				if (sockHandle >= FD_SETSIZE)
					return;
				#endif

				FD_CLR(sockHandle, &m_allSockets);
				FD_CLR(sockHandle, &m_readSockets);
				FD_CLR(sockHandle, &m_writeSockets);
				FD_CLR(sockHandle, &m_excepts);
			}
		}

		void ClearSocketData()
		{
			FD_ZERO(&m_allSockets);
			FD_ZERO(&m_readSockets);
			FD_ZERO(&m_writeSockets);
			FD_ZERO(&m_excepts);

			m_maxSocket = 0;
			m_socketCount = 0;
		}
	};

	SocketManager::SocketManager()
		: m_pSocketData(EXELIUS_NEW(SocketData()))
		, m_quitThread(false)
	{
		EXE_ASSERT(m_pSocketData);

		// Spin up the networking thread.
		m_socketSelectThread = std::thread(&SocketManager::SelectSockets, this);
	}

	SocketManager::~SocketManager()
	{
		m_quitThread = true;
		m_socketSelectThread.join();

		EXELIUS_DELETE(m_pSocketData);
	}

	void SocketManager::SelectSockets()
	{
		MessageServer* pMessageServer = MessageServer::GetInstance();
		EXE_ASSERT(pMessageServer);

		while (!m_quitThread)
		{
			// Recreate the SocketData so it is cleared.
			SocketData threadSocketData;
			SwapSocketData(threadSocketData);

			HandleNewConnections();

			HandleClosedConnections();

			if (!Select(threadSocketData))
				continue;

			HandleCurrentSockets(threadSocketData);
		}
	}

	void SocketManager::RegisterPeerSockets(const Peer& peerToConnect)
	{
		EXE_ASSERT(peerToConnect.GetReliableSocket());
		EXE_ASSERT(peerToConnect.GetUnreliableSocket());

		m_socketLock.lock();
		m_socketsToAdd.emplace_back(peerToConnect.GetReliableSocket());
		m_socketsToAdd.emplace_back(peerToConnect.GetUnreliableSocket());
		m_socketLock.unlock();
	}
	
	void SocketManager::DisconnectPeer(const Peer& peerToDisconnect)
	{
		EXE_ASSERT(peerToDisconnect.GetReliableSocket());
		EXE_ASSERT(peerToDisconnect.GetUnreliableSocket());

		m_socketLock.lock();
		m_socketsToRemove.emplace_back(peerToDisconnect.GetReliableSocket());
		m_socketsToRemove.emplace_back(peerToDisconnect.GetUnreliableSocket());
		m_socketLock.unlock();
	}

	void SocketManager::DisconnectAll()
	{
		MessageServer* pMessageServer = MessageServer::GetInstance();
		EXE_ASSERT(pMessageServer);

		m_socketLock.lock();
		for (auto& pSock : m_socketsToAdd)
		{
			pMessageServer->PushMessage(EXELIUS_NEW(ConnectionFailedMessage(pSock->m_peerID)));
		}
		m_socketsToAdd.clear();

		for (auto& sock : m_sockets)
		{
			if (!sock)
				continue;
			m_socketsToRemove.emplace_back(sock);
		}
		m_socketLock.unlock();
	}

	bool SocketManager::Select(SocketData& socketData, float timeOut)
	{
		timeval time;
		time.tv_sec = static_cast<long>(timeOut / 1000000);
		time.tv_usec = static_cast<long>(static_cast<unsigned long long>(timeOut) % 1000000);

		int count = select(socketData.m_maxSocket + 1, &socketData.m_readSockets, &socketData.m_writeSockets, &socketData.m_excepts, timeOut != 0.0f ? &time : nullptr);

		return count > 0;
	}

	void SocketManager::SwapSocketData(SocketData& outData)
	{
		m_socketLock.lock();
		outData.m_readSockets = m_pSocketData->m_allSockets;
		outData.m_writeSockets = m_pSocketData->m_allSockets;
		outData.m_excepts = m_pSocketData->m_allSockets;
		outData.m_maxSocket = m_pSocketData->m_maxSocket + 1;
		outData.m_socketCount = m_pSocketData->m_socketCount;
		m_socketLock.unlock();
	}

	void SocketManager::HandleNewConnections()
	{
		MessageServer* pMessageServer = MessageServer::GetInstance();
		EXE_ASSERT(pMessageServer);

		for (auto& pSock : m_socketsToAdd)
		{
			Socket::Status status = pSock->Connect(10.0f);
			if (status == Socket::Status::Done)
			{
				pMessageServer->PushMessage(EXELIUS_NEW(ConnectedMessage(pSock->m_peerID)));

				m_socketLock.lock();
				m_sockets.emplace_back(pSock);
				m_socketLock.unlock();

				EXE_ASSERT(m_pSocketData);
				m_pSocketData->AddSocketData(pSock->m_socket);
			}
			else
			{
				pSock->CloseSocket();
				pMessageServer->PushMessage(EXELIUS_NEW(ConnectionFailedMessage(pSock->m_peerID)));
			}
		}

		m_socketsToAdd.clear();
	}

	void SocketManager::HandleClosedConnections()
	{
		MessageServer* pMessageServer = MessageServer::GetInstance();
		EXE_ASSERT(pMessageServer);

		m_socketLock.lock();
		for (const auto& pRemoveSock : m_socketsToRemove)
		{
			if (!pRemoveSock)
				continue; // Skip nullptrs

			for (auto iter = m_sockets.begin(); iter != m_sockets.end(); ++iter)
			{
				if (!iter || pRemoveSock != *iter)
					continue;

				EXE_ASSERT(m_pSocketData);
				m_pSocketData->RemoveSocketData((*iter)->m_socket);
				(*iter)->CloseSocket();
				pMessageServer->PushMessage(EXELIUS_NEW(DisconnectedMessage((*iter)->m_peerID)));
				iter = m_sockets.erase(iter);
				if (iter == m_sockets.end())
					break; // Exit outer for loop.
			}
		}
		m_socketLock.unlock();

		m_socketsToRemove.clear();
	}

	void SocketManager::HandleCurrentSockets(const SocketData& socketData)
	{
		for (auto& sock : m_sockets)
		{
			if (!sock->IsValid() && FD_ISSET(sock->m_socket, &socketData.m_excepts))
			{
				m_socketLock.lock();
				m_socketsToRemove.emplace_back(sock);
				m_socketLock.unlock();
				continue;
			}

			if (FD_ISSET(sock->m_socket, &socketData.m_readSockets))
				sock->ReceiveIncomingMessage();

			if (FD_ISSET(sock->m_socket, &socketData.m_writeSockets))
				sock->SendOutgoingMessages();
		}
	}
}
