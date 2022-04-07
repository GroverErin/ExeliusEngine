#pragma once
#include "source/os/events/EventManagement.h"
#include "source/utility/containers/Vector2.h"

#include "source/os/platform/PlatformForwardDeclarations.h"

#include <EASTL/string.h>

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
		struct WindowData
		{
			eastl::string m_title;
			Vector2u m_windowSize;
			bool m_isVSync = false;
			OSEventMessenger m_messenger;
		};

		WindowData m_windowData;

		RenderContext* m_pRenderContext;

		GLFWwindow* m_pWindow;
		bool m_isVSync;

	public:
		OpenGLWindow(const eastl::string& title, const Vector2u& windowSize, bool isVSyncEnabled);
		OpenGLWindow(const OpenGLWindow&) = delete;
		OpenGLWindow(OpenGLWindow&&) = delete;
		OpenGLWindow& operator=(const OpenGLWindow&) = delete;
		OpenGLWindow& operator=(OpenGLWindow&&) = delete;
		~OpenGLWindow();

#undef CreateWindow // Defined in WinUser.h :(
		bool CreateWindow(const eastl::string& title, const Vector2u& windowSize, bool isVSyncEnabled);

		void InitializeRenderContext(Window* pAbstractWindow);

		void Update();

		Vector2u GetWindowSize() const;

		void SetVSync(bool isEnabled);

		bool IsVSync() const;

		OSEventMessenger& GetEventMessenger() { return m_windowData.m_messenger; }

		void Shutdown();

		GLFWwindow* GetGLFWWindow() const { EXE_ASSERT(m_pWindow); return m_pWindow; }
	};
}