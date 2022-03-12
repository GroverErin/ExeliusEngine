#pragma once
#include "source/render/RenderHelpers.h"

#include <cstdint>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class OpenGLVertexBuffer
	{
		uint32_t m_rendererID;
		BufferLayout m_layout;
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32_t size);

		const BufferLayout& GetLayout() const { return m_layout; }
		void SetLayout(const BufferLayout& layout) { m_layout = layout; }
	};

	class OpenGLIndexBuffer
	{
		uint32_t m_rendererID;
		uint32_t m_count;
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return m_count; }
	};
}