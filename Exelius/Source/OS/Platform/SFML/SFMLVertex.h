#pragma once
#include "Source/Utility/Containers/Vector2.h"
#include "Source/Utility/Generic/Color.h"

namespace sf
{
	class Vertex;
}

namespace Exelius
{
	class SFMLVertex
	{
		sf::Vertex* m_pSFMLVertex;
	public:
		SFMLVertex();

		SFMLVertex(const Vector2f& position);

		SFMLVertex(const Vector2f& position, const Color& color);

		SFMLVertex(const Vector2f& position, const Vector2f& textureCoordinates);

		SFMLVertex(const Vector2f& position, const Color& color, const Vector2f& textureCoordinates);

		SFMLVertex(const SFMLVertex&) = delete;
		SFMLVertex(SFMLVertex&& vertex) noexcept;
		SFMLVertex& operator=(const SFMLVertex&) = delete;
		SFMLVertex& operator=(SFMLVertex&&) = delete;

		~SFMLVertex();

		sf::Vertex& GetSFMLVertex() const { EXE_ASSERT(m_pSFMLVertex); return *m_pSFMLVertex; }
	};
}