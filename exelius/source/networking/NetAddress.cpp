#include "EXEPCH.h"
#include "NetAddress.h"
#include "source/networking/NetIncludes.h"

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
	struct NetAddress::SockAddrImpl
	{
		sockaddr_in6 m_ipv6;
		sockaddr_in m_ipv4;
	};
	
	NetAddress::NetAddress(const eastl::string& address, unsigned short portNumber)
		: m_pAddress(EXELIUS_NEW(SockAddrImpl()))
		, m_portNumber(portNumber)
		, m_isValid(false)
	{
		EXE_ASSERT(m_portNumber > 1024); // Port is attempting to use restricted port number.
		m_pAddress->m_ipv4.sin_family = AF_INET;
		ResolveStringAddress(address, portNumber);
	}
	
	NetAddress::NetAddress(const NetAddress& other)
	{
		m_pAddress = EXELIUS_NEW(SockAddrImpl());
		m_pAddress->m_ipv4 = other.m_pAddress->m_ipv4;
		m_pAddress->m_ipv6 = other.m_pAddress->m_ipv6;
		m_portNumber = other.m_portNumber;
		EXE_ASSERT(m_portNumber > 1024); // Port is attempting to use restricted port number.
		m_isValid = other.m_isValid;
	}

	//NetAddress& NetAddress::operator=(const NetAddress& other)
	//{
	//	m_pAddress = EXELIUS_NEW(SockAddrImpl());
	//	m_pAddress->m_ipv4 = other.m_pAddress->m_ipv4;
	//	m_pAddress->m_ipv6 = other.m_pAddress->m_ipv6;
	//	m_portNumber = other.m_portNumber;
	//	EXE_ASSERT(m_portNumber > 1024); // Port is attempting to use restricted port number.
	//	m_isValid = other.m_isValid;
	//	return *this;
	//}

	NetAddress::~NetAddress()
	{
		EXELIUS_DELETE(m_pAddress);
	}

	const char* NetAddress::ToString(bool includePort) const
	{
		char* addressString = nullptr;
		int errorCheck = 0;

		if (m_pAddress->m_ipv4.sin_family == AF_INET)
		{
			errorCheck = getnameinfo((struct sockaddr*)&m_pAddress->m_ipv4, sizeof(struct sockaddr_in), addressString, 22, nullptr, 0, NI_NUMERICHOST);
		}
		else
		{
			errorCheck = getnameinfo((struct sockaddr*)&m_pAddress->m_ipv6, sizeof(struct sockaddr_in6), addressString, INET6_ADDRSTRLEN, nullptr, 0, NI_NUMERICHOST);
		}

		if (errorCheck != 0)
			addressString = "0";

		if (addressString == nullptr)
			return nullptr;

		if (includePort)
		{
			unsigned char ch[2];
			ch[0] = '|';
			ch[1] = 0;
			strcat(addressString, (const char*)ch);
			_itoa(m_portNumber, addressString + strlen(addressString), 10);
		}

		return addressString;
	}

	uint32_t NetAddress::ToInteger() const
	{
		if (m_pAddress->m_ipv4.sin_family == AF_INET)
		{
			return ntohl(m_pAddress->m_ipv4.sin_addr.s_addr);
		}
		else
		{
			uint32_t asInt = 0;
			sscanf_s((const char*)&m_pAddress->m_ipv6.sin6_addr.s6_addr, "%d", &asInt);
			return asInt;
		}
	}

	void NetAddress::ResolveStringAddress(const eastl::string& addressString, unsigned short portNumber)
	{
		// Set the port for both address types.
		m_pAddress->m_ipv6.sin6_port = htons(portNumber);
		m_pAddress->m_ipv4.sin_port = htons(portNumber);

		// Set the address.
		char address[INET6_ADDRSTRLEN];

		// Validate the string
		for (auto& c : addressString)
		{
			if ((c < '0' || c > '9') && (c < 'a' || c > 'f') && (c < 'A' || c > 'F') && c != '.' && c != ':' && c != '%' && c != '-' && c != '/')
				return; // We have an invalid address string.
		}

		strncpy(address, addressString.c_str(), sizeof(address));
		address[sizeof(address) - 1] = 0;

		addrinfo hints;
		memset(&hints, 0, sizeof(addrinfo));
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_family = AF_UNSPEC;

		addrinfo* returnInfo = nullptr;
		getaddrinfo(address, "", &hints, &returnInfo);

		EXE_ASSERT(returnInfo);

		if (returnInfo->ai_family == AF_INET)
		{
			m_pAddress->m_ipv4.sin_family = AF_INET;
			memcpy(&m_pAddress->m_ipv4, (struct sockaddr_in*)returnInfo->ai_addr, sizeof(struct sockaddr_in));
		}
		else
		{
			m_pAddress->m_ipv4.sin_family = AF_INET6;
			m_pAddress->m_ipv6.sin6_family = AF_INET6;
			memcpy(&m_pAddress->m_ipv6, (struct sockaddr_in6*)returnInfo->ai_addr, sizeof(struct sockaddr_in6));
		}

		freeaddrinfo(returnInfo);

		m_isValid = true;
	}

	bool operator ==(const NetAddress& left, const NetAddress& right)
	{
		return (left.GetPort() == right.GetPort()) && (left.ToInteger() == right.ToInteger());
	}

	bool operator !=(const NetAddress& left, const NetAddress& right)
	{
		return !(left == right);
	}
}