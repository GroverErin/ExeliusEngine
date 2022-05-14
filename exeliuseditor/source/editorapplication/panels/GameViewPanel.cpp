#include "GameViewPanel.h"
#include "editorapplication/EditorLayer.h"

#include <imgui.h>
#include <filesystem>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	GameViewPanel::GameViewPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene)
		: EditorPanel(pEditorLayer, pActiveScene, "Game View")
		, m_gameViewSize({ 0.0f, 0.0f })
	{
		m_gameViewBounds[0] = { 0.0f, 0.0f };
		m_gameViewBounds[1] = { 0.0f, 0.0f };
	}

	void GameViewPanel::UpdatePanel()
	{
		if (m_pEditorLayer->GetEditorState() == EditorState::Play)
			m_pActiveScene->OnRuntimeUpdate();

		m_framebuffers.clear();

		FramebufferSpecification fbSpec;
		fbSpec.m_attachmentSpec =
		{
			FramebufferTextureFormat::RGBA8,
			FramebufferTextureFormat::RED_INTEGER,
			FramebufferTextureFormat::Depth
		};

		auto view = m_pActiveScene->GetAllGameObjectsWith<TransformComponent, CameraComponent>();
		for (auto gameObjectWithCamera : view)
		{
			auto [transformComponent, cameraComponent] = view.get<TransformComponent, CameraComponent>(gameObjectWithCamera);
			if (!cameraComponent.m_isActive)
				continue;

			float viewportWidth = m_gameViewSize.x * cameraComponent.m_viewportRect.z;
			float viewportHeight = m_gameViewSize.y * cameraComponent.m_viewportRect.w;

			if (viewportWidth <= 0.0f || viewportHeight <= 0.0f)
				continue;

			fbSpec.m_width = (uint32_t)(viewportWidth);
			fbSpec.m_height = (uint32_t)(viewportHeight);
			auto pFramebuffer = MakeShared<Framebuffer>(fbSpec);
			m_framebuffers.emplace_back(pFramebuffer);

			pFramebuffer->Bind();

			pFramebuffer->ClearAttachment(1, -1);
			Renderer2D::GetInstance()->SetClearColor({ 0, 0, 0, 255 });
			Renderer2D::GetInstance()->Clear();

			cameraComponent.m_camera.SetViewportSize((uint32_t)viewportWidth, (uint32_t)viewportHeight);

			m_pActiveScene->OnRuntimeRender(cameraComponent.m_camera, transformComponent.GetTransform());

			pFramebuffer->Unbind();
		}
	}

	void GameViewPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(m_pPanelName);

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_gameViewBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_gameViewBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_isPanelSelected = ImGui::IsWindowFocused();
		m_isPanelHovered = ImGui::IsWindowHovered();
		//Application::GetInstance()->GetImGuiLayer()->ShouldBlockEvents(!m_isPanelSelected && !m_isPanelHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_gameViewSize = { viewportPanelSize.x, viewportPanelSize.y };

		ImVec2 currentDrawPos = ImGui::GetCursorPos();

		auto view = m_pActiveScene->GetAllGameObjectsWith<CameraComponent>();
		int index = -1;
		for (auto gameObjectWithCamera : view)
		{
			++index;
			auto& cameraComponent = view.get<CameraComponent>(gameObjectWithCamera);
			if (!cameraComponent.m_isActive)
				continue;

			if (index < (int)m_framebuffers.size())
			{
				auto& pFramebuffer = m_framebuffers[index];

				ImVec2 localPos = currentDrawPos;
				localPos.x += cameraComponent.m_viewportRect.x * viewportPanelSize.x;
				localPos.y += cameraComponent.m_viewportRect.y * viewportPanelSize.y;

				uint64_t textureID = pFramebuffer->GetColorAttachmentRendererID();
				ImGui::SetCursorPos(localPos);
				ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_gameViewSize.x * cameraComponent.m_viewportRect.z, m_gameViewSize.y * cameraComponent.m_viewportRect.w }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}
}