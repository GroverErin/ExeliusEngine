#pragma once

#include <cstdint>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class OpenGLUniformBuffer
	{
		uint32_t m_rendererID;
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		~OpenGLUniformBuffer();

		void SetData(const void* data, uint32_t size, uint32_t offset = 0);
	};
}