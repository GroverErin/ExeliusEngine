#pragma once

#include <EASTL/string.h>
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <glm/glm.hpp>

// TODO: Remove
typedef unsigned int GLenum;

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class OpenGLShader
	{
		// TODO: This should NOT be hardcoded.
		static constexpr const char* s_kCacheDirectory = "assets/cache/shader/opengl";

		uint32_t m_rendererID;
		eastl::string m_filepath;
		eastl::string m_name;

		eastl::unordered_map<GLenum, eastl::vector<uint32_t>> m_vulkanSPIRV;
		eastl::unordered_map<GLenum, eastl::vector<uint32_t>> m_openGLSPIRV;

		eastl::unordered_map<GLenum, eastl::string> m_openGLSourceCode;
	public:
		OpenGLShader(const eastl::string& filepath, const eastl::string& shaderSource);

		OpenGLShader(const eastl::string& name, const eastl::string& vertexSrc, const eastl::string& fragmentSrc);

		~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		void SetInt(const eastl::string& name, int value);
		void SetIntArray(const eastl::string& name, int* values, uint32_t count);
		void SetFloat(const eastl::string& name, float value);
		void SetFloat2(const eastl::string& name, const glm::vec2& value);
		void SetFloat3(const eastl::string& name, const glm::vec3& value);
		void SetFloat4(const eastl::string& name, const glm::vec4& value);
		void SetMat4(const eastl::string& name, const glm::mat4& value);

		const eastl::string& GetName() const { return m_name; }

		void UploadUniformInt(const eastl::string& name, int value);
		void UploadUniformIntArray(const eastl::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const eastl::string& name, float value);
		void UploadUniformFloat2(const eastl::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const eastl::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const eastl::string& name, const glm::vec4& value);

		void UploadUniformMat3(const eastl::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const eastl::string& name, const glm::mat4& matrix);

	private:
		void CreateCacheDirectoryIfNeeded();
		const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage);
		const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage);

		eastl::unordered_map<GLenum, eastl::string> PreProcess(const eastl::string& source);

		void CompileOrGetVulkanBinaries(const eastl::unordered_map<GLenum, eastl::string>& shaderSources);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(GLenum stage, const eastl::vector<uint32_t>& shaderData);
	};
}