#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/SmartPointers.h"

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
		std::vector<SharedPtr<VertexBuffer>> m_vertexBuffers;
		SharedPtr<IndexBuffer> m_indexBuffer;

	public:

		OpenGLVertexArray();
		~OpenGLVertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer);

		const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const;
		const SharedPtr<IndexBuffer>& GetIndexBuffer() const;
	};
}