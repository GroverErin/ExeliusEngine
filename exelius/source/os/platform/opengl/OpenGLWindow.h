#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/render/WindowProperties.h"

struct GLFWwindow;

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Window);
	FORWARD_DECLARE(View);
	FORWARD_DECLARE(Texture);
	FORWARD_DECLARE(VertexArray);

	FORWARD_DECLARE(RenderContext);

	/// <summary>
	/// Window class that handles OpenGL specific function calls.
	/// The window also processes OpenGL events and passes them to
	/// the engines Event Management system.
	/// </summary>
	class OpenGLWindow
	{
		WindowProperties m_windowProperties;
		RenderContext* m_pRenderContext;
		GLFWwindow* m_pWindow;

	public:
		OpenGLWindow(const WindowProperties& windowProperties);
		OpenGLWindow(const OpenGLWindow&) = delete;
		OpenGLWindow(OpenGLWindow&&) = delete;
		OpenGLWindow& operator=(const OpenGLWindow&) = delete;
		OpenGLWindow& operator=(OpenGLWindow&&) = delete;
		~OpenGLWindow();

#undef CreateWindow // Defined in WinUser.h :(
		bool CreateWindow(const WindowProperties& windowProperties);

		void InitializeRenderContext(Window* pAbstractWindow);

		void Update();

		const glm::vec2& GetWindowSize() const;
		const glm::vec2& GetWindowPosition() const;

		void SetVSync(bool isEnabled);

		bool IsVSync() const;

		void SetFullscreen(bool setFullscreen);
		bool IsFullscreen() const { return m_windowProperties.m_isFullscreen; }

		void MinimizeWindow();
		void MaximizeWindow();
		void RestoreWindow();

		void CloseWindow();

		bool IsMaximized() const { return m_windowProperties.m_isMaximized; }

		const WindowProperties& GetWindowProperties() const { return m_windowProperties; }

		OSEventMessenger& GetEventMessenger() { EXE_ASSERT(m_windowProperties.m_pMessenger); return *m_windowProperties.m_pMessenger; } // TODO: Don't return reference maybe?

		void Shutdown();

		GLFWwindow* GetGLFWWindow() const { EXE_ASSERT(m_pWindow); return m_pWindow; }
	};
}