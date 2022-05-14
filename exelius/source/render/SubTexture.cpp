#include "EXEPCH.h"
#include "SubTexture.h"
#include "source/render/Texture.h"
#include "source/engine/resources/resourcetypes/TextureResource.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	SubTexture::SubTexture(const ResourceID& textureResource, const glm::vec2& min, const glm::vec2& max)
		: m_textureResource(textureResource)
	{
		m_textureCoordinates[0] = { min.x, min.y };
		m_textureCoordinates[1] = { max.x, min.y };
		m_textureCoordinates[2] = { max.x, max.y };
		m_textureCoordinates[3] = { min.x, max.y };
	}

	SharedPtr<SubTexture> SubTexture::CreateFromCoordinates(const ResourceID& textureResource, const glm::vec2& coordinates, const glm::vec2& spriteSize)
	{
		ResourceHandle textureResourceHandle(textureResource);
		if (!textureResourceHandle.IsReferenceHeld())
			return nullptr; // TODO: Log unable to create.

		TextureResource* pTextureResource = textureResourceHandle.GetAs<TextureResource>();
		if (!pTextureResource)
			return nullptr;

		Texture* pTexture = pTextureResource->GetTexture();
		if (!pTexture)
			return nullptr;

		glm::vec2 min = { (coordinates.x * spriteSize.x) / pTexture->GetWidth(), (coordinates.y * spriteSize.y) / pTexture->GetHeight() };
		glm::vec2 max = { ((coordinates.x + 1) * spriteSize.x) / pTexture->GetWidth(), ((coordinates.y + 1) * spriteSize.y) / pTexture->GetHeight() };
		return MakeShared<SubTexture>(textureResource, min, max);
	}
}