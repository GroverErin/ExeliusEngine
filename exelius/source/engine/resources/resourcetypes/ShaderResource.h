#pragma once
#include "source/resource/Resource.h"
#include "source/os/platform/PlatformForwardDeclarations.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Shader);

	class ShaderResource
		: public Resource
	{
		Shader* m_pShader;
	public:
		ShaderResource(const ResourceID& id);
		ShaderResource(const ShaderResource&) = delete;
		ShaderResource(ShaderResource&&) = delete;
		ShaderResource& operator=(const ShaderResource&) = delete;
		virtual ~ShaderResource() final override;

		virtual LoadResult Load(eastl::vector<std::byte>&& data) final override;
		virtual void Unload() final override;

		Shader& GetShader() const { return *m_pShader; }
	};
}