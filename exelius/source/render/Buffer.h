#pragma once
#include "source/render/RenderHelpers.h"

#include <cstdint>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplVertexBuffer>
	class _VertexBuffer
	{
		ImplVertexBuffer m_impl;
	public:
		_VertexBuffer(uint32_t size)
			: m_impl(size)
		{
			//
		}

		_VertexBuffer(float* vertices, uint32_t size)
			: m_impl(vertices, size)
		{
			//
		}

		void Bind() const { m_impl.Bind(); }
		void Unbind() const { m_impl.Unbind(); }

		void SetData(const void* data, uint32_t size) { m_impl.SetData(data, size); }

		const BufferLayout& GetLayout() const { return m_impl.GetLayout(); }
		void SetLayout(const BufferLayout& layout) { m_impl.SetLayout(layout); }
	};

	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// 
	/// Currently Exelius only supports 32-bit index buffers
	/// </summary>
	template <class ImplIndexBuffer>
	class _IndexBuffer
	{
		ImplIndexBuffer m_impl;
	public:
		_IndexBuffer(uint32_t* indices, uint32_t count)
			: m_impl(indices, count)
		{
			//
		}

		void Bind() const { m_impl.Bind(); }
		void Unbind() const { m_impl.Unbind(); }

		uint32_t GetCount() const { return m_impl.GetCount(); }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLBuffer.h"
	namespace Exelius
	{
		using VertexBuffer = _VertexBuffer<SFMLVertexBuffer>;
		using IndexBuffer = _IndexBuffer<SFMLIndexBuffer>;
	}
#elif EXELIUS_RENDERER == OPENGL_RENDERER
	#include "source/os/platform/opengl/OpenGLBuffer.h"
	namespace Exelius
	{
		using VertexBuffer = _VertexBuffer<OpenGLVertexBuffer>;
		using IndexBuffer = _IndexBuffer<OpenGLIndexBuffer>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif