#pragma once

#include <EASTL/functional.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Message;

	class MessageReceiver
	{
		using ReceiverFunction = eastl::function<void(Message*)>;
		ReceiverFunction m_callbackFunction;
	public:

		MessageReceiver(ReceiverFunction callback);

		void InvokeCallback(Message* pMessage) const;

		void ClearCallback();
	};
}