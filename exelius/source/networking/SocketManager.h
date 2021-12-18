#pragma once
#include "source/networking/NetHelpers.h"

#include "source/networking/Socket.h"

#include <EASTL/vector.h>

#include <thread>
#include <atomic>
#include <mutex>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Peer;

	class SocketManager
	{
		struct SocketData;
		SocketData* m_pSocketData;
		eastl::vector<Socket*> m_sockets;

		eastl::vector<Socket*> m_socketsToAdd;
		eastl::vector<SocketHandle> m_socketsToRemove;

		/// <summary>
		/// A mutex used to protect the sockets from data race conditions.
		/// This allows the main thread to continue when select and 
		/// connect is blocking.
		/// </summary>
		std::mutex m_socketLock;

		/// <summary>
		/// The dedicated socket reading thread.
		/// </summary>
		std::thread m_socketSelectThread;

		/// <summary>
		/// To shutdown the socket thread or continue operations.
		/// The thread will shutdown if this is true.
		/// </summary>
		std::atomic_bool m_quitThread;

		static SocketManager* s_pGlobalSocketManager;

		SocketManager();
		~SocketManager();
	public:
		static SocketManager* GetInstance();

		static void DestroyInstance();

		void SelectSockets();

		// Attempt to connect (non-blocking) to given address with new Peer
		// If the netaddress contains a port, but an Any or None IpAddress, then we are going to establish a listener.
		// Peer State will be NoConnection until the select function says it is writable, which will set it to UnvalidatedConnection.
		bool ConnectPeer(Peer& peerToConnect);

		void DisconnectPeer(Peer& peerToDisconnect);

		void DisconnectAll();
	private:

		void AddSocket(SocketHandle sockHandle);

		void RemoveSocket(SocketHandle sockHandle);

		void RemoveSocketFromSockets(SocketHandle sockHandle);

		void ClearSockets();

		bool Select(SocketData& socketData, float timeOut = 0.0f);
	};
}