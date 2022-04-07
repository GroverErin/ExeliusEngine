#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <cstddef>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Texture);

	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplTexture>
	class _Texture
	{
		ImplTexture m_impl;
	public:
		_Texture(uint32_t width, uint32_t height)
			: m_impl(width, height)
		{
			//
		}

		_Texture(eastl::vector<std::byte>&& data)
			: m_impl(eastl::move(data))
		{
			//
		}

		uint32_t GetWidth() const { return m_impl.GetWidth(); }
		uint32_t GetHeight() const { return m_impl.GetHeight(); }
		uint32_t GetRendererID() const { return m_impl.GetRendererID(); }

		void SetData(void* data, uint32_t size) { m_impl.SetData(data, size); }

		void Bind(uint32_t slot = 0) const { m_impl.Bind(slot); }

		bool IsLoaded() const { return m_impl.IsLoaded(); }

		bool operator==(const Texture& other) const { return m_impl.operator==(other); }
	};
}

#if EXELIUS_RENDERER == OPENGL_RENDERER
	#include "source/os/platform/opengl/OpenGLTexture.h"
	namespace Exelius
	{
		using Texture = _Texture<OpenGLTexture>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif
