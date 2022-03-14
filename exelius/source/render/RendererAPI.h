#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/Color.h"
#include "source/utility/generic/SmartPointers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(VertexArray);

	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplRendererAPI>
	class _RendererAPI
	{
		ImplRendererAPI m_impl;
	public:
		_RendererAPI()
			: m_impl()
		{
			//
		}

		void Initialize() { m_impl.Initialize(); }

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			m_impl.SetViewport(x, y, width, height);
		}

		void SetClearColor(Color color) { m_impl.SetClearColor(color); }
		void Clear() { m_impl.Clear(); }

		void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t indexCount = 0) { m_impl.DrawIndexed(vertexArray, indexCount); }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLRendererAPI.h"
	namespace Exelius
	{
		using RendererAPI = _RendererAPI<SFMLRendererAPI>;
	}
#elif EXELIUS_RENDERER == OPENGL_RENDERER
	#include "source/os/platform/opengl/OpenGLRendererAPI.h"
	namespace Exelius
	{
		using RendererAPI = _RendererAPI<OpenGLRendererAPI>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif