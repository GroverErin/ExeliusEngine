#include "EXEPCH.h"
#include "FontResource.h"
#include "source/resource/ResourceHandle.h"

#include <rapidjson/document.h>
/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FontResource::FontResource(const ResourceID& id)
		: Resource(id)
        , m_fontDefaultWidth(0.0f)
        , m_fontDefaultHeight(0.0f)
	{
		//
	}

	Resource::LoadResult FontResource::Load(eastl::vector<std::byte>&& data)
	{
        // Set the raw byte data to a string value.
        m_text = eastl::string((const char*)data.begin(), (const char*)data.end());
        if (m_text.empty())
        {
            EXE_LOG_CATEGORY_WARN("ResourceManager", "Failed to read data in Spritesheet Resource.");
            return LoadResult::kFailed;
        }

        // Parse the text as JSON data.
        rapidjson::Document jsonDoc;
        if (jsonDoc.Parse(m_text.c_str()).HasParseError())
        {
            EXE_LOG_CATEGORY_ERROR("ResourceManager", "Failed to Parse JSON.");
            return LoadResult::kFailed;
        }

        EXE_ASSERT(jsonDoc.IsObject());

        // Find and load the Texture linked to this spritesheet.
        auto textureMember = jsonDoc.FindMember("Texture");

        if (textureMember == jsonDoc.MemberEnd())
        {
            EXE_LOG_CATEGORY_WARN("ResourceManager", "No Texture field found. Spritesheets must have a texture.");
            return LoadResult::kFailed;
        }

        // Set and queue the texture for loading.
        EXE_ASSERT(textureMember->value.IsString());
        m_textureResourceID = textureMember->value.GetString();
        EXE_ASSERT(m_textureResourceID.IsValid());

        ResourceHandle textureResource(m_textureResourceID);
        //EXE_ASSERT(textureResource.IsReferenceHeld());

        // TODO:
        //  This might be more efficient if this is false, so that all loading can be queued together.
        //  Although, this will need to be nicely managed by the user in some way... otherwise,
        //  the resource thread will never know if the queue is ready to process. Something to think about...
        textureResource.QueueLoad(true);
        textureResource.LockResource();

        // THis may not exist, which is okay.
        auto monospaced = jsonDoc.FindMember("Monospaced");
        if (monospaced != jsonDoc.MemberEnd())
        {
            EXE_ASSERT(monospaced->value.IsObject());

            auto monoData = monospaced->value.FindMember("MonoData");

            EXE_ASSERT(monoData->value.IsArray());
            EXE_ASSERT(monoData->value.GetArray().Size() == 4); // SHouldn't be more or less than 4.


            // Get the monospaced data consists of the monospaced dimensions (w + h)
            // as well as the iterative boundaries. The boundaries are how far the
            // data will iterate through the image before ending the source rect
            // extraction. This is provided in dimensions as well (w + h).
            int monoDataValues[4] = { 0,0,0,0 };
            int index = 0;
            for (const auto& value : monoData->value.GetArray())
            {
                EXE_ASSERT(value.IsNumber());
                monoDataValues[index] = value.GetInt();
                ++index;
            }

            m_fontDefaultWidth = (float)monoDataValues[0];
            m_fontDefaultHeight = (float)monoDataValues[1];

            auto nameOrder = monospaced->value.FindMember("NameOrder");

            if (nameOrder == jsonDoc.MemberEnd())
            {
                EXE_LOG_CATEGORY_WARN("ResourceManager", "Monospaced spritesheets must provide a name table.");
                return LoadResult::kFailed;
            }

            EXE_ASSERT(nameOrder->value.IsArray());

            int x = 0;
            int y = 0;
            for (const auto& name : nameOrder->value.GetArray())
            {
                EXE_ASSERT(name.IsString());

                if (y > monoDataValues[3])
                {
                    break;
                }

                IRectangle sourceRect(0, 0, 0, 0);
                sourceRect.x = x;
                sourceRect.y = y;
                sourceRect.w = monoDataValues[0];
                sourceRect.h = monoDataValues[1];
                m_glyphs.try_emplace(*(name.GetString()), sourceRect);

                x += monoDataValues[0];

                if (x >= monoDataValues[2])
                {
                    x = 0;
                    y += monoDataValues[1];
                }
            }

        }

		return LoadResult::kKeptRawData;
	}

	void FontResource::Unload()
	{
		ResourceHandle textureResource(m_textureResourceID);
		textureResource.UnlockResource();
	}
}