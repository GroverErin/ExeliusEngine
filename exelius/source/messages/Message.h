#pragma once
#include <cstdint>

#include <EASTL/functional.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	using MessageID = uint32_t;
	class Message
	{
	protected:
		MessageID m_id = 0;
		eastl::function<void(Message*)> m_callback;
	public:
		Message(MessageID id);

		MessageID GetMessageID() const { return m_id; }
		
		//Add assert instead of boolean
		bool SetMessageCallback(const eastl::function<void(Message*)>& callbackToSet);

		void ClearCallback();

		void ExecuteMessageCallback();

		bool IsCallbackSet() const;
	};
}
