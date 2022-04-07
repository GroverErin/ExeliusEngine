#pragma once
#include "source/resource/Resource.h"
#include "source/os/platform/PlatformForwardDeclarations.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
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
		virtual void Unload() final override;

		void SetTexture(Texture* pTextureToSet) { m_pTexture = pTextureToSet; }
		Texture& GetTexture() const { EXE_ASSERT(m_pTexture); return *m_pTexture; }
	};
}