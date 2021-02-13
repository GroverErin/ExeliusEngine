#pragma once
#include "Source/Debug/LogBase.h"

extern Exelius::Application* Exelius::CreateApplication();

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	Exelius::_Log::Initialize();

	auto* pApp = Exelius::CreateApplication();

	assert(pApp);
	pApp->Run();

	delete pApp;
	return 0;
}

// Creates the application run by the engine. Required to be in the clients application.
// The client is REQUIRED to have created a class inhereting from Exelius::Application.
// That class name should be passed in to this macro.
// Example: EXELIUS_MAIN(SandboxApp)
#define EXELIUS_MAIN(userAppClassName)\
Exelius::Application* Exelius::CreateApplication() { return new userAppClassName(); }