#pragma once
#include "source/render/RenderHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplFramebuffer>
	class _Framebuffer
	{
		ImplFramebuffer m_impl;
	public:

		_Framebuffer(const FramebufferSpecification& spec)
			: m_impl(spec)
		{
			//
		}

		void Bind() { m_impl.Bind(); }
		void Unbind() { m_impl.Unbind(); }

		void Resize(uint32_t width, uint32_t height) { m_impl.Resize(width, height); }
		int ReadPixel(uint32_t attachmentIndex, int x, int y) { return m_impl.ReadPixel(attachmentIndex, x, y); }

		void ClearAttachment(uint32_t attachmentIndex, int value) { m_impl.ClearAttachment(attachmentIndex, value); }

		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const { return m_impl.GetColorAttachmentRendererID(index); }

		const FramebufferSpecification& GetSpecification() const { return m_impl.GetSpecification(); }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLFramebuffer.h"
	namespace Exelius
	{
		using Framebuffer = _Framebuffer<SFMLFramebuffer>;
	}
#elif EXELIUS_RENDERER == OPENGL_RENDERER
	#include "source/os/platform/opengl/OpenGLFramebuffer.h"
	namespace Exelius
	{
		using Framebuffer = _Framebuffer<OpenGLFramebuffer>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif