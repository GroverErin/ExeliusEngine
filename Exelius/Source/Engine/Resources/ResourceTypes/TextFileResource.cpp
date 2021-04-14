#include "EXEPCH.h"
#include "TextFileResource.h"

namespace Exelius
{
    TextFileResource::TextFileResource(const ResourceID& id)
        : Resource(id)
    {
        //
    }

    Resource::LoadResult TextFileResource::Load(eastl::vector<std::byte>&& data)
    {
        m_text = eastl::string((const char*)data.begin(), (const char*)data.end());
        if (m_text.empty())
        {
            EXELOG_ENGINE_WARN("Failed to write data to TextFile Resource.");
            return LoadResult::kFailed;
        }

        return LoadResult::kKeptRawData;
    }
}
