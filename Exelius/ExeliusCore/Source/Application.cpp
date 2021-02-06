#include "EXEPCH.h"

#include "Application.h"

#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

namespace Exelius
{
	Application::Application()
		: m_isRunning(true)
	{
		m_pWindow = std::unique_ptr<Window>(Window::Create());
		m_pWindow->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_isRunning)
		{
			m_pWindow->OnUpdate();
		}
	}

	void Application::OnEvent(Event& evnt)
	{
		EXE_CORE_TRACE("{0}", evnt);
		EventDispatcher dispatcher(evnt);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(Application::OnWindowClose));
	}

	bool Application::OnWindowClose(WindowCloseEvent& evnt)
	{
		m_isRunning = false;
		return true;
	}
}
