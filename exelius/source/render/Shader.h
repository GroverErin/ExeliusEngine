#pragma once

#include <EASTL/string.h>
#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplShader>
	class _Shader
	{
		ImplShader m_impl;
	public:

		_Shader(const eastl::string& filepath, const eastl::string& shaderSource)
			: m_impl(filepath, shaderSource)
		{
			//
		}

		_Shader(const eastl::string& name, const eastl::string& vertexSrc, const eastl::string& fragmentSrc)
			: m_impl(name, vertexSrc, fragmentSrc)
		{
			//
		}

		void Bind() const { m_impl.Bind(); }
		void Unbind() const { m_impl.Unbind(); }

		void SetInt(const eastl::string& name, int value) { m_impl.SetInt(name, value); }
		void SetIntArray(const eastl::string& name, int* values, uint32_t count) { m_impl.SetIntArray(name, values, count); }
		void SetFloat(const eastl::string& name, float value) { m_impl.SetFloat(name, value); }
		void SetFloat2(const eastl::string& name, const glm::vec2& value) { m_impl.SetFloat2(name, value); }
		void SetFloat3(const eastl::string& name, const glm::vec3& value) { m_impl.SetFloat3(name, value); }
		void SetFloat4(const eastl::string& name, const glm::vec4& value) { m_impl.SetFloat4(name, value); }
		void SetMat4(const eastl::string& name, const glm::mat4& value) { m_impl.SetMat4(name, value); }

		const eastl::string& GetName() const { return m_impl.GetName(); }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLShader.h"
	namespace Exelius
	{
		using Shader = _Shader<SFMLShader>;
	}
#elif EXELIUS_RENDERER == OPENGL_RENDERER
	#include "source/os/platform/opengl/OpenGLShader.h"
	namespace Exelius
	{
		using Shader = _Shader<OpenGLShader>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif