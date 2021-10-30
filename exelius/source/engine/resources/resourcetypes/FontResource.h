#pragma once
#include "source/resource/Resource.h"
#include "source/utility/math/Rectangle.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class FontResource
		: public Resource
	{
		ResourceID m_textureResourceID;

		// TESTING ONLY
		eastl::string m_text;


		eastl::unordered_map<char, FRectangle> m_glyphs;
		float m_fontDefaultWidth;
		float m_fontDefaultHeight;

	public:
		FontResource(const ResourceID& id);
		FontResource(const FontResource&) = delete;
		FontResource(FontResource&&) = delete;
		FontResource& operator=(const FontResource&) = delete;
		virtual ~FontResource() final override = default;

		virtual LoadResult Load(eastl::vector<std::byte>&& data) final override;
		virtual void Unload() final override;

		FRectangle GetGlyphRect(char c)
		{
			auto found = m_glyphs.find(c);

			if (found == m_glyphs.end())
				return {};

			// TODO: Error check?
			return m_glyphs[c];
		}

		const ResourceID& GetTextureResource() const { return m_textureResourceID; }

		float GetDefaultFontWidth() const { return m_fontDefaultWidth; }
		float GetDefaultFontHeight() const { return m_fontDefaultHeight; }

		// TESTING ONLY
		const eastl::string& GetRawText() const { return m_text; }
	};
}