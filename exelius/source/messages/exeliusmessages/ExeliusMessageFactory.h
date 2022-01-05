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
		virtual ~ExeliusMessageFactory() = default;
		virtual Message* CreateMessage(MessageID messageID, const eastl::vector<char>& messageData) override;
	};
}