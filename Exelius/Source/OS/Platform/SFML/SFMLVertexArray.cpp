#include "EXEPCH.h"
#include "SFMLVertexArray.h"
#include "Source/OS/Interface/Graphics/Vertex.h"

#include <SFML/Graphics/VertexArray.hpp>

namespace Exelius
{
	SFMLVertexArray::SFMLVertexArray(size_t vertexCount)
		: m_pSFMLVertexArray(new sf::VertexArray(sf::Quads, vertexCount))
	{
		EXE_ASSERT(m_pSFMLVertexArray);
	}

	SFMLVertexArray::~SFMLVertexArray()
	{
		delete m_pSFMLVertexArray;
		m_pSFMLVertexArray = nullptr;
	}

	size_t SFMLVertexArray::GetVertexCount() const
	{
		return m_pSFMLVertexArray->getVertexCount();
	}

	void SFMLVertexArray::Clear()
	{
		m_pSFMLVertexArray->clear();
	}

	void SFMLVertexArray::Resize(size_t vertexCount)
	{
		m_pSFMLVertexArray->resize(vertexCount);
	}

	void SFMLVertexArray::Append(const Vertex& vertex)
	{
		m_pSFMLVertexArray->append(vertex.GetNativeVertex().GetSFMLVertex());
	}
}
