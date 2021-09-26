#include "EXEPCH.h"
#include "TextureResource.h"
#include "Source/OS/Interface/Graphics/Texture.h"

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
    }

    Resource::LoadResult TextureResource::Load(eastl::vector<std::byte>&& data)
    {
        delete m_pTexture;

        m_pTexture = new Texture();
        if (m_pTexture->LoadFromMemory(data.data(), data.size()))
            return LoadResult::kKeptRawData;
        return LoadResult::kFailed;
    }

    void TextureResource::Unload()
    {
        delete m_pTexture;
        m_pTexture = nullptr;
    }
}