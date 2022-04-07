#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/resource/ResourceHandle.h"

#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Texture);

	class SubTexture
	{
		ResourceHandle m_textureResource;
		glm::vec2 m_textureCoordinates[4];

	public:
		SubTexture(const ResourceID& textureResource, const glm::vec2& min, const glm::vec2& max);

		const ResourceID& GetTextureResourceID() const { return m_textureResource.GetID(); }
		const glm::vec2* GetTextureCoordinates() const { return m_textureCoordinates; }

		static SharedPtr<SubTexture> CreateFromCoordinates(const ResourceID& textureResource, const glm::vec2& coordinates, const glm::vec2& spriteSize);
	};
}