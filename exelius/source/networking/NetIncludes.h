#pragma once

#ifdef EXE_WINDOWS
	#pragma comment(lib, "Ws2_32.lib")
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#include <WinSock2.h>
	#include <ws2tcpip.h>

	struct WindowsSocketGlobalInitializer
	{
		WindowsSocketGlobalInitializer()
		{
			WSAData wsadata;
			int value = WSAStartup(MAKEWORD(2, 2), &wsadata);
			EXE_ASSERT(value == 0);
		}

		~WindowsSocketGlobalInitializer()
		{
			WSACleanup();
		}
	};

	WindowsSocketGlobalInitializer g_Init;
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>
#endif // EXELIUS_WINDOWS