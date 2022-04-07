#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/SmartPointers.h"
#include "source/utility/generic/Color.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(VertexArray);

	class OpenGLRendererAPI
	{
	public:
		void Initialize();

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		void SetClearColor(Color color);
		void Clear();

		void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t indexCount = 0);

		void DrawLines(const SharedPtr<VertexArray>& vertexArray, uint32_t vertexCount);

		void SetLineWidth(float width);
	};
}