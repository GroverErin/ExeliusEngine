#pragma once
#include "Source/Resource/Resource.h"
#include "Source/OS/Platform/PlatformForwardDeclarations.h"

namespace Exelius
{
	FORWARD_DECLARE(Texture);

	class TextureResource
		: public Resource
	{
		Texture* m_pTexture;
	public:
		TextureResource(const ResourceID& id);
		TextureResource(const TextureResource&) = delete;
		TextureResource(TextureResource&&) = delete;
		TextureResource& operator=(const TextureResource&) = delete;
		virtual ~TextureResource() final override;

		virtual LoadResult Load(eastl::vector<std::byte>&& data) final override;
		virtual void Unload() final override {}

		Texture* GetTexture() const { return m_pTexture; }
	};
}