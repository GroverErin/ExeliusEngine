#pragma once
#include "source/utility/containers/Vector2.h"
#include "source/os/events/EventManagement.h"

#include "source/os/platform/PlatformForwardDeclarations.h"

#include <EASTL/string.h>

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
		_Window() = default;

		/// <summary>
		/// The purpose of this constructor is to pass the window parameters to the Implementation Window.
		/// </summary>
		/// <param name="title">The name of the window to be opened.</param>
		/// <param name="width">The width of the window to be opened.</param>
		/// <param name="height">The height of the window to be opened.</param>
		_Window(const eastl::string& title, const Vector2u& windowSize)
			: m_impl(title, windowSize)
		{
			//
		}

		void InitializeRenderContext() { m_impl.InitializeRenderContext(this); }

		/// <summary>
		/// Calls the implementations update method.
		/// </summary>
		void Update() { m_impl.Update(); }

		// TODO:
		// This may need to change due to renderer changes.
		void Render() const { m_impl.Render(); }

		void Draw(const VertexArray& vertices, const Texture& texture) { m_impl.Draw(vertices, texture); }

		void Draw(const VertexArray& vertices) { m_impl.Draw(vertices); }

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

		/// <summary>
		/// Gets the implmentations OS Event messenger.
		/// </summary>
		/// <returns>The implementations OS Event messenger.</returns>
		OSEventMessenger& GetEventMessenger() { return m_impl.GetEventMessenger(); }

		#undef CreateWindow // Defined in WinUser.h :(
		bool CreateWindow(const eastl::string& title, const Vector2u& windowSize)
		{
			m_impl.CreateWindow(title, windowSize);
		}

		bool IsOpen() const
		{
			return m_impl.IsOpen();
		}

		Vector2i GetPosition() const
		{
			return m_impl.GetPosition();
		}

		void SetPosition(const Vector2i& position)
		{
			m_impl.SetPosition(position);
		}

		Vector2u GetWindowSize() const
		{
			return m_impl.GetWindowSize();
		}

		void SetWindowSize(const Vector2u& windowSize)
		{
			m_impl.SetWindowSize(windowSize);
		}

		void SetWindowTitle(const eastl::string& title)
		{
			m_impl.SetWindowTitle(title);
		}

		void SetIcon(const Vector2u& size, const uint8_t* pPixels)
		{
			m_impl.SetIcon(size, pPixels);
		}

		void SetVisible(bool isVisible)
		{
			m_impl.SetVisible(isVisible);
		}

		bool HasFocus() const
		{
			return m_impl.HasFocus();
		}

		void Clear()
		{
			m_impl.Clear();
		}

		void SetView(const View& view)
		{
			m_impl.SetView(view);
		}

		const View& GetView() const
		{
			return m_impl.GetView();
		}

		const View& GetDefaultView() const
		{
			return m_impl.GetDefaultView();
		}

		IRectangle GetViewport(const View& view) const
		{
			return m_impl.GetViewport(view);
		}

		Vector2f MapPixelToCoords(const Vector2i& point) const
		{
			return m_impl.MapPixelToCoords(point);
		}

		Vector2f MapPixelToCoords(const Vector2i& point, const View& view) const
		{
			return m_impl.MapPixelToCoords(point, view);
		}

		Vector2i MapCoordsToPixel(const Vector2f& point) const
		{
			return m_impl.MapCoordsToPixel(point);
		}

		Vector2i MapCoordsToPixel(const Vector2f& point, const View& view) const
		{
			return m_impl.MapCoordsToPixel(point, view);
		}

		void PushGLStates()
		{
			m_impl.PushGLStates();
		}

		void PopGLStates()
		{
			m_impl.PopGLStates();
		}

		void ResetGLStates()
		{
			m_impl.ResetGLStates();
		}

		void SetMouseCursorVisible(bool isVisible)
		{
			m_impl.SetMouseCursorVisible(isVisible);
		}

		void SetMouseCursorGrabbed(bool isGrabbed)
		{
			m_impl.SetMouseCursorGrabbed(isGrabbed);
		}

		void SetKeyRepeatEnabled(bool isEnabled)
		{
			m_impl.SetKeyRepeatEnabled(isEnabled);
		}

		void SetFramerateLimit(unsigned int framerateLimit)
		{
			m_impl.SetFramerateLimit(framerateLimit);
		}

		void SetJoystickThreshold(float threshold)
		{
			m_impl.SetJoystickThreshold(threshold);
		}

		bool SetActive(bool isActive) const
		{
			return m_impl.SetActive(isActive);
		}

		void RequestFocus()
		{
			m_impl.RequestFocus();
		}

		void Shutdown()
		{
			m_impl.Shutdown();
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