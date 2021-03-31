#include "EXEPCH.h"
#include "JSONResource.h"

namespace Exelius
{
    JSONResource::JSONResource(const ResourceID& id)
        : Resource(id)
    {
        //
    }

    Resource::LoadResult JSONResource::Load(eastl::vector<std::byte>&& data)
    {
        m_text = eastl::string((const char*)data.begin(), (const char*)data.end());
        if (m_text.empty())
        {
            EXELOG_ENGINE_WARN("Failed to read data from JSON Resource.");
            return LoadResult::kFailed;
        }

        return LoadResult::kKeptRawData;
    }
}
