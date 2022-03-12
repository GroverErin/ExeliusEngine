#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(VertexBuffer);
	FORWARD_DECLARE(IndexBuffer);

	class OpenGLVertexArray
	{
		uint32_t m_rendererID;
		uint32_t m_vertexBufferIndex = 0;
		std::vector<eastl::unique_ptr<VertexBuffer>> m_vertexBuffers;
		eastl::unique_ptr<IndexBuffer> m_indexBuffer;

	public:

		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const eastl::unique_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const eastl::unique_ptr<IndexBuffer>& indexBuffer);

		const std::vector<eastl::unique_ptr<VertexBuffer>>& GetVertexBuffers() const;
		const eastl::unique_ptr<IndexBuffer>& GetIndexBuffer() const;
	};
}