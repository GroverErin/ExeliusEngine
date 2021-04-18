#pragma once
#include "Source/Resource/Resource.h"

// THIS SHOULD NOT BE HERE
#include "Source/Engine/Graphics/Texture.h"

namespace Exelius
{
	//class Texture;
	class TextureResource;

	class SpritesheetResource
		: public Resource
	{
	public:
		struct Sprite
		{
			StringIntern m_name;
		};
	private:
		TextureResource* m_pTextureResource;

		// TESTING ONLY
		eastl::string m_text;
		eastl::unordered_map<StringIntern, Sprite> m_sprites;

	public:
		SpritesheetResource(const ResourceID& id);
		SpritesheetResource(const SpritesheetResource&) = delete;
		SpritesheetResource(SpritesheetResource&&) = delete;
		SpritesheetResource& operator=(const SpritesheetResource&) = delete;
		virtual ~SpritesheetResource() final override;

		virtual LoadResult Load(eastl::vector<std::byte>&& data) final override;
		virtual void Unload() final override {}

		Sprite* GetSprite(const StringIntern& name)
		{
			EXE_ASSERT(name.IsValid());

			// TODO: Error check.
			return &m_sprites[name];
		}

		TextureResource* GetTextureResource() const { return m_pTextureResource; }

		// TESTING ONLY
		const eastl::string& GetRawText() const { return m_text; }
	};
}