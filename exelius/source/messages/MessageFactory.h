#pragma once
#include "source/messages/Message.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class MessageFactory
	{
	public:
		virtual ~MessageFactory() = default;
		virtual Message* CreateMessage(MessageID id, const eastl::vector<char>& messageData) = 0;
	};
}