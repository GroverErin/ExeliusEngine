#pragma once
#include "source/utility/generic/Singleton.h"
#include "source/utility/containers/RingBuffer.h"
#include "Message.h"

#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <EASTL/shared_ptr.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class MessageReceiver;
	class MessageServer
		: public Singleton<MessageServer>
	{
		using ReceiverList = eastl::vector<eastl::shared_ptr<MessageReceiver>>;
		eastl::unordered_map<MessageID, ReceiverList> m_receivers;
		RingBufferMT<Message*, 4096> m_messages;
	public:
		~MessageServer();

		void AddMessageReceiver(MessageID id, eastl::shared_ptr<MessageReceiver> pNewEntry);

		void PushMessage(Message* pMessageToPush);

		void DispatchMessages();
	};
}
