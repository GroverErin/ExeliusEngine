#pragma once

#include "Events/Event.h"
#include "Window.h"

int main(int argc, char* argv[]);

namespace Exelius
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_pWindow;
		bool m_isRunning;
	};
	
	// To be defined in client.
	Application* CreateApplication();
}