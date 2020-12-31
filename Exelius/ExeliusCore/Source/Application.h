#pragma once

int main(int argc, char* argv[]);

namespace Exelius
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
	
	// To be defined in client.
	Application* CreateApplication();
}