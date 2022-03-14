#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/SmartPointers.h"

#include <EASTL/unordered_map.h>
#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Shader);

	class ShaderLibrary
	{
		eastl::unordered_map<eastl::string, SharedPtr<Shader>> m_shaders;
	public:
		void Add(const eastl::string& name, const SharedPtr<Shader>& shader);
		void Add(const SharedPtr<Shader>& shader);

		SharedPtr<Shader> Load(const eastl::string& filepath);
		SharedPtr<Shader> Load(const eastl::string& name, const eastl::string& filepath);

		SharedPtr<Shader> GetShader(const eastl::string& name);

		bool Exists(const eastl::string& name) const;
	};
}