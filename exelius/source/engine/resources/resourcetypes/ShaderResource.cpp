#include "EXEPCH.h"
#include "ShaderResource.h"
#include "source/render/Shader.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    ShaderResource::ShaderResource(const ResourceID& id)
        : Resource(id)
        , m_pShader(nullptr)
    {
        //
    }

    ShaderResource::~ShaderResource()
    {
    }

    Resource::LoadResult ShaderResource::Load(eastl::vector<std::byte>&& data)
    {
        eastl::string shaderSourceText = eastl::string((const char*)data.begin(), (const char*)data.end());
        if (shaderSourceText.empty())
        {
            EXE_LOG_CATEGORY_WARN("ShaderResource", "Failed to load resource, data is empty.");
            return LoadResult::kFailed;
        }

        m_pShader = EXELIUS_NEW(Shader(GetResourceID().Get(), shaderSourceText));

        if (!m_pShader)
            return LoadResult::kFailed;

        return LoadResult::kKeptRawData;
    }

    void ShaderResource::Unload()
    {
        delete m_pShader;
        m_pShader = nullptr;
    }
}