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

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int, const char* msg)
	{
		EXE_LOG_CATEGORY_FATAL("OpenGL", msg);
		EXE_ASSERT(false);
	}

	OpenGLWindow::OpenGLWindow(const eastl::string& title, const Vector2u& windowSize, bool isVSyncEnabled)
		: m_pRenderContext(nullptr)
		, m_pWindow(nullptr)
		, m_isVSync(isVSyncEnabled)
	{
		CreateWindow(title, windowSize, isVSyncEnabled);
	}

	OpenGLWindow::~OpenGLWindow()
	{
		Shutdown();
	}

	bool OpenGLWindow::CreateWindow(const eastl::string& title, const Vector2u& windowSize, bool isVSyncEnabled)
	{
		m_isVSync = isVSyncEnabled;
		m_windowData.m_title = title;
		m_windowData.m_windowSize = windowSize;

		EXE_LOG_CATEGORY_INFO("OpenGL", "Creating OpenGL Window: {0} ({1}, {2})", m_windowData.m_title.c_str(), m_windowData.m_windowSize.w, m_windowData.m_windowSize.h);

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			EXE_ASSERT(success);
			glfwSetErrorCallback(GLFWErrorCallback);
		}

#if defined(EXE_DEBUG)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		m_pWindow = glfwCreateWindow(m_windowData.m_windowSize.w, m_windowData.m_windowSize.h, m_windowData.m_title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		EXE_ASSERT(m_pWindow);

		glfwSetWindowUserPointer(m_pWindow, &m_windowData);

		// Set GLFW Window Event Callbacks
		glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWindow);
				WindowClosedEvent windowClosedEvent;
				data.m_messenger.NotifyObservers(windowClosedEvent);
			});

		glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWindow);
				data.m_windowSize.w = width;
				data.m_windowSize.h = height;

				WindowResizedEvent windowResizedEvent(width, height);
				data.m_messenger.NotifyObservers(windowResizedEvent);
			});

		glfwSetWindowFocusCallback(m_pWindow, [](GLFWwindow* pWindow, int focused)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWindow);

				switch (focused)
				{
					case GLFW_TRUE:
					{
						WindowGainedFocusEvent windowGainedFocusEvent;
						data.m_messenger.NotifyObservers(windowGainedFocusEvent);
						break;
					}
					case GLFW_FALSE:
					{
						WindowLostFocusEvent windowLostFocusEvent;
						data.m_messenger.NotifyObservers(windowLostFocusEvent);
						break;
					}
				}
			});

		// Set GLFW Keyboard Event Callbacks
		glfwSetKeyCallback(m_pWindow, [](GLFWwindow* pWindow, int key, int, int action, int)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWindow);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent keyPressedEvent(ConvertKeyCode(key));
						data.m_messenger.NotifyObservers(keyPressedEvent);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent keyReleasedEvent(ConvertKeyCode(key));
						data.m_messenger.NotifyObservers(keyReleasedEvent);
						break;
					}
				}
			});

		glfwSetCharCallback(m_pWindow, [](GLFWwindow* pWindow, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWindow);

				// TODO: Maybe handle unicode conversion?
				KeyTypedEvent keyTypedEvent(ConvertKeyCode(keycode), keycode);
				data.m_messenger.NotifyObservers(keyTypedEvent);
			});

		// Set GLFW Mouse Event Callbacks
		glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* pWindow, int button, int action, int)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWindow);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent mouseButtonPressed(ConvertMouseCode(button));
						data.m_messenger.NotifyObservers(mouseButtonPressed);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent mouseButtonReleased(ConvertMouseCode(button));
						data.m_messenger.NotifyObservers(mouseButtonReleased);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_pWindow, [](GLFWwindow* pWindow, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWindow);

				MouseScrolledEvent windowScrolledEvent(xOffset, yOffset);
				data.m_messenger.NotifyObservers(windowScrolledEvent);
			});

		glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* pWindow, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWindow);

				MouseMovedEvent mouseMovedEvent(xPos, yPos);
				data.m_messenger.NotifyObservers(mouseMovedEvent);
			});

		glfwSetCursorEnterCallback(m_pWindow, [](GLFWwindow* pWindow, int entered)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(pWindow);

				switch (entered)
				{
					case GLFW_TRUE:
					{
						MouseGainedFocusEvent mouseGainedFocusEvent;
						data.m_messenger.NotifyObservers(mouseGainedFocusEvent);
						break;
					}
					case GLFW_FALSE:
					{
						MouseLostFocusEvent mouseLostFocusEvent;
						data.m_messenger.NotifyObservers(mouseLostFocusEvent);
						break;
					}
				}
			});

		// TODO: Joypad Support
		
		
		return true;
	}

	void OpenGLWindow::InitializeRenderContext(Window* pAbstractWindow)
	{
		m_pRenderContext = EXELIUS_NEW(RenderContext());
		EXE_ASSERT(m_pRenderContext);
		m_pRenderContext->Initialize(pAbstractWindow);
		SetVSync(m_isVSync);
	}

	void OpenGLWindow::Update()
	{
		EXE_ASSERT(m_pRenderContext);
		glfwPollEvents();
		m_pRenderContext->SwapBuffers();
	}

	Vector2u OpenGLWindow::GetWindowSize() const
	{
		return Vector2u();
	}

	void OpenGLWindow::Shutdown()
	{
		glfwDestroyWindow(m_pWindow);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}

		EXELIUS_DELETE(m_pRenderContext);
	}

	void OpenGLWindow::SetVSync(bool isEnabled)
	{
		if (isEnabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_isVSync = isEnabled;
	}

	bool OpenGLWindow::IsVSync() const
	{
		return m_isVSync;
	}
}