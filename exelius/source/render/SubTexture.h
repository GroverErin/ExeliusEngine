#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/SmartPointers.h"

#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Texture);

	class SubTexture
	{
		SharedPtr<Texture> m_pTexture;
		glm::vec2 m_textureCoordinates[4];

	public:
		SubTexture(const SharedPtr<Texture>& pTexture, const glm::vec2& min, const glm::vec2& max);

		const SharedPtr<Texture> GetTexture() const { return m_pTexture; }
		const glm::vec2* GetTextureCoordinates() const { return m_textureCoordinates; }

		static SharedPtr<SubTexture> CreateFromCoordinates(const SharedPtr<Texture>& pTexture, const glm::vec2& coordinates, const glm::vec2& spriteSize);
	};
}