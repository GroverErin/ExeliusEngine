#pragma once

#include <cstdint>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplUniformBuffer>
	class _UniformBuffer
	{
		ImplUniformBuffer m_impl;
	public:
		_UniformBuffer(uint32_t size, uint32_t binding)
			: m_impl(size, binding)
		{
			//
		}

		void SetData(const void* data, uint32_t size, uint32_t offset = 0) { m_impl.SetData(data, size, offset); }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLUniformBuffer.h"
	namespace Exelius
	{
		using UniformBuffer = _UniformBuffer<SFMLUniformBuffer>;
	}
#elif EXELIUS_RENDERER == OPENGL_RENDERER
	#include "source/os/platform/opengl/OpenGLUniformBuffer.h"
	namespace Exelius
	{
		using UniformBuffer = _UniformBuffer<OpenGLUniformBuffer>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif