#include "EXEPCH.h"
#include "OpenGLTexture.h"
#include "source/render/Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
		: m_isLoaded(true)
		, m_width(width)
		, m_height(height)
		, m_rendererID(0)
		, m_internalFormat(0)
		, m_dataFormat(0)
	{
		m_internalFormat = GL_RGBA8;
		m_dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture::OpenGLTexture(eastl::vector<std::byte>&& data)
		: m_isLoaded(false)
		, m_width(0)
		, m_height(0)
		, m_rendererID(0)
		, m_internalFormat(0)
		, m_dataFormat(0)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* stbiData = nullptr;
		{
			stbiData = (stbi_uc*)(data.data());
			stbiData = stbi_load_from_memory(stbiData, (int)data.size(), &width, &height, &channels, 0);
		}

		if (stbiData)
		{
			m_isLoaded = true;

			m_width = width;
			m_height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_internalFormat = internalFormat;
			m_dataFormat = dataFormat;

			if (!(internalFormat & dataFormat))
			{
				EXE_LOG_CATEGORY_FATAL("OpenGLTexture", "Format not supported!");
				EXE_ASSERT(false);
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
			glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height);

			glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, stbiData);

			stbi_image_free(stbiData);
		}
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	uint32_t OpenGLTexture::GetWidth() const
	{
		return m_width;
	}

	uint32_t OpenGLTexture::GetHeight() const
	{
		return m_height;
	}

	uint32_t OpenGLTexture::GetRendererID() const
	{
		return m_rendererID;
	}

	void OpenGLTexture::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
		EXE_ASSERT(size == m_width * m_height * bpp);
		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererID);
	}

	bool OpenGLTexture::IsLoaded() const
	{
		return m_isLoaded;
	}

	bool OpenGLTexture::operator==(const Texture& other) const
	{
		return m_rendererID == other.GetRendererID();
	}
}
