#pragma once
#include "Log.h"

extern Exelius::Application* Exelius::CreateApplication();

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	Exelius::Log::Initialize();

	auto app = Exelius::CreateApplication();
	app->Run();
	delete app;
	return 0;
}