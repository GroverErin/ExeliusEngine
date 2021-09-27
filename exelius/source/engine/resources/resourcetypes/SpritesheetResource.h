#pragma once
#include "source/resource/Resource.h"
#include "source/os/interface/graphics/Sprite.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	//class TextureResource;

	class SpritesheetResource
		: public Resource
	{
		//TextureResource* m_pTextureResource;
		ResourceID m_textureResourceID;

		// TESTING ONLY
		eastl::string m_text;
		eastl::unordered_map<StringIntern, IRectangle> m_sprites;

	public:
		SpritesheetResource(const ResourceID& id);
		SpritesheetResource(const SpritesheetResource&) = delete;
		SpritesheetResource(SpritesheetResource&&) = delete;
		SpritesheetResource& operator=(const SpritesheetResource&) = delete;
		virtual ~SpritesheetResource() final override = default;

		virtual LoadResult Load(eastl::vector<std::byte>&& data) final override;
		virtual void Unload() final override;

		IRectangle GetSprite(const StringIntern& name)
		{
			EXE_ASSERT(name.IsValid());

			auto found = m_sprites.find(name);

			if (found == m_sprites.end())
				return {};

			// TODO: Error check?
			return m_sprites[name];
		}

		const ResourceID& GetTextureResource() const { return m_textureResourceID; }

		// TESTING ONLY
		const eastl::string& GetRawText() const { return m_text; }
	};
}