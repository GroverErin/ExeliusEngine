#pragma once
#include "source/utility/generic/Macros.h"

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <cstdint>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	enum class ShaderDataType : uint8_t
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
		}

		EXE_ASSERT(false);
		return 0;
	}

	struct BufferElement
	{
		eastl::string m_name;
		ShaderDataType m_type;
		uint32_t m_size;
		size_t m_offset;
		bool m_normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const eastl::string& name, bool normalized = false)
			: m_name(name)
			, m_type(type)
			, m_size(ShaderDataTypeSize(type))
			, m_offset(0)
			, m_normalized(normalized)
		{
			//
		}

		uint32_t GetComponentCount() const
		{
			switch (m_type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3; // 3* float3
				case ShaderDataType::Mat4:    return 4; // 4* float4
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}

			EXE_ASSERT(false);
			return 0;
		}
	};

	class BufferLayout
	{
		eastl::vector<BufferElement> m_bufferElements;
		uint32_t m_stride = 0;
	public:
		BufferLayout()
		{
			//
		}

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_bufferElements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_stride; }
		const eastl::vector<BufferElement>& GetElements() const { return m_bufferElements; }

		eastl::vector<BufferElement>::iterator begin() { return m_bufferElements.begin(); }
		eastl::vector<BufferElement>::iterator end() { return m_bufferElements.end(); }
		eastl::vector<BufferElement>::const_iterator begin() const { return m_bufferElements.begin(); }
		eastl::vector<BufferElement>::const_iterator end() const { return m_bufferElements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_stride = 0;
			for (auto& element : m_bufferElements)
			{
				element.m_offset = offset;
				offset += element.m_size;
				m_stride += element.m_size;
			}
		}
	};

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: m_textureFormat(format) {}

		FramebufferTextureFormat m_textureFormat = FramebufferTextureFormat::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: m_attachments(attachments) {}

		eastl::vector<FramebufferTextureSpecification> m_attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		FramebufferAttachmentSpecification m_attachmentSpec;
		uint32_t m_samples = 1;

		bool m_swapChainTarget = false;
	};
}