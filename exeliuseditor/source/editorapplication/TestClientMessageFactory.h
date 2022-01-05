#pragma once
// TODO: This is a hack, and should NOT be necessary. This solves a linker error
#ifndef EXELIUS_USE_EASTL_ALLOCATOR
#define EASTL_USER_CONFIG_HEADER <source/utility/thirdparty/CustomEASTLConfig.h>
#endif
#include <include/Exelius.h>
#include <source/messages/exeliusmessages/ExeliusMessageFactory.h>

namespace Exelius
{
	class TestClientMessageFactory
		: public ExeliusMessageFactory
	{
	public:
		virtual ~TestClientMessageFactory() = default;
		virtual Message* CreateMessage(MessageID messageID, const eastl::vector<char>& messageData) final override;
	};
}