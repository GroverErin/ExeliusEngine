#pragma once
#include "source/networking/NetHelpers.h"
#include "source/utility/generic/SmartPointers.h"

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
	class Socket;

	class SocketManager
	{
		class SocketData;
		SocketData* m_pSocketData;
		eastl::vector<SharedPtr<Socket>> m_sockets;

		eastl::vector<SharedPtr<Socket>> m_socketsToAdd;
		eastl::vector<SharedPtr<Socket>> m_socketsToRemove;

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

	public:
		SocketManager();
		~SocketManager();

		void SelectSockets();

		void RegisterPeerSockets(const Peer& peerToConnect);

		void DisconnectPeer(const Peer& peerToDisconnect);

		void DisconnectAll();

	private:
		// Threaded Functions
		bool Select(SocketData& socketData, float timeOut = 0.0f);

		void SwapSocketData(SocketData& outData);

		void HandleNewConnections();
		void HandleClosedConnections();
		void HandleCurrentSockets(const SocketData& socketData);
	};
}