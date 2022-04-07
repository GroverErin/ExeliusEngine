#pragma once
#include "source/render/RenderHelpers.h"
#include "source/utility/generic/Macros.h"

#include <EASTL/vector.h>
#include <cstdint>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class OpenGLFramebuffer
	{
		uint32_t m_rendererID;
		FramebufferSpecification m_specification;

		eastl::vector<FramebufferTextureSpecification> m_colorAttachmentSpecifications;
		FramebufferTextureSpecification m_depthAttachmentSpecification;

		eastl::vector<uint32_t> m_colorAttachments;
		uint32_t m_depthAttachment;
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		void Invalidate();

		void Bind();
		void Unbind();

		void Resize(uint32_t width, uint32_t height);
		int ReadPixel(uint32_t attachmentIndex, int x, int y);

		void ClearAttachment(uint32_t attachmentIndex, int value);

		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const { EXE_ASSERT(index < m_colorAttachments.size()); return m_colorAttachments[index]; }

		const FramebufferSpecification& GetSpecification() const { return m_specification; }

	private:
		void CreateTextures(bool isMultisampled, uint32_t* outID, uint32_t count);

		void BindTexture(bool isMultisampled, uint32_t id);

		bool IsDepthFormat(FramebufferTextureFormat format);
	};
}