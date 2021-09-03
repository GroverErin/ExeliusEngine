#include "EXEPCH.h"
#include "SFMLVertex.h"

#include <SFML/Graphics/Vertex.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	SFMLVertex::SFMLVertex()
		: m_pSFMLVertex(new sf::Vertex())
	{
		EXE_ASSERT(m_pSFMLVertex);
	}

	SFMLVertex::SFMLVertex(const Vector2f& position)
		: m_pSFMLVertex(new sf::Vertex({ position.x, position.y }))
	{
		EXE_ASSERT(m_pSFMLVertex);
	}

	SFMLVertex::SFMLVertex(const Vector2f& position, const Color& color)
		: m_pSFMLVertex(new sf::Vertex({ position.x, position.y }, { color.r, color.g, color.b, color.a }))
	{
		EXE_ASSERT(m_pSFMLVertex);
	}

	SFMLVertex::SFMLVertex(const Vector2f& position, const Vector2f& textureCoordinates)
		: m_pSFMLVertex(new sf::Vertex({ position.x, position.y }, { textureCoordinates.x, textureCoordinates.y }))
	{
		EXE_ASSERT(m_pSFMLVertex);
	}

	SFMLVertex::SFMLVertex(const Vector2f& position, const Color& color, const Vector2f& textureCoordinates)
		: m_pSFMLVertex(new sf::Vertex({ position.x, position.y }, { color.r, color.g, color.b, color.a }, { textureCoordinates.x, textureCoordinates.y }))
	{
		EXE_ASSERT(m_pSFMLVertex);
	}

	SFMLVertex::SFMLVertex(SFMLVertex&& vertex) noexcept
	{
		EXE_ASSERT(vertex.m_pSFMLVertex);
		m_pSFMLVertex = vertex.m_pSFMLVertex;
		vertex.m_pSFMLVertex = nullptr;
	}

	SFMLVertex::~SFMLVertex()
	{
		delete m_pSFMLVertex;
		m_pSFMLVertex = nullptr;
	}
}