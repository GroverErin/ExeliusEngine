#pragma once
#include "Source/OS/Platform/PlatformForwardDeclarations.h"
#include "Source/Utility/Generic/Macros.h"

namespace sf
{
	class VertexArray;
}

namespace Exelius
{
	FORWARD_DECLARE(Vertex);

	class SFMLVertexArray
	{
		sf::VertexArray* m_pSFMLVertexArray;

	public:
		SFMLVertexArray(size_t vertexCount);
		SFMLVertexArray(const SFMLVertexArray&) = delete;
		SFMLVertexArray(SFMLVertexArray&& vertex) noexcept = delete; // TODO: This might be useful to change.
		SFMLVertexArray& operator=(const SFMLVertexArray&) = delete;
		SFMLVertexArray& operator=(SFMLVertexArray&&) = delete;
		~SFMLVertexArray();

		size_t GetVertexCount() const;

		void Clear();

		void Resize(size_t vertexCount);

		void Append(const Vertex& vertex);

		sf::VertexArray& GetNativeVertexArray() const { EXE_ASSERT(m_pSFMLVertexArray); return *m_pSFMLVertexArray; }
	};
}