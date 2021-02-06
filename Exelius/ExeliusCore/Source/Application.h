#pragma once

#include "Events/Event.h"
#include "Window.h"

#include "Events/ApplicationEvent.h"
#include <memory>

// I had this here for a reason initially, but now I am not sure why...
// I will remove this if no issues are found later on.
//int main(int argc, char* argv[]);

namespace Exelius
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& evnt);

	private:
		bool OnWindowClose(WindowCloseEvent& evnt);

		std::unique_ptr<Window> m_pWindow;
		bool m_isRunning;
	};
	
	// To be defined in client.
	Application* CreateApplication();
}