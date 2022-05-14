#include "EXEPCH.h"
#include "OpenGLWindow.h"

#include "source/os/platform/opengl/OpenGLInputConversions.h"
#include "source/render/RenderContext.h"

#include "source/os/events/ApplicationEvents.h"
#include "source/os/events/KeyEvents.h"
#include "source/os/events/MouseEvents.h"
#include "source/os/events/JoystickEvents.h"
#include "source/os/events/TouchEvents.h"
#include "source/os/events/MiscEvents.h"

#include "source/engine/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int, const char* msg)
	{
		// TODO: An error triggers here when my monitor setup sleeps or is turned on or off.
		// GLFW: Win32: Unable to query monitor DPI.
		EXE_LOG_CATEGORY_FATAL("OpenGL", msg);
		//EXE_ASSERT(false);
	}

	OpenGLWindow::OpenGLWindow(const WindowProperties& windowProperties)
		: m_windowProperties(windowProperties)
		, m_pRenderContext(nullptr)
		, m_pWindow(nullptr)
	{
		CreateWindow(windowProperties);
	}

	OpenGLWindow::~OpenGLWindow()
	{
		Shutdown();
	}

	bool OpenGLWindow::CreateWindow(const WindowProperties& windowProperties)
	{
		m_windowProperties = windowProperties;

		if (!m_windowProperties.m_pMessenger)
			m_windowProperties.m_pMessenger = EXELIUS_NEW(OSEventMessenger());

		EXE_LOG_CATEGORY_INFO("OpenGL", "Creating OpenGL Window: {0} ({1}, {2})", m_windowProperties.m_title.c_str(), m_windowProperties.m_windowSize.x, m_windowProperties.m_windowSize.y);

#ifdef EXE_WINDOWS
		if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
		{
			EXE_LOG_CATEGORY_INFO("OpenGL", "Windows Failed To Set DPI Awareness.");
			EXE_ASSERT(false);
		}
#endif

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			EXE_ASSERT(success);
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		float xScale, yScale;
		glfwGetMonitorContentScale(pMonitor, &xScale, &yScale);

		if (xScale > 1 || yScale > 1)
		{
			m_windowProperties.m_DPIScale.x = xScale;
			m_windowProperties.m_DPIScale.y = yScale;

			glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
		}


#if defined(EXE_DEBUG)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		if (!m_windowProperties.m_hasDecorators)
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

		if (m_windowProperties.m_isFullscreen)
			m_pWindow = glfwCreateWindow((int)m_windowProperties.m_windowSize.x, (int)m_windowProperties.m_windowSize.y, m_windowProperties.m_title.c_str(), glfwGetPrimaryMonitor(), nullptr);
		else
			m_pWindow = glfwCreateWindow((int)m_windowProperties.m_windowSize.x, (int)m_windowProperties.m_windowSize.y, m_windowProperties.m_title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		EXE_ASSERT(m_pWindow);
		int xPos = -1;
		int yPos = -1;

		glfwGetWindowPos(m_pWindow, &xPos, &yPos);

		m_windowProperties.m_windowPosition.x = (float)xPos;
		m_windowProperties.m_windowPosition.y = (float)yPos;
		m_windowProperties.m_previousWindowPosition.x = (float)xPos;
		m_windowProperties.m_previousWindowPosition.y = (float)yPos;

		glfwSetWindowUserPointer(m_pWindow, &m_windowProperties);

		//-------------------------------------------------------------------------------------------------------------------
		// Set GLFW Window Event Callbacks
		//-------------------------------------------------------------------------------------------------------------------

		// Window Closed
		glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				WindowClosedEvent windowClosedEvent;
				windowProperties.m_pMessenger->NotifyObservers(windowClosedEvent);
			});

		// Window Resized
		glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				// Store the old window size for restoration later.
				windowProperties.m_previousWindowSize = windowProperties.m_windowSize;

				if (width < 0)
					width = 0;

				if (height < 0)
					height = 0;

				// Set new size.
				windowProperties.m_windowSize.x = (float)width;
				windowProperties.m_windowSize.y = (float)height;

				WindowResizedEvent windowResizedEvent(width, height);
				windowProperties.m_pMessenger->NotifyObservers(windowResizedEvent);
			});

		// Window Moved
		glfwSetWindowPosCallback(m_pWindow, [](GLFWwindow* pWindow, int x, int y)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				// Store the old window position for restoration later.
				windowProperties.m_previousWindowPosition = windowProperties.m_windowPosition;

				if (x < 0)
					x = 0;

				if (y < 0)
					y = 0;

				// Set new position.
				windowProperties.m_windowPosition.x = (float)x;
				windowProperties.m_windowPosition.y = (float)y;

				WindowMovedEvent windowMovedEvent(x, y);
				windowProperties.m_pMessenger->NotifyObservers(windowMovedEvent);
			});

		// Window Focus Changed
		glfwSetWindowFocusCallback(m_pWindow, [](GLFWwindow* pWindow, int focused)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				switch (focused)
				{
					case GLFW_TRUE:
					{
						WindowGainedFocusEvent windowGainedFocusEvent;
						windowProperties.m_pMessenger->NotifyObservers(windowGainedFocusEvent);
						break;
					}
					case GLFW_FALSE:
					{
						WindowLostFocusEvent windowLostFocusEvent;
						windowProperties.m_pMessenger->NotifyObservers(windowLostFocusEvent);
						break;
					}
				}
			});

		// Window Maximized
		glfwSetWindowMaximizeCallback(m_pWindow, [](GLFWwindow* pWindow, int maximized)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				if (maximized == GLFW_TRUE)
					windowProperties.m_isMaximized = true;
				else
					windowProperties.m_isMaximized = false;
			});

		/*glfwSetMonitorCallback([](GLFWmonitor*, int)
			{
				int count;
				GLFWmonitor** ppMonitors = glfwGetMonitors(&count);
				EXE_LOG_CATEGORY_WARN("OpenGL", "Monitor was disconnected.");
				if (count == 0)
				{
					Application::GetInstance()->CloseApplication();
				}
			});*/

		//-------------------------------------------------------------------------------------------------------------------
		// Set GLFW Keyboard Event Callbacks
		//-------------------------------------------------------------------------------------------------------------------
		glfwSetKeyCallback(m_pWindow, [](GLFWwindow* pWindow, int key, int, int action, int)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent keyPressedEvent(ConvertKeyCode(key));
						windowProperties.m_pMessenger->NotifyObservers(keyPressedEvent);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent keyReleasedEvent(ConvertKeyCode(key));
						windowProperties.m_pMessenger->NotifyObservers(keyReleasedEvent);
						break;
					}
				}
			});

		glfwSetCharCallback(m_pWindow, [](GLFWwindow* pWindow, unsigned int keycode)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				// TODO: Maybe handle unicode conversion?
				KeyTypedEvent keyTypedEvent(ConvertKeyCode(keycode), keycode);
				windowProperties.m_pMessenger->NotifyObservers(keyTypedEvent);
			});

		//-------------------------------------------------------------------------------------------------------------------
		// Set GLFW Mouse Event Callbacks
		//-------------------------------------------------------------------------------------------------------------------
		glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* pWindow, int button, int action, int)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent mouseButtonPressed(ConvertMouseCode(button));
						windowProperties.m_pMessenger->NotifyObservers(mouseButtonPressed);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent mouseButtonReleased(ConvertMouseCode(button));
						windowProperties.m_pMessenger->NotifyObservers(mouseButtonReleased);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_pWindow, [](GLFWwindow* pWindow, double xOffset, double yOffset)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				MouseScrolledEvent windowScrolledEvent(xOffset, yOffset);
				windowProperties.m_pMessenger->NotifyObservers(windowScrolledEvent);
			});

		glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* pWindow, double xPos, double yPos)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				MouseMovedEvent mouseMovedEvent(xPos, yPos);
				windowProperties.m_pMessenger->NotifyObservers(mouseMovedEvent);
			});

		glfwSetCursorEnterCallback(m_pWindow, [](GLFWwindow* pWindow, int entered)
			{
				EXE_ASSERT(pWindow);
				WindowProperties& windowProperties = *(WindowProperties*)glfwGetWindowUserPointer(pWindow);
				EXE_ASSERT(windowProperties.m_pMessenger);

				switch (entered)
				{
					case GLFW_TRUE:
					{
						MouseGainedFocusEvent mouseGainedFocusEvent;
						windowProperties.m_pMessenger->NotifyObservers(mouseGainedFocusEvent);
						break;
					}
					case GLFW_FALSE:
					{
						MouseLostFocusEvent mouseLostFocusEvent;
						windowProperties.m_pMessenger->NotifyObservers(mouseLostFocusEvent);
						break;
					}
				}
			});

		//-------------------------------------------------------------------------------------------------------------------
		// TODO: Joypad Support
		//-------------------------------------------------------------------------------------------------------------------

		return true;
	}

	void OpenGLWindow::InitializeRenderContext(Window* pAbstractWindow)
	{
		EXE_ASSERT(pAbstractWindow);
		m_pRenderContext = EXELIUS_NEW(RenderContext());
		EXE_ASSERT(m_pRenderContext);
		m_pRenderContext->Initialize(pAbstractWindow);
		SetVSync(m_windowProperties.m_isVSync);
	}

	void OpenGLWindow::Update()
	{
		EXE_ASSERT(m_pRenderContext);

		glfwPollEvents();
		m_pRenderContext->SwapBuffers();
	}

	const glm::vec2& OpenGLWindow::GetWindowSize() const
	{
		return m_windowProperties.m_windowSize;
	}

	const glm::vec2& OpenGLWindow::GetWindowPosition() const
	{
		return m_windowProperties.m_windowPosition;
	}

	void OpenGLWindow::Shutdown()
	{
		EXE_ASSERT(m_pWindow);

		glfwDestroyWindow(m_pWindow);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}

		EXELIUS_DELETE(m_windowProperties.m_pMessenger);
		EXELIUS_DELETE(m_pRenderContext);
	}

	void OpenGLWindow::SetVSync(bool isEnabled)
	{
		if (isEnabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_windowProperties.m_isVSync = isEnabled;
	}

	bool OpenGLWindow::IsVSync() const
	{
		return m_windowProperties.m_isVSync;
	}

	void OpenGLWindow::SetFullscreen(bool setFullscreen)
	{
		if (setFullscreen && m_windowProperties.m_isFullscreen)
			return; // Don't do anything here.

		if (setFullscreen)
		{
			if (!m_pWindow)
			{
				EXE_LOG_CATEGORY_FATAL("OpenGL", "Failed to Set Fullscreen: Window was nullptr.");
				EXE_ASSERT(false);
				return;
			}

			auto* pMonitor = glfwGetPrimaryMonitor();
			if (!pMonitor)
			{
				EXE_LOG_CATEGORY_FATAL("OpenGL", "Failed to Set Fullscreen: Primary monitor not found or was nullptr.");
				EXE_ASSERT(false);
				return;
			}

			const GLFWvidmode* pMode = glfwGetVideoMode(pMonitor);
			if (!pMode)
			{
				EXE_LOG_CATEGORY_FATAL("OpenGL", "Failed to Set Fullscreen: Primary monitor Video Mode not found or was nullptr.");
				EXE_ASSERT(false);
				return;
			}

			// Store the old window size for restoration later.
			//m_windowProperties.m_previousWindowSize = m_windowProperties.m_windowSize;
			//m_windowProperties.m_previousWindowPosition = m_windowProperties.m_windowPosition;

			//m_windowProperties.m_windowSize.x = (float)pMode->width;
			//m_windowProperties.m_windowSize.y = (float)pMode->height;
			//m_windowProperties.m_windowPosition.x = 0;
			//m_windowProperties.m_windowPosition.y = 0;

			glfwSetWindowMonitor(m_pWindow, pMonitor, 0, 0, pMode->width, pMode->height, pMode->refreshRate);
			m_windowProperties.m_isFullscreen = true;
		}
		else
		{
			glfwSetWindowMonitor(m_pWindow, nullptr,
				(int)m_windowProperties.m_previousWindowPosition.x,
				(int)m_windowProperties.m_previousWindowPosition.y,
				(int)m_windowProperties.m_previousWindowSize.x,
				(int)m_windowProperties.m_previousWindowSize.y,
				GLFW_DONT_CARE);

			m_windowProperties.m_isFullscreen = false;
			glfwSetWindowAttrib(m_pWindow, GLFW_DECORATED, GLFW_TRUE);
		}
	}

	void OpenGLWindow::MinimizeWindow()
	{
		if (!m_pWindow)
		{
			EXE_LOG_CATEGORY_FATAL("OpenGL", "Failed to Minimize Window: Window was nullptr.");
			EXE_ASSERT(m_pWindow);
			return;
		}

		glfwIconifyWindow(m_pWindow);
	}

	void OpenGLWindow::MaximizeWindow()
	{
		if (!m_pWindow)
		{
			EXE_LOG_CATEGORY_FATAL("OpenGL", "Failed to Maximize Window: Window was nullptr.");
			EXE_ASSERT(m_pWindow);
			return;
		}

		glfwMaximizeWindow(m_pWindow);
	}
	
	void OpenGLWindow::RestoreWindow()
	{
		if (!m_pWindow)
		{
			EXE_LOG_CATEGORY_FATAL("OpenGL", "Failed to Restore Window: Window was nullptr.");
			EXE_ASSERT(m_pWindow);
			return;
		}

		glfwRestoreWindow(m_pWindow);
	}
	
	void OpenGLWindow::CloseWindow()
	{
		if (!m_pWindow)
		{
			EXE_LOG_CATEGORY_FATAL("OpenGL", "Failed to Restore Window: Window was nullptr.");
			EXE_ASSERT(m_pWindow);
			return;
		}

		EXE_ASSERT(m_windowProperties.m_pMessenger);

		// The documentations states that the close callback is called after setting this to true, but it does not.
		//glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);

		WindowClosedEvent windowClosedEvent;
		m_windowProperties.m_pMessenger->NotifyObservers(windowClosedEvent);
	}
}