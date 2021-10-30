#pragma once
#include "source/utility/containers/RingBuffer.h"

#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <EASTL/functional.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Message;
	using MessageID = uint32_t;
	class MessageServer
	{
		using RecieverList = eastl::vector<eastl::function<void(Message*)>>;
		eastl::unordered_map<MessageID, RecieverList> m_recievers;
		RingBufferMT<Message*, 4096> m_messages;
	public:

		void AddMessageReciever(MessageID id, eastl::function<void(Message*)> callback);

		void PushMessage(Message* pMessageToPush);

		void DispatchMessages();
	};

	inline static MessageServer* s_pGlobalMessageServer = nullptr;
}
