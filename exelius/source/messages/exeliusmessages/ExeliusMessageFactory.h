#pragma once
#include "source/messages/MessageFactory.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class ExeliusMessageFactory
		: public MessageFactory
	{
	public:
		virtual Message* CreateMessage(MessageID, const eastl::vector<char>&) override { return nullptr; }
	};
}