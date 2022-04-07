#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"

#include <EASTL/vector.h>
#include <cstddef>

// TODO: Remove
typedef unsigned int GLenum;

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Texture);

	class OpenGLTexture
	{
		eastl::string m_path;
		bool m_isLoaded;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_rendererID;
		GLenum m_internalFormat;
		GLenum m_dataFormat;

	public:
		OpenGLTexture(uint32_t width, uint32_t height);
		OpenGLTexture(eastl::vector<std::byte>&& data);
		~OpenGLTexture();

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		uint32_t GetRendererID() const;

		void SetData(void* data, uint32_t size);

		void Bind(uint32_t slot = 0) const;

		bool IsLoaded() const;

		bool operator==(const Texture& other) const;
	};
}