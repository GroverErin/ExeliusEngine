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
        //
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

        EXE_ASSERT(jsonDoc.IsObject());

        auto textureMember = jsonDoc.FindMember("Texture");

        if (textureMember == jsonDoc.MemberEnd())
        {
            EXELOG_ENGINE_WARN("No Texture field found. Spritesheets must have a texture.");
            return LoadResult::kFailed;
        }

        EXE_ASSERT(textureMember->value.IsString());
        auto& id = ResourceManager::GetInstance()->LoadNow(textureMember->value.GetString());
        EXE_ASSERT(id.IsValid());

        m_pTextureResource = GetResourceAs<TextureResource>(id, false);

        if (!m_pTextureResource)
        {
            EXELOG_ENGINE_WARN("Failed to load Spritesheet Resource: Texture was nullptr.");

            return LoadResult::kFailed;
        }

        auto spriteMember = jsonDoc.FindMember("Sprites");

        if (spriteMember == jsonDoc.MemberEnd())
        {
            EXELOG_ENGINE_WARN("No Sprite field found. Spritesheets must have at least 1 Sprite. TODO: Maybe no sprite means default value?");
            return LoadResult::kFailed;
        }

        EXE_ASSERT(spriteMember->name.IsString());
        EXE_ASSERT(spriteMember->value.IsObject());
        bool containsSpriteData = false;

        for (auto spriteItr = spriteMember->value.MemberBegin(); spriteItr != spriteMember->value.MemberEnd(); ++spriteItr)
        {
            containsSpriteData = true;

            // Construct the sprite.
            EXE_ASSERT(spriteItr->name.IsString());
            EXE_ASSERT(spriteItr->value.IsObject());

            IRectangle builtRect;

            auto xMember = spriteItr->value.FindMember("sourceX");
            auto yMember = spriteItr->value.FindMember("sourceY");
            auto wMember = spriteItr->value.FindMember("sourceW");
            auto hMember = spriteItr->value.FindMember("sourceH");

            //TODO: More error checking
            if (xMember != spriteItr->value.MemberEnd())
                builtRect.m_left = xMember->value.GetInt();
            if (yMember != spriteItr->value.MemberEnd())
                builtRect.m_top = yMember->value.GetInt();
            if (wMember != spriteItr->value.MemberEnd())
                builtRect.m_width = wMember->value.GetInt();
            if (hMember != spriteItr->value.MemberEnd())
                builtRect.m_height = hMember->value.GetInt();

            m_sprites.try_emplace(spriteItr->name.GetString(), Sprite(*m_pTextureResource->GetTexture(), builtRect));
        }

        if (!containsSpriteData)
        {
            EXELOG_ENGINE_WARN("No Sprite field found. Spritesheets must have at least 1 Sprite. TODO: Maybe no sprite means default value?");
            return LoadResult::kFailed;
        }

        return LoadResult::kKeptRawData;
    }

    void SpritesheetResource::Unload()
    {
        EXE_ASSERT(ResourceManager::GetInstance());
        ResourceManager::GetInstance()->ReleaseResource(m_pTextureResource->GetResourceID());
        m_pTextureResource = nullptr;
    }
}