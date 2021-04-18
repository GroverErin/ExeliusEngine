#include "EXEPCH.h"
#include "SpritesheetResource.h"

#include "Source/Engine/Resources/ResourceRetrieval.h"

#include <rapidjson/document.h>

namespace Exelius
{
    SpritesheetResource::SpritesheetResource(const ResourceID& id)
        : Resource(id)
        , m_pTextureResource(nullptr)
    {
        //
    }

    SpritesheetResource::~SpritesheetResource()
    {
        ResourceManager::GetInstance()->ReleaseResource(m_pTextureResource->GetResourceID());
        m_pTextureResource = nullptr;
    }

    Resource::LoadResult SpritesheetResource::Load(eastl::vector<std::byte>&& data)
    {
        delete m_pTextureResource;

        m_text = eastl::string((const char*)data.begin(), (const char*)data.end());
        if (m_text.empty())
        {
            EXELOG_ENGINE_WARN("Failed to read data in Spritesheet Resource.");
            return LoadResult::kFailed;
        }

        rapidjson::Document jsonDoc;
        if (jsonDoc.Parse(m_text.c_str()).HasParseError())
        {
            EXELOG_ENGINE_ERROR("Failed to Parse JSON.");
            return LoadResult::kFailed;
        }

        auto textureMember = jsonDoc.FindMember("Texture");

        EXE_ASSERT(jsonDoc.IsObject());

        if (textureMember == jsonDoc.MemberEnd())
        {
            EXELOG_ENGINE_WARN("No Texture field found. Spritesheets must have a texture.");
            return LoadResult::kFailed;
        }

        EXE_ASSERT(textureMember->value.IsString());
        auto id = ResourceManager::GetInstance()->LoadNow(textureMember->value.GetString());
        EXE_ASSERT(id.IsValid());

        m_pTextureResource = GetResourceAs<TextureResource>(id, false);

        if (!m_pTextureResource)
        {
            EXELOG_ENGINE_WARN("Failed to load Spritesheet Resource: Texture was nullptr.");

            return LoadResult::kFailed;
        }

        return LoadResult::kKeptRawData;
    }
}