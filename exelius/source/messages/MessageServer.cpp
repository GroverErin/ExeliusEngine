#include "EXEPCH.h"
#include "MessageServer.h"
#include "Message.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void MessageServer::AddMessageReciever(MessageID id, eastl::function<void(Message*)> callback)
	{
		const auto& foundList = m_recievers.find(id);

		if (foundList != m_recievers.end())
			foundList->second.emplace_back(callback);
		else
		{
			// This doesn't seem like the best thing...
			eastl::vector<eastl::function<void(Message*)>> newCallbacksList;
			newCallbacksList.emplace_back(callback);
			m_recievers.emplace(id, newCallbacksList);
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

			// TODO: Clone recievers here.
			for (auto& callback : m_recievers[pMessage->GetMessageID()])
			{
				callback(pMessage);
			}

			EXELIUS_DELETE(pMessage);
			pMessage = nullptr;
		}
	}
}
