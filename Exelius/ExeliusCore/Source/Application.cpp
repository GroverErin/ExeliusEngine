#include "EXEPCH.h"

#include "Application.h"

namespace Exelius
{
	Application::Application()
		: m_isRunning(true)
	{
		m_pWindow = std::unique_ptr<Window>(Window::Create());
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
}
