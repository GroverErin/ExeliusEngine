#include "EXEPCH.h"
#include "TextFileResource.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    TextFileResource::TextFileResource(const ResourceID& id)
        : Resource(id)
    {
        //
    }

    Resource::LoadResult TextFileResource::Load(eastl::vector<std::byte>&& data)
    {
        Log log("ResourceManager");
        m_text = eastl::string((const char*)data.begin(), (const char*)data.end());
        if (m_text.empty())
        {
            log.Warn("Failed to write data to TextFile Resource.");
            return LoadResult::kFailed;
        }

        return LoadResult::kKeptRawData;
    }
}
