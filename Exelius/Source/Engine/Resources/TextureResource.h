#pragma once
#include "Source/Resource/Resource.h"
#include "Source/Engine/Graphics/Texture.h"

namespace Exelius
{
	class TextureResource
		: public Resource
	{
		Texture* m_pTexture;
	public:
		TextureResource();
		TextureResource(const TextureResource&) = delete;
		TextureResource(TextureResource&&) = delete;
		TextureResource& operator=(const TextureResource&) = delete;
		virtual ~TextureResource() final override;

		virtual bool Load(const std::byte* pData, size_t dataSize) final override;

		Texture* GetTexture() const { return m_pTexture; }
	};
}