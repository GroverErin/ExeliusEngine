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
	Renderer::Renderer()
		: m_pRendererAPI(nullptr)
	{
		//
	}

	Renderer::~Renderer()
	{
		Shutdown();
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

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_sceneData.m_viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const eastl::unique_ptr<Shader>& shader, const eastl::unique_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_viewProjection", m_sceneData.m_viewProjectionMatrix);
		shader->SetMat4("u_transform", transform);

		vertexArray->Bind();
		DrawIndexed(vertexArray);
	}

	void Renderer::DrawIndexed(const eastl::unique_ptr<VertexArray>& vertexArray, uint32_t count)
	{
		m_pRendererAPI->DrawIndexed(vertexArray, count);
	}

	void Renderer::Clear()
	{
		m_pRendererAPI->Clear();
	}

	void Renderer::Initialize()
	{
		m_pRendererAPI = EXELIUS_NEW(RendererAPI());
		EXE_ASSERT(m_pRendererAPI);
		m_pRendererAPI->Initialize();
	}

	void Renderer::Shutdown()
	{
		EXELIUS_DELETE(m_pRendererAPI);
	}
}
