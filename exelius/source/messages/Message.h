#pragma once
#include "source/utility/string/StringHash.h"

#include <EASTL/vector.h>
#include <EASTL/functional.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	using MessageID = uint32_t;
	static constexpr MessageID GetStaticMessageID(const char* pMessageName) { return (MessageID)StringHash::HashString32(pMessageName); }

	/// <summary>
	/// Defines the Message ID based on the name of the message class. This is REQUIRED in order to allow a message to work.
	/// Crashes may occur otherwise.
	/// 
	/// This should be defined in the Constructor of the Message Base class. 
	/// 
	/// PlayerMovedMessage Example:
	///		class PlayerMovedMessage
	///			: public Message
	///		{
	///		public:
	///			// NOTE: Class/Component name *matches* parameter value. This is not a hard requirement, but is best practice.
	///			PlayerMovedMessage()
	///				: Message(DEFINE_MESSAGE(PlayerMovedMessage))
	///			{
	///				//
	///			}
	///		}
	/// 
	/// Similarly, this must be done for NetMessage:
	/// 
	/// NetPlayerMovedMessage Example:
	///		class NetPlayerMovedMessage
	///			: public NetMessage[NetProtocol::Unreliable]
	///		{
	///		public:
	///			// NOTE: Class/Component name *matches* parameter value. This is not a hard requirement, but is best practice.
	///			NetPlayerMovedMessage()
	///				: NetMessage(DEFINE_MESSAGE(NetPlayerMovedMessage))
	///			{
	///				//
	///			}
	///		}
	/// 
	/// @note
	/// Replace the "[]" with angle brackets... used for proper xml displaying in VS. Sorry.
	/// </summary>
	#define DEFINE_MESSAGE(MESSAGETYPE) Exelius::GetStaticMessageID(#MESSAGETYPE)

	class Message
	{
		MessageID m_id;
		size_t m_packetReadPos;
		size_t m_packetSendPos;
		bool m_isPacketValid;

	protected:
		eastl::function<void(Message*)> m_callback;
		eastl::vector<char> m_dataPacket;

		Message(MessageID id);
	public:
		size_t m_readPos;
		size_t m_sendPos; // For handling partial sends

		// Overridables

		virtual ~Message() = default;

		// General Functions

		MessageID GetMessageID() const { return m_id; }
		
		// Callback Functions

		void SetMessageCallback(const eastl::function<void(Message*)>& callbackToSet);

		void ClearCallback();

		void ExecuteMessageCallback();

		bool IsCallbackSet() const;

		// Data Packet Functions

		void AppendToDataPacket(const void* data, size_t sizeInBytes);

		void ClearDataPacket();

		const eastl::vector<char>& GetDataPacket() const;

		size_t GetDataPacketSize() const;

		bool EndOfDataPacket() const;

		// Stream Overloads

		Message& operator >>(int32_t& data);
		Message& operator >>(uint32_t& data);

		Message& operator >>(int64_t& data);
		Message& operator >>(uint64_t& data);

		Message& operator >>(float& data);

		Message& operator >>(char* data);
		Message& operator >>(eastl::string& data);

		Message& operator <<(int32_t data);
		Message& operator <<(uint32_t data);

		Message& operator <<(int64_t data);
		Message& operator <<(uint64_t data);

		Message& operator <<(float data);

		Message& operator <<(const char* data);
		Message& operator <<(const eastl::string& data);

	private:
		bool CheckDataPacketSize(size_t size);
	};
}
