#include "EXEPCH.h"
#include "Renderer.h"
#include "source/render/RendererAPI.h"
#include "source/render/Shader.h"
#include "source/render/VertexArray.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	Renderer::Renderer(const WindowProperties& windowProperties)
		: m_window(windowProperties)
		, m_pRendererAPI(nullptr)
	{
		Initialize();
	}

	Renderer::~Renderer()
	{
		Shutdown();
	}

	void Renderer::Update()
	{
		m_window.Update();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		m_pRendererAPI->SetViewport(0, 0, width, height);
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		m_pRendererAPI->SetViewport(x, y, width, height);
	}

	void Renderer::SetClearColor(Color color)
	{
		m_pRendererAPI->SetClearColor(color);
	}

	void Renderer::Clear()
	{
		m_pRendererAPI->Clear();
	}

	void Renderer::ResetRenderStats()
	{
		memset(&m_stats, 0, sizeof(RenderStatistics));
	}

	Renderer::RenderStatistics Renderer::GetRenderStats()
	{
		return m_stats;
	}

	void Renderer::Initialize()
	{
		m_window.InitializeRenderContext();

		m_pRendererAPI = EXELIUS_NEW(RendererAPI());
		EXE_ASSERT(m_pRendererAPI);
		m_pRendererAPI->Initialize();
	}

	void Renderer::Shutdown()
	{
		EXELIUS_DELETE(m_pRendererAPI);
	}
}
