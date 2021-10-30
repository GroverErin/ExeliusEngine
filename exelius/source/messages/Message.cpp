#include "EXEPCH.h"
#include "Message.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	Message::Message(MessageID id)
		: m_id(id)
	{
		//
	}

	bool Message::SetMessageCallback(const eastl::function<void(Message*)>& callbackToSet)
	{
		if (IsCallbackSet())
			return false;

		m_callback = callbackToSet;
		return true;
	}
	
	void Message::ClearCallback()
	{
		m_callback = nullptr;
	}
	
	void Message::ExecuteMessageCallback()
	{
		if (m_callback)
			m_callback(this);
	}

	bool Message::IsCallbackSet() const
	{
		if (m_callback)
			return true;

		return false;
	}
}
