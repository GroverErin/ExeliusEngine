#pragma once
#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct NetAddressDefines
	{
		static constexpr const char* None = "0";
		static constexpr const char* Any_IPV4 = "0.0.0.0";
		static constexpr const char* Any_IPV6 = "::";
		static constexpr const char* LocalHost_IPV4 = "127.0.0.1";
		static constexpr const char* LocalHost_IPV6 = "::1";
		static constexpr const char* Broadcast_IPV4 = "255.255.255.255";
	};

	class NetAddress
	{
		struct SockAddrImpl;
		SockAddrImpl* m_pAddress;
		unsigned short m_portNumber;
		bool m_isValid;

	public:
		NetAddress(const eastl::string& address, unsigned short portNumber);
		NetAddress(const NetAddress& other);
		NetAddress(NetAddress&& other) = delete;
		//NetAddress& operator=(const NetAddress& other);
		NetAddress& operator=(NetAddress&& other) = delete;
		~NetAddress();

		eastl::string ToString(bool includePort = false) const;

		uint32_t ToInteger() const;

		unsigned short GetPort() const { return m_portNumber; }

	private:
		void ResolveStringAddress(const eastl::string& addressString, unsigned short portNumber);
	};

	bool operator ==(const NetAddress& left, const NetAddress& right);

	bool operator !=(const NetAddress& left, const NetAddress& right);
}