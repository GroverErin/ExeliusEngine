#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/Color.h"

#include <EASTL/unique_ptr.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(VertexArray);

	class OpenGLRendererAPI
	{
	public:
		OpenGLRendererAPI();

		void Initialize();

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		void SetClearColor(Color color);
		void Clear();

		void DrawIndexed(const eastl::unique_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
	};
}