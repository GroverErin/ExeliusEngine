#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/SmartPointers.h"

#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(VertexBuffer);
	FORWARD_DECLARE(IndexBuffer);

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

		void AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer) { m_impl.AddVertexBuffer(vertexBuffer); }
		void SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer) { m_impl.SetIndexBuffer(indexBuffer); }

		const eastl::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const { return m_impl.GetVertexBuffers(); }
		const SharedPtr<IndexBuffer>& GetIndexBuffer() const { return m_impl.GetIndexBuffer(); }
	};
}

#if EXELIUS_RENDERER == OPENGL_RENDERER
	#include "source/os/platform/opengl/OpenGLVertexArray.h"
	namespace Exelius
	{
		using VertexArray = _VertexArray<OpenGLVertexArray>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif