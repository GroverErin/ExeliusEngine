#include "SceneViewPanel.h"

#include <imgui.h>
#include <filesystem>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	SceneViewPanel::SceneViewPanel()
		: m_pFramebuffer(nullptr)
		, m_sceneViewSize({ 0.0f, 0.0f })
		, m_isSceneViewFocused(false)
		, m_isSceneViewHovered(false)
	{
		m_sceneViewBounds[0] = { 0.0f, 0.0f };
		m_sceneViewBounds[1] = { 0.0f, 0.0f };
	}

	void SceneViewPanel::InitializePanel()
	{
		FramebufferSpecification fbSpec;
		fbSpec.m_attachmentSpec =
		{
			FramebufferTextureFormat::RGBA8,
			FramebufferTextureFormat::RED_INTEGER,
			FramebufferTextureFormat::Depth
		};

		fbSpec.m_width = 1280;
		fbSpec.m_height = 720;
		m_pFramebuffer = MakeShared<Framebuffer>(fbSpec);
	}

	void SceneViewPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Scene View");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_sceneViewBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_sceneViewBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_isSceneViewFocused = ImGui::IsWindowFocused();
		m_isSceneViewHovered = ImGui::IsWindowHovered();
		Application::GetInstance()->GetImGuiLayer()->ShouldBlockEvents(!m_isSceneViewFocused && !m_isSceneViewHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_sceneViewSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_pFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_sceneViewSize.x, m_sceneViewSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewPanel::BeginRenderSceneView()
	{
		Renderer2D::GetInstance()->ResetRenderStats();
		m_pFramebuffer->Bind();
		Renderer2D::GetInstance()->SetClearColor({ 0, 0, 0, 255 });
		Renderer2D::GetInstance()->Clear();

		// Clear our GameObject ID attachment to -1
		m_pFramebuffer->ClearAttachment(1, -1);
	}

	void SceneViewPanel::EndRenderSceneView()
	{
		m_pFramebuffer->Unbind();
	}

	void Exelius::SceneViewPanel::ResizeSceneViewport()
	{
		FramebufferSpecification spec = m_pFramebuffer->GetSpecification();
		if (m_sceneViewSize.x > 0.0f && m_sceneViewSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.m_width != m_sceneViewSize.x || spec.m_height != m_sceneViewSize.y))
		{
			m_pFramebuffer->Resize((uint32_t)m_sceneViewSize.x, (uint32_t)m_sceneViewSize.y);
		}
	}
	
	void Exelius::SceneViewPanel::HandleMousePicking(const SharedPtr<Scene>& pActiveScene)
	{
		ImVec2 mousePosition = ImGui::GetMousePos();
		mousePosition.x -= m_sceneViewBounds[0].x;
		mousePosition.y -= m_sceneViewBounds[0].y;
		glm::vec2 viewportSize = m_sceneViewBounds[1] - m_sceneViewBounds[0];
		mousePosition.y = viewportSize.y - mousePosition.y;
		int mouseX = (int)mousePosition.x;
		int mouseY = (int)mousePosition.y;

		// If we are within the scene viewport.
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int gameObjectID = m_pFramebuffer->ReadPixel(1, mouseX, mouseY);

			if (gameObjectID == -1)
				m_hoveredGameObject = GameObject();
			else
				m_hoveredGameObject = GameObject((entt::entity)gameObjectID, pActiveScene.get());
		}
	}
}