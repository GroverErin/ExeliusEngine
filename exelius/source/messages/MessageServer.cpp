#include "EXEPCH.h"
#include "MessageServer.h"
#include "MessageReceiver.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	MessageServer::~MessageServer()
	{
		Message* pMessage = nullptr;
		while (m_messages.PopFront(pMessage))
		{
			EXELIUS_DELETE(pMessage);
		}

		m_receivers.clear();
	}

	void MessageServer::AddMessageReceiver(MessageID id, eastl::shared_ptr<MessageReceiver> pNewEntry)
	{
		const auto& foundList = m_receivers.find(id);

		if (foundList != m_receivers.end())
			foundList->second.emplace_back(pNewEntry);
		else
		{
			// This doesn't seem like the best thing...
			eastl::vector<eastl::shared_ptr<MessageReceiver>> newReceiversList;
			newReceiversList.emplace_back(pNewEntry);
			m_receivers.emplace(id, newReceiversList);
		}
	}
	
	void MessageServer::PushMessage(Message* pMessageToPush)
	{
		if (!m_messages.PushBack(pMessageToPush))
		{
			// This probably should never happen...
			EXE_ASSERT(false);
		}
	}
	
	void MessageServer::DispatchMessages()
	{
		Message* pMessage = nullptr;
		while (m_messages.PopFront(pMessage))
		{
			if (!pMessage)
				continue;

			pMessage->ExecuteMessageCallback();

			// TODO: Clone receivers here and protect with mutex, as recievers are added from multiple threads.
			for (const auto& entry : m_receivers[pMessage->GetMessageID()])
			{
				if (entry)
					entry->InvokeCallback(pMessage);
			}

			EXELIUS_DELETE(pMessage);
			pMessage = nullptr;
		}
	}
}
