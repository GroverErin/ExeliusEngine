#pragma once
#include <source/messages/Message.h>
#include <source/networking/NetMessage.h>
#include <iostream>

class TestCustomMessage
	: public Exelius::Message
{
public:
	TestCustomMessage()
		: Message(DEFINE_MESSAGE(TestCustomMessage))
	{
		m_callback = [](Message*) { std::cout << "Test Complete\n"; };
	}
};

class NetTestCustomMessage
	: public Exelius::NetMessage<Exelius::NetProtocol::LocalOnly>
{
public:
	NetTestCustomMessage()
		: NetMessage(DEFINE_MESSAGE(TestCustomMessage))
	{
		m_callback = [](Message*) { std::cout << "Network Test Complete\n"; };
	}
};