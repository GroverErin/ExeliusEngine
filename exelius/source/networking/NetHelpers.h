#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	using PeerID = int32_t;
	static constexpr PeerID PeerID_Invalid = -1;

	#if EXE_WINDOWS
	using SocketHandle = UINT_PTR;

	// INVALID_SOCKET should be used here instead.
	// I just dont want to have to include winsock2.h here.
	using SOCKET = UINT_PTR;
	static constexpr SocketHandle SocketHandle_Invalid = (SOCKET)(~0);
	#else
	using SocketHandle = int;
	static constexpr SocketHandle SocketHandle_Invalid = -1;
	#endif
}