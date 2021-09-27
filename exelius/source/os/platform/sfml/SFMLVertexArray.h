#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/Macros.h"

/// <summary>
/// SFML namespace, used only in SFML specific code within Exelius.
/// </summary>
namespace sf
{
	class VertexArray;
}

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
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