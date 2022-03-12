#pragma once
#include "source/utility/containers/Vector2.h"
#include "source/utility/generic/Color.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplVertex>
	class _Vertex
	{
		ImplVertex m_impl;
	public:
		_Vertex()
			: m_impl()
		{
			//
		}

		_Vertex(const Vector2f& position)
			: m_impl(position)
		{
			//
		}

		_Vertex(const Vector2f& position, const Color& color)
			: m_impl(position, color)
		{
			//
		}

		_Vertex(const Vector2f& position, const Vector2f& textureCoordinates)
			: m_impl(position, textureCoordinates)
		{
			//
		}

		_Vertex(const Vector2f& position, const Color& color, const Vector2f& textureCoordinates)
			: m_impl(position, color, textureCoordinates)
		{
			//
		}

		const ImplVertex& GetNativeVertex() const { return m_impl; }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLVertex.h"
	namespace Exelius
	{
		using Vertex = _Vertex<SFMLVertex>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif