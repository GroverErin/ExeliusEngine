#include "EXEPCH.h"
#include "OpenGLShader.h"
#include "source/utility/generic/Timing.h"

#include <fstream>
#include <filesystem>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	static GLenum ShaderTypeFromString(const eastl::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		EXE_LOG_CATEGORY_FATAL("ShaderCompiler", "Unknown shader type!");
		EXE_ASSERT(false);
		return 0;
	}

	static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		}

		EXE_LOG_CATEGORY_FATAL("ShaderCompiler", "Unknown shader stage!");
		EXE_ASSERT(false);
		return (shaderc_shader_kind)0;
	}

	static const char* GLShaderStageToString(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		}

		EXE_LOG_CATEGORY_FATAL("ShaderCompiler", "Unknown shader stage!");
		EXE_ASSERT(false);
		return nullptr;
	}

	OpenGLShader::OpenGLShader(const eastl::string& filepath, const eastl::string& shaderSource)
		: m_filepath(filepath)
	{
		CreateCacheDirectoryIfNeeded();
		auto shaderSources = PreProcess(shaderSource);

		Timer timer(true);
		CompileOrGetVulkanBinaries(shaderSources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();
		EXE_LOG_CATEGORY_INFO("ShaderCompiler", "Shader creation took {0} ms", timer.GetElapsedTimeAsMilliseconds());

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const eastl::string& name, const eastl::string& vertexSrc, const eastl::string& fragmentSrc)
		: m_name(name)
	{
		eastl::unordered_map<GLenum, eastl::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_rendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_rendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const eastl::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const eastl::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const eastl::string& name, float value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const eastl::string& name, const glm::vec2& value)
	{
		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const eastl::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const eastl::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const eastl::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const eastl::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const eastl::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const eastl::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const eastl::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::UploadUniformFloat3(const eastl::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const eastl::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const eastl::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const eastl::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::CreateCacheDirectoryIfNeeded()
	{
		eastl::string cacheDirectory = s_kCacheDirectory;
		if (!std::filesystem::exists(cacheDirectory.c_str()))
			std::filesystem::create_directories(cacheDirectory.c_str());
	}

	const char* OpenGLShader::GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";
		}

		EXE_LOG_CATEGORY_FATAL("ShaderCompiler", "Unknown shader stage!");
		EXE_ASSERT(false);
		return "";
	}

	const char* OpenGLShader::GLShaderStageCachedVulkanFileExtension(uint32_t stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";
		}

		EXE_LOG_CATEGORY_FATAL("ShaderCompiler", "Unknown shader stage!");
		EXE_ASSERT(false);
		return "";
	}

	eastl::unordered_map<GLenum, eastl::string> OpenGLShader::PreProcess(const eastl::string& source)
	{
		eastl::unordered_map<GLenum, eastl::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);

		//Start of shader type declaration line
		size_t pos = source.find(typeToken, 0);
		while (pos != eastl::string::npos)
		{
			//End of shader type declaration line.
			size_t eol = source.find_first_of("\r\n", pos);
			if (eol == eastl::string::npos)
			{
				EXE_LOG_CATEGORY_FATAL("ShaderCompiler", "Syntax Error!");
				EXE_ASSERT(false);
			}

			//Start of shader type name (after "#type " keyword).
			size_t begin = pos + typeTokenLength + 1;
			eastl::string type = source.substr(begin, eol - begin);
			if (!ShaderTypeFromString(type))
			{
				EXE_LOG_CATEGORY_FATAL("ShaderCompiler", "Invalid shader type '{}' specified!", type.c_str());
				EXE_ASSERT(false);
			}

			//Start of shader code after shader type declaration line.
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			if (nextLinePos == eastl::string::npos)
			{
				EXE_LOG_CATEGORY_FATAL("ShaderCompiler", "Syntax Error!");
				EXE_ASSERT(false);
			}

			//Start of next shader type declaration line.
			pos = source.find(typeToken, nextLinePos);

			shaderSources[ShaderTypeFromString(type)] = (pos == eastl::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const eastl::unordered_map<GLenum, eastl::string>& shaderSources)
	{
		glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = s_kCacheDirectory;

		auto& shaderData = m_vulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_filepath.c_str();
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize((size_t)size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source.c_str(), GLShaderStageToShaderC(stage), m_filepath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					EXE_LOG_CATEGORY_ERROR("ShaderCompiler", module.GetErrorMessage());
					EXE_ASSERT(false);
				}

				shaderData[stage] = eastl::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_openGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = s_kCacheDirectory;

		shaderData.clear();
		m_openGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_vulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_filepath.c_str();
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize((size_t)size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv.data(), spirv.size());
				m_openGLSourceCode[stage] = glslCompiler.compile().c_str();
				auto& source = m_openGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source.c_str(), GLShaderStageToShaderC(stage), m_filepath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{

					EXE_LOG_CATEGORY_ERROR("ShaderCompiler", module.GetErrorMessage());
					EXE_ASSERT(false);
				}

				shaderData[stage] = eastl::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_openGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			EXE_LOG_CATEGORY_ERROR("ShaderCompiler", "Shader linking failed ({0}):\n{1}", m_filepath.c_str(), infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_rendererID = program;
	}

	void OpenGLShader::Reflect(GLenum stage, const eastl::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData.data(), shaderData.size());
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		EXE_LOG_CATEGORY_TRACE("ShaderCompiler", "OpenGLShader::Reflect - {0} {1}", GLShaderStageToString(stage), m_filepath.c_str());
		EXE_LOG_CATEGORY_TRACE("ShaderCompiler", "    {0} uniform buffers", resources.uniform_buffers.size());
		EXE_LOG_CATEGORY_TRACE("ShaderCompiler", "    {0} resources", resources.sampled_images.size());

		EXE_LOG_CATEGORY_TRACE("ShaderCompiler", "Uniform buffers:");

		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			EXE_LOG_CATEGORY_TRACE("ShaderCompiler", "  {0}", resource.name);
			EXE_LOG_CATEGORY_TRACE("ShaderCompiler", "    Size = {0}", bufferSize);
			EXE_LOG_CATEGORY_TRACE("ShaderCompiler", "    Binding = {0}", binding);
			EXE_LOG_CATEGORY_TRACE("ShaderCompiler", "    Members = {0}", memberCount);
		}
	}
}