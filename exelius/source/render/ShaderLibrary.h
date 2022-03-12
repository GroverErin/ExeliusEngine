#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"

#include <EASTL/unordered_map.h>
#include <EASTL/string.h>
#include <EASTL/unique_ptr.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Shader);

	class ShaderLibrary
	{
		eastl::unordered_map<eastl::string, eastl::unique_ptr<Shader>> m_shaders;
	public:
		void Add(const eastl::string& name, const eastl::unique_ptr<Shader>& shader);
		void Add(const eastl::unique_ptr<Shader>& shader);

		eastl::unique_ptr<Shader> Load(const eastl::string& filepath);
		eastl::unique_ptr<Shader> Load(const eastl::string& name, const eastl::string& filepath);

		eastl::unique_ptr<Shader> GetShader(const eastl::string& name);

		bool Exists(const eastl::string& name) const;
	};
}