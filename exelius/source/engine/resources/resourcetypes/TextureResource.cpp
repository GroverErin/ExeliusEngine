#include "EXEPCH.h"
#include "TextureResource.h"
#include "source/render/Texture.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    TextureResource::TextureResource(const ResourceID& id)
        : Resource(id)
        , m_pTexture(nullptr)
    {
        //
    }

    TextureResource::~TextureResource()
    {
        if (m_pTexture)
            m_pTexture->Unbind();
    }

    Resource::LoadResult TextureResource::Load(eastl::vector<std::byte>&& data)
    {
        m_pTexture = EXELIUS_NEW(Texture(eastl::move(data)));
        if (!m_pTexture)
        {
            EXE_LOG_CATEGORY_WARN("TextureResource", "Failed to load resource, texture was not successfully created.");
            EXELIUS_DELETE(m_pTexture);
            return LoadResult::kFailed;
        }

        return LoadResult::kKeptRawData;
    }

    void TextureResource::Unload()
    {
        delete m_pTexture;
        m_pTexture = nullptr;
    }
}