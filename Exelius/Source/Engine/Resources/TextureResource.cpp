#include "EXEPCH.h"
#include "TextureResource.h"

Exelius::TextureResource::TextureResource()
    : m_pTexture(nullptr)
{
}

Exelius::TextureResource::~TextureResource()
{
    delete m_pTexture;
    m_pTexture = nullptr;
}

bool Exelius::TextureResource::Load(const std::byte* pData, size_t dataSize)
{
    delete m_pTexture;

    m_pTexture = new Texture();
    if (m_pTexture->LoadFromMemory(pData, dataSize))
        return true;
    return false;
}
