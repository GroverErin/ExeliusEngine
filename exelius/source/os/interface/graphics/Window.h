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

		Vector2u GetWindowSize() const
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