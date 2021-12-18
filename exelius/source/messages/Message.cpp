#include "EXEPCH.h"
#include "Message.h"


// These are needed for the ntohl and htonl functions
#ifdef EXE_WINDOWS
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#include <WinSock2.h>
	#include <ws2tcpip.h>
#else // Probably dont need all of these
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h>
#endif // EXELIUS_WINDOWS

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	Message::Message(MessageID id)
		: m_id(id)
		, m_packetReadPos(0)
		, m_packetSendPos(0)
		, m_isPacketValid(true)
		, m_callback(nullptr)
	{
		//
	}
	
	void Message::SetMessageCallback(const eastl::function<void(Message*)>& callbackToSet)
	{
		EXE_ASSERT(!IsCallbackSet());
		m_callback = callbackToSet;
	}

	void Message::ClearCallback()
	{
		m_callback = nullptr;
	}

	void Message::ExecuteMessageCallback()
	{
		if (IsCallbackSet())
			m_callback(this);
	}
	
	bool Message::IsCallbackSet() const
	{
		if (m_callback)
			return true;

		return false;
	}
	
	void Message::AppendToDataPacket(const void* data, size_t sizeInBytes)
	{
		if (data && (sizeInBytes > 0))
		{
			std::size_t start = m_dataPacket.size();
			m_dataPacket.resize(start + sizeInBytes);
			std::memcpy(&m_dataPacket[start], data, sizeInBytes);
		}
	}
	
	void Message::ClearDataPacket()
	{
		m_dataPacket.clear();
		m_packetReadPos = 0;
		m_isPacketValid = true;
	}
	
	const eastl::vector<char>& Message::GetDataPacket() const
	{
		return m_dataPacket;
	}
	
	size_t Message::GetDataPacketSize() const
	{
		return m_dataPacket.size();
	}
	
	bool Message::EndOfDataPacket() const
	{
		return m_packetReadPos >= m_dataPacket.size();
	}

	Message& Message::operator>>(int32_t& data)
	{
		if (CheckDataPacketSize(sizeof(data)))
		{
			data = ntohl(*reinterpret_cast<const int32_t*>(&m_dataPacket[m_packetReadPos]));
			m_packetReadPos += sizeof(data);
		}

		return *this;
	}

	Message& Message::operator>>(uint32_t& data)
	{
		if (CheckDataPacketSize(sizeof(data)))
		{
			data = ntohl(*reinterpret_cast<const uint32_t*>(&m_dataPacket[m_packetReadPos]));
			m_packetReadPos += sizeof(data);
		}

		return *this;
	}

	Message& Message::operator>>(int64_t& data)
	{
		if (CheckDataPacketSize(sizeof(data)))
		{
			// Since ntohll is not available everywhere, we have to convert
			// to network byte order (big endian) manually
			const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&m_dataPacket[m_packetReadPos]);
			data = (static_cast<int64_t>(bytes[0]) << 56) |
				(static_cast<int64_t>(bytes[1]) << 48) |
				(static_cast<int64_t>(bytes[2]) << 40) |
				(static_cast<int64_t>(bytes[3]) << 32) |
				(static_cast<int64_t>(bytes[4]) << 24) |
				(static_cast<int64_t>(bytes[5]) << 16) |
				(static_cast<int64_t>(bytes[6]) << 8) |
				(static_cast<int64_t>(bytes[7]));
			m_packetReadPos += sizeof(data);
		}

		return *this;
	}

	Message& Message::operator>>(uint64_t& data)
	{
		if (CheckDataPacketSize(sizeof(data)))
		{
			// Since ntohll is not available everywhere, we have to convert
			// to network byte order (big endian) manually
			const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&m_dataPacket[m_packetReadPos]);
			data = (static_cast<uint64_t>(bytes[0]) << 56) |
				(static_cast<uint64_t>(bytes[1]) << 48) |
				(static_cast<uint64_t>(bytes[2]) << 40) |
				(static_cast<uint64_t>(bytes[3]) << 32) |
				(static_cast<uint64_t>(bytes[4]) << 24) |
				(static_cast<uint64_t>(bytes[5]) << 16) |
				(static_cast<uint64_t>(bytes[6]) << 8) |
				(static_cast<uint64_t>(bytes[7]));
			m_packetReadPos += sizeof(data);
		}

		return *this;
	}

	Message& Message::operator>>(float& data)
	{
		if (CheckDataPacketSize(sizeof(data)))
		{
			data = *reinterpret_cast<const float*>(&m_dataPacket[m_packetReadPos]);
			m_packetReadPos += sizeof(data);
		}

		return *this;
	}

	Message& Message::operator>>(char* data)
	{
		// First extract string length
		uint32_t length = 0;
		*this >> length;

		if ((length > 0) && CheckDataPacketSize(length))
		{
			// Then extract characters
			std::memcpy(data, &m_dataPacket[m_packetReadPos], length);
			data[length] = '\0';

			// Update reading position
			m_packetReadPos += length;
		}

		return *this;
	}

	Message& Message::operator>>(eastl::string& data)
	{
		// First extract string length
		uint32_t length = 0;
		*this >> length;

		data.clear();
		if ((length > 0) && CheckDataPacketSize(length))
		{
			// Then extract characters
			data.assign(&m_dataPacket[m_packetReadPos], length);

			// Update reading position
			m_packetReadPos += length;
		}

		return *this;
	}

	Message& Message::operator<<(int32_t data)
	{
		int32_t toWrite = htonl(data);
		AppendToDataPacket(&toWrite, sizeof(toWrite));
		return *this;
	}

	Message& Message::operator<<(uint32_t data)
	{
		uint32_t toWrite = htonl(data);
		AppendToDataPacket(&toWrite, sizeof(toWrite));
		return *this;
	}

	Message& Message::operator<<(int64_t data)
	{
		// Since htonll is not available everywhere, we have to convert
		// to network byte order (big endian) manually
		uint8_t toWrite[] =
		{
			static_cast<uint8_t>((data >> 56) & 0xFF),
			static_cast<uint8_t>((data >> 48) & 0xFF),
			static_cast<uint8_t>((data >> 40) & 0xFF),
			static_cast<uint8_t>((data >> 32) & 0xFF),
			static_cast<uint8_t>((data >> 24) & 0xFF),
			static_cast<uint8_t>((data >> 16) & 0xFF),
			static_cast<uint8_t>((data >> 8) & 0xFF),
			static_cast<uint8_t>((data) & 0xFF)
		};
		AppendToDataPacket(&toWrite, sizeof(toWrite));
		return *this;
	}

	Message& Message::operator<<(uint64_t data)
	{
		// Since htonll is not available everywhere, we have to convert
		// to network byte order (big endian) manually
		uint8_t toWrite[] =
		{
			static_cast<uint8_t>((data >> 56) & 0xFF),
			static_cast<uint8_t>((data >> 48) & 0xFF),
			static_cast<uint8_t>((data >> 40) & 0xFF),
			static_cast<uint8_t>((data >> 32) & 0xFF),
			static_cast<uint8_t>((data >> 24) & 0xFF),
			static_cast<uint8_t>((data >> 16) & 0xFF),
			static_cast<uint8_t>((data >> 8) & 0xFF),
			static_cast<uint8_t>((data) & 0xFF)
		};
		AppendToDataPacket(&toWrite, sizeof(toWrite));
		return *this;
	}

	Message& Message::operator<<(float data)
	{
		AppendToDataPacket(&data, sizeof(data));
		return *this;
	}

	Message& Message::operator<<(const char* data)
	{
		// First insert string length
		uint32_t length = static_cast<uint32_t>(std::strlen(data));
		*this << length;

		// Then insert characters
		AppendToDataPacket(data, length * sizeof(char));

		return *this;
	}

	Message& Message::operator<<(const eastl::string& data)
	{
		// First insert string length
		uint32_t length = static_cast<uint32_t>(data.size());
		*this << length;

		// Then insert characters
		if (length > 0)
			AppendToDataPacket(data.c_str(), length * sizeof(std::string::value_type));

		return *this;
	}
	
	bool Message::CheckDataPacketSize(size_t size)
	{
		m_isPacketValid = m_isPacketValid && (m_packetReadPos + size <= m_dataPacket.size());

		return m_isPacketValid;
	}
}
