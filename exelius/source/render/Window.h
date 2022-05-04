#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/render/WindowProperties.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(View);
	FORWARD_DECLARE(Texture);
	FORWARD_DECLARE(VertexArray);

	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplWindow>
	class _Window
	{
		ImplWindow m_impl;
	public:
		/// <summary>
		/// The purpose of this constructor is to pass the window parameters to the Implementation Window.
		/// </summary>
		/// <param name="title">The name of the window to be opened.</param>
		/// <param name="width">The width of the window to be opened.</param>
		/// <param name="height">The height of the window to be opened.</param>
		_Window(const WindowProperties& windowProperties)
			: m_impl(windowProperties)
		{
			//
		}

		void InitializeRenderContext() { m_impl.InitializeRenderContext(this); }

		/// <summary>
		/// Calls the implementations update method.
		/// </summary>
		void Update() { m_impl.Update(); }

		ImplWindow& GetNativeWindow() { return m_impl; }

		/// <summary>
		/// Calls the implementations set VSync method.
		/// </summary>
		/// <param name="enabled">Set VSync on or off.</param>
		void SetVSync(bool isEnabled) { m_impl.SetVSync(isEnabled); }

		/// <summary>
		/// Calls the implementations Is VSync method.
		/// </summary>
		/// <returns></returns>
		bool IsVSync() const { return m_impl.IsVSync(); }

		void SetFullscreen(bool setFullscreen = true) { m_impl.SetFullscreen(setFullscreen); }
		bool IsFullscreen() const { return m_impl.IsFullscreen(); }

		void MinimizeWindow() { m_impl.MinimizeWindow(); }

		// Does nothing on fullscreen windows.
		void MaximizeWindow() { m_impl.MaximizeWindow(); }

		void RestoreWindow() { m_impl.RestoreWindow(); }

		// This is NOT the same as Fullscreen. A window can be fullscreen but not maximized (ie minimized/iconified).
		bool IsMaximized() const { return m_impl.IsMaximized(); }

		void CloseWindow() { m_impl.CloseWindow(); }

		const WindowProperties& GetWindowProperties() const { return m_impl.GetWindowProperties(); }

		/// <summary>
		/// Gets the implmentations OS Event messenger.
		/// </summary>
		/// <returns>The implementations OS Event messenger.</returns>
		OSEventMessenger& GetEventMessenger() { return m_impl.GetEventMessenger(); }

		#undef CreateWindow // Defined in WinUser.h :(
		bool CreateWindow(WindowProperties windowProperties)
		{
			m_impl.CreateWindow(windowProperties);
		}

		const glm::vec2& GetWindowPosition() const
		{
			return m_impl.GetWindowPosition();
		}

		const glm::vec2& GetWindowSize() const
		{
			return m_impl.GetWindowSize();
		}
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLWindow.h"
	namespace Exelius
	{
		using Window = _Window<SFMLWindow>;
	}
#elif EXELIUS_RENDERER == OPENGL_RENDERER
	#include "source/os/platform/opengl/OpenGLWindow.h"
	namespace Exelius
	{
		using Window = _Window<OpenGLWindow>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif