#pragma once

#include "Source/OS/Platform/PlatformForwardDeclarations.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Vertex);

	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplVertexArray>
	class _VertexArray
	{
		ImplVertexArray m_impl;
	public:
		_VertexArray(size_t vertexCount = 0)
			: m_impl(vertexCount)
		{
			//
		}

		size_t GetVertexCount() const { return m_impl.GetVertexCount(); }

		void Clear() { m_impl.Clear(); }

		void Resize(size_t vertexCount) { m_impl.Resize(vertexCount); }

		void Append(const Vertex& vertex) { m_impl.Append(vertex); }

		const ImplVertexArray& GetNativeVertexArray() const { return m_impl; }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "Source/OS/Platform/SFML/SFMLVertexArray.h"
	namespace Exelius
	{
		using VertexArray = _VertexArray<SFMLVertexArray>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif