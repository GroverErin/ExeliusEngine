#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"

struct GLFWwindow;

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Window);

	class OpenGLRenderContext
	{
	public:
		OpenGLRenderContext();
		~OpenGLRenderContext();

		void Initialize(Window* pWindow);

		void SwapBuffers();

	private:
		GLFWwindow* m_pWindow;
	};
}