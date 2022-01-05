#include "TestClientMessageFactory.h"
#include "TestClientMessage.h"

namespace Exelius
{
	Message* TestClientMessageFactory::CreateMessage(MessageID messageID, const eastl::vector<char>& messageData)
	{
		if (messageID == DEFINE_MESSAGE(TestCustomMessage))
		{
			return EXELIUS_NEW(TestCustomMessage());
		}

		return ExeliusMessageFactory::CreateMessage(messageID, messageData);
	}
}
