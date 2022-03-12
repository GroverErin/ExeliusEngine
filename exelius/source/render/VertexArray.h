#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplVertexArray>
	class _VertexArray
	{
		ImplVertexArray m_impl;
	public:
		_VertexArray()
			: m_impl()
		{
			//
		}

		void Bind() const { m_impl.Bind(); }
		void Unbind() const { m_impl.Unbind(); }

		void AddVertexBuffer(const eastl::unique_ptr<VertexBuffer>& vertexBuffer) { m_impl.AddVertexBuffer(vertexBuffer); }
		void SetIndexBuffer(const eastl::unique_ptr<IndexBuffer>& indexBuffer) { m_impl.SetIndexBuffer(indexBuffer); }

		const std::vector<eastl::unique_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_impl.GetVertexBuffers(); }
		const eastl::unique_ptr<IndexBuffer>& GetIndexBuffer() const { return m_impl.GetIndexBuffer(); }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLVertexArray.h"
	namespace Exelius
	{
		using VertexArray = _VertexArray<SFMLVertexArray>;
	}
#elif EXELIUS_RENDERER == OPENGL_RENDERER
	#include "source/os/platform/opengl/OpenGLVertexArray.h"
	namespace Exelius
	{
		using VertexArray = _VertexArray<OpenGLVertexArray>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif