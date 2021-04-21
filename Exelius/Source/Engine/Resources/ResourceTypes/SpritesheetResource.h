#pragma once
#include "Source/Resource/Resource.h"
#include "Source/OS/Interface/Graphics/Sprite.h"

namespace Exelius
{
	class TextureResource;

	class SpritesheetResource
		: public Resource
	{
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
		virtual void Unload() final override;

		Sprite* GetSprite(const StringIntern& name)
		{
			EXE_ASSERT(name.IsValid());

			auto found = m_sprites.find(name);

			if (found == m_sprites.end())
				return nullptr;

			// TODO: Error check.
			return &m_sprites[name];
		}

		TextureResource* GetTextureResource() const { return m_pTextureResource; }

		// TESTING ONLY
		const eastl::string& GetRawText() const { return m_text; }
	};
}