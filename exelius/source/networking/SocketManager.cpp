#include "EXEPCH.h"
#include "SocketManager.h"

#include "source/networking/Peer.h"

#include "source/messages/MessageServer.h"
#include "source/messages/exeliusmessages/networking/ConnectionMessages.h"

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
	SocketManager* SocketManager::s_pGlobalSocketManager = nullptr;

	struct SocketManager::SocketData
	{
		fd_set allSockets;  // Set containing all the sockets handles
		fd_set readSockets;	// Set containing handles of the sockets that are ready for reading.
		fd_set writeSockets;// Set containing handles of the sockets that are ready for writing.
		fd_set excepts;		// Caught errors
		int    maxSocket;   // Maximum socket handle
		int    socketCount; // Number of socket handles

		SocketData()
			: maxSocket(0)
			, socketCount(0)
		{
			FD_ZERO(&allSockets);
			FD_ZERO(&readSockets);
			FD_ZERO(&writeSockets);
			FD_ZERO(&excepts);
		}
	};

	SocketManager::SocketManager()
		: m_pSocketData(EXELIUS_NEW(SocketData()))
		, m_quitThread(false)
	{
		EXE_ASSERT(m_pSocketData);

		// Spin up the loader thread.
		m_socketSelectThread = std::thread(&SocketManager::SelectSockets, this);
	}

	SocketManager::~SocketManager()
	{
		m_quitThread = true;
		m_socketSelectThread.join();

		for (auto* pSocket : m_sockets)
		{
			EXELIUS_DELETE(pSocket);
		}
		for (auto* pSocket : m_socketsToAdd)
		{
			EXELIUS_DELETE(pSocket);
		}

		EXELIUS_DELETE(m_pSocketData);
	}

	SocketManager* SocketManager::GetInstance()
	{
		if (!s_pGlobalSocketManager)
			s_pGlobalSocketManager = EXELIUS_NEW(SocketManager());
		return s_pGlobalSocketManager;
	}

	void SocketManager::DestroyInstance()
	{
		EXELIUS_DELETE(s_pGlobalSocketManager);
	}

	void SocketManager::SelectSockets()
	{
		MessageServer* pMessageServer = MessageServer::GetInstance();
		EXE_ASSERT(pMessageServer);

		while (!m_quitThread)
		{
			// Recreate the SocketData so it is cleared.
			SocketData threadSocketData;

			m_socketLock.lock();
			threadSocketData.readSockets = m_pSocketData->allSockets;
			threadSocketData.writeSockets = m_pSocketData->allSockets;
			threadSocketData.excepts = m_pSocketData->allSockets;
			threadSocketData.maxSocket = m_pSocketData->maxSocket + 1;
			threadSocketData.socketCount = m_pSocketData->socketCount;
			m_socketLock.unlock();

			m_socketLock.lock();
			for (auto* pSock : m_socketsToAdd)
			{
				Socket::Status status = pSock->Connect(10.0f);
				if (status == Socket::Status::Done)
				{
					pMessageServer->PushMessage(EXELIUS_NEW(ConnectedMessage(pSock->m_peerID)));
					m_sockets.emplace_back(pSock); // TODO: consider making m_sockets a vector of pointers.

					AddSocket(pSock->m_socket);
				}
				else
				{
					pSock->CloseSocket();
					pMessageServer->PushMessage(EXELIUS_NEW(ConnectionFailedMessage(pSock->m_peerID)));
					EXELIUS_DELETE(pSock);
				}
			}

			m_socketsToAdd.clear();

			for (auto& sockHandle : m_socketsToRemove)
			{
				RemoveSocketFromSockets(sockHandle);
			}

			m_socketsToRemove.clear();
			m_socketLock.unlock();

			if (!Select(threadSocketData))
				continue;

			for (auto& sock : m_sockets)
			{
				if (!sock->IsValid() && FD_ISSET(sock->m_socket, &threadSocketData.excepts))
				{
					m_socketLock.lock();
					m_socketsToRemove.emplace_back(sock->m_socket);
					m_socketLock.unlock();
					continue;
				}

				if (FD_ISSET(sock->m_socket, &threadSocketData.readSockets))
					sock->ReceiveIncomingMessage();

				if (FD_ISSET(sock->m_socket, &threadSocketData.writeSockets))
					sock->SendOutgoingMessages();
			}
		}
	}

	bool SocketManager::ConnectPeer(Peer& peerToConnect)
	{
		MessageServer* pMessageServer = MessageServer::GetInstance();
		EXE_ASSERT(pMessageServer);

		Socket* pReliableSocket = EXELIUS_NEW(Socket(Socket::SocketType::TCP, peerToConnect.m_netAddress, peerToConnect.m_id));
		Socket* pUnreliableSocket = EXELIUS_NEW(Socket(Socket::SocketType::UDP, peerToConnect.m_netAddress, peerToConnect.m_id));

		// This should be an invalid peer. (Technically, it is an already valid peer)
		if (peerToConnect.m_pReliableSocket != nullptr && peerToConnect.m_pUnreliableSocket != nullptr)
		{
			pMessageServer->PushMessage(EXELIUS_NEW(ConnectionFailedMessage(peerToConnect.m_id)));
			return false;
		}

		peerToConnect.m_pReliableSocket = pReliableSocket;
		peerToConnect.m_pUnreliableSocket = pUnreliableSocket;

		m_socketLock.lock();
		m_socketsToAdd.emplace_back(pReliableSocket);
		m_socketsToAdd.emplace_back(pUnreliableSocket);
		m_socketLock.unlock();

		return true;
	}
	
	void SocketManager::DisconnectPeer(Peer& peerToDisconnect)
	{
		m_socketLock.lock();
		m_socketsToRemove.emplace_back(peerToDisconnect.m_pReliableSocket->m_socket);
		m_socketsToRemove.emplace_back(peerToDisconnect.m_pUnreliableSocket->m_socket);
		m_socketLock.unlock();
	}

	void SocketManager::DisconnectAll()
	{
		MessageServer* pMessageServer = MessageServer::GetInstance();
		EXE_ASSERT(pMessageServer);

		m_socketLock.lock();
		for (auto* pSock : m_socketsToAdd)
		{
			pMessageServer->PushMessage(EXELIUS_NEW(ConnectionFailedMessage(pSock->m_peerID)));
			EXELIUS_DELETE(pSock);
		}
		m_socketsToAdd.clear();

		for (auto& sock : m_sockets)
		{
			m_socketsToRemove.emplace_back(sock->m_socket);
		}
		m_socketLock.unlock();
	}

	void SocketManager::AddSocket(SocketHandle sockHandle)
	{
		EXE_ASSERT(m_pSocketData);

		if (sockHandle != SocketHandle_Invalid)
		{
			#if EXE_WINDOWS
			if (m_pSocketData->socketCount >= FD_SETSIZE)
			{
				// TODO: Log Error.
				return;
			}

			if (FD_ISSET(sockHandle, &m_pSocketData->allSockets))
				return;

			m_pSocketData->socketCount++;
			#else
			if (sockHandle >= FD_SETSIZE)
			{
				// TODO: Log Error.
				return;
			}

			// SocketHandle is an int in POSIX
			maxSocket = std::max(maxSocket, sockHandle);
			#endif

			FD_SET(sockHandle, &m_pSocketData->allSockets);
		}
	}
	
	void SocketManager::RemoveSocket(SocketHandle sockHandle)
	{
		EXE_ASSERT(m_pSocketData);

		if (sockHandle != SocketHandle_Invalid)
		{
			#if defined(EXE_WINDOWS)
			if (!FD_ISSET(sockHandle, &m_pSocketData->allSockets))
				return;
			m_pSocketData->socketCount--;
			#else
			if (sockHandle >= FD_SETSIZE)
				return;
			#endif

			FD_CLR(sockHandle, &m_pSocketData->allSockets);
			FD_CLR(sockHandle, &m_pSocketData->readSockets);
			FD_CLR(sockHandle, &m_pSocketData->writeSockets);
			FD_CLR(sockHandle, &m_pSocketData->excepts);
		}
	}

	void SocketManager::RemoveSocketFromSockets(SocketHandle sockHandle)
	{
		MessageServer* pMessageServer = MessageServer::GetInstance();
		EXE_ASSERT(pMessageServer);

		for (auto& sock : m_sockets)
		{
			if (sockHandle != sock->m_socket)
				continue;

			RemoveSocket(sock->m_socket);
			sock->CloseSocket();
			pMessageServer->PushMessage(EXELIUS_NEW(DisconnectedMessage(sock->m_peerID)));
			m_sockets.erase(&sock);
			EXELIUS_DELETE(sock);
			return;
		}
	}
	
	void SocketManager::ClearSockets()
	{
		EXE_ASSERT(m_pSocketData);

		FD_ZERO(&m_pSocketData->allSockets);
		FD_ZERO(&m_pSocketData->readSockets);
		FD_ZERO(&m_pSocketData->writeSockets);
		FD_ZERO(&m_pSocketData->excepts);

		m_pSocketData->maxSocket = 0;
		m_pSocketData->socketCount = 0;
	}

	bool SocketManager::Select(SocketData& socketData, float timeOut)
	{
		timeval time;
		time.tv_sec = static_cast<long>(timeOut / 1000000);
		time.tv_usec = static_cast<long>(static_cast<unsigned long long>(timeOut) % 1000000);

		int count = select(socketData.maxSocket + 1, &socketData.readSockets, &socketData.writeSockets, &socketData.excepts, timeOut != 0.0f ? &time : nullptr);

		return count > 0;
	}
}
