#pragma once

#include "source/os/platform/PlatformForwardDeclarations.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Window);

	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplRenderContext>
	class _RenderContext
	{
		ImplRenderContext m_impl;
	public:

		_RenderContext()
			: m_impl()
		{
			//
		}

		void Initialize(Window* pWindow) { m_impl.Initialize(pWindow); }
		void SwapBuffers() { m_impl.SwapBuffers(); }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLRenderContext.h"
	namespace Exelius
	{
		using RenderContext = _RenderContext<SFMLRenderContext>;
	}
#elif EXELIUS_RENDERER == OPENGL_RENDERER
	#include "source/os/platform/opengl/OpenGLRenderContext.h"
	namespace Exelius
	{
		using RenderContext = _RenderContext<OpenGLRenderContext>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif