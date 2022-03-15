#include "EXEPCH.h"
#include "OpenGLRenderContext.h"
#include "source/render/Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	OpenGLRenderContext::OpenGLRenderContext()
		: m_pWindow(nullptr)
	{
		//
	}

	OpenGLRenderContext::~OpenGLRenderContext()
	{
		m_pWindow = nullptr;
	}

	void OpenGLRenderContext::Initialize(Window* pWindow)
	{
		EXE_ASSERT(pWindow);
		m_pWindow = pWindow->GetNativeWindow().GetGLFWWindow();
		EXE_ASSERT(m_pWindow);

		glfwMakeContextCurrent(m_pWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		EXE_ASSERT(status);
		EXE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5));
	}

	void OpenGLRenderContext::SwapBuffers()
	{
		EXE_ASSERT(m_pWindow);
		glfwSwapBuffers(m_pWindow);
	}
}