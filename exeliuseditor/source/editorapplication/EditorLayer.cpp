#include "EditorLayer.h"
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

EditorLayer::EditorLayer()
	: Layer("EditorLayer")
	, m_CameraController(1280.0f / 720.0f)
	, m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void EditorLayer::OnAttach()
{
	m_CheckerboardTexture = Exelius::MakeShared<Exelius::Texture>("assets/textures/tileset.png");
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate()
{
	m_CameraController.OnUpdate();

	// Render
	Exelius::Renderer2D::GetInstance()->ResetStats();
	{
		Exelius::Renderer::GetInstance()->SetClearColor({ 64, 64, 64, 255 });
		Exelius::Renderer::GetInstance()->Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += Exelius::Time::DeltaTime * 50.0f;

		Exelius::Renderer2D::GetInstance()->BeginScene(m_CameraController.GetCamera());
		Exelius::Renderer2D::GetInstance()->DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Exelius::Renderer2D::GetInstance()->DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Exelius::Renderer2D::GetInstance()->DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		Exelius::Renderer2D::GetInstance()->DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 10.0f);
		Exelius::Renderer2D::GetInstance()->DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, 20.0f);
		Exelius::Renderer2D::GetInstance()->EndScene();

		Exelius::Renderer2D::GetInstance()->BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Exelius::Renderer2D::GetInstance()->DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Exelius::Renderer2D::GetInstance()->EndScene();
	}
}

void EditorLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = Exelius::Renderer2D::GetInstance()->GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.m_drawCalls);
	ImGui::Text("Quads: %d", stats.m_quadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void EditorLayer::OnEvent(Exelius::Event& e)
{
	m_CameraController.OnEvent(e);
}
