#include "EXEPCH.h"
#include "ShaderLibrary.h"

#include "source/render/Shader.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void ShaderLibrary::Add(const eastl::string& name, const eastl::unique_ptr<Shader>& shader)
	{
		EXE_ASSERT(!Exists(name));
		m_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const eastl::unique_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	eastl::unique_ptr<Shader> ShaderLibrary::Load(const eastl::string& filepath)
	{
		auto shader = EXELIUS_NEW(Shader(filepath));
		Add(shader);
		return shader;
	}

	eastl::unique_ptr<Shader> ShaderLibrary::Load(const eastl::string& name, const eastl::string& filepath)
	{
		auto shader = EXELIUS_NEW(Shader(filepath));
		Add(name, shader);
		return shader;
	}

	eastl::unique_ptr<Shader> ShaderLibrary::GetShader(const eastl::string& name)
	{
		return m_shaders[name];
	}

	bool ShaderLibrary::Exists(const eastl::string& name) const
	{
		return m_shaders.find(name) != m_shaders.end();
	}
}
