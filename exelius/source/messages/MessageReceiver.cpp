#include "EXEPCH.h"
#include "MessageReceiver.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	MessageReceiver::MessageReceiver(ReceiverFunction callback)
		: m_callbackFunction(callback)
	{
		//
	}

	void MessageReceiver::InvokeCallback(Message* pMessage) const
	{
		if (m_callbackFunction)
			m_callbackFunction(pMessage);
	}

	void MessageReceiver::ClearCallback()
	{
		m_callbackFunction = nullptr;
	}
}