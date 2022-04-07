#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/Color.h"
#include "source/utility/generic/SmartPointers.h"
#include "source/render/Window.h"

#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(RendererAPI);

	class Renderer
	{
		Window m_window;
		RendererAPI* m_pRendererAPI;
	public:
		struct RenderStatistics
		{
			uint32_t m_drawCalls = 0;
			uint32_t m_quadCount = 0;

			uint32_t GetTotalVertexCount() const { return m_quadCount * 4; }
			uint32_t GetTotalIndexCount() const { return m_quadCount * 6; }
		};
	protected:
		Renderer::RenderStatistics m_stats;
	public:
		Renderer(eastl::string& windowTitle, const Vector2u& windowSize, bool isVSyncEnabled = true);
		virtual ~Renderer();

		void Update();

		void OnWindowResize(uint32_t width, uint32_t height);

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void SetClearColor(Color color);

		void Clear();

		void ResetRenderStats();
		RenderStatistics GetRenderStats();

		Window& GetWindow() { return m_window; }
	private:

		void Initialize();

		void Shutdown();
	};
}