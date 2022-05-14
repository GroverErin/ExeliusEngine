#include "SceneViewPanel.h"
#include "editorapplication/EditorLayer.h"
#include "include/Input.h"

#include <imgui.h>
#include <filesystem>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	SceneViewPanel::SceneViewPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene)
		: EditorPanel(pEditorLayer, pActiveScene, "Scene View")
		, m_pFramebuffer(nullptr)
		, m_sceneViewSize({ 0.0f, 0.0f })
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

		fbSpec.m_width = (uint32_t)Renderer2D::GetInstance()->GetWindow().GetWindowSize().x;
		fbSpec.m_height = (uint32_t)Renderer2D::GetInstance()->GetWindow().GetWindowSize().y;
		m_pFramebuffer = MakeShared<Framebuffer>(fbSpec);
	}

	void SceneViewPanel::UpdatePanel()
	{
		ResizeSceneView();
		BeginRenderSceneView();

		m_editorCamera.OnUpdate();
		m_pActiveScene->OnUpdateEditor(m_editorCamera);

		HandleMousePicking();

		RenderPhysicsColliders();

		EndRenderSceneView();
	}

	void SceneViewPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(m_pPanelName);

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_sceneViewBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_sceneViewBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_isPanelSelected = ImGui::IsWindowFocused();
		m_isPanelHovered = ImGui::IsWindowHovered();
		//Application::GetInstance()->GetImGuiLayer()->ShouldBlockEvents(!m_isPanelSelected && !m_isPanelHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_sceneViewSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_pFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_sceneViewSize.x, m_sceneViewSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				if (m_pEditorLayer)
					m_pEditorLayer->OpenScene(std::filesystem::path("assets") / path);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewPanel::OnEvent(Event& evnt)
	{
		if (!m_isPanelSelected && !m_isPanelHovered)
			return;

		m_editorCamera.OnEvent(evnt);

		if (evnt.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressedEvent* pMouseButton = static_cast<MouseButtonPressedEvent*>(&evnt);

			evnt.m_isHandled = false;
			OnMouseButtonPressed(*pMouseButton);
		}
		else if (evnt.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent* pKeyPressed = static_cast<KeyPressedEvent*>(&evnt);
			evnt.m_isHandled = false;
			OnKeyPressed(*pKeyPressed);
		}
	}

	void SceneViewPanel::ResizeSceneView()
	{
		FramebufferSpecification spec = m_pFramebuffer->GetSpecification();
		if (m_sceneViewSize.x > 0.0f && m_sceneViewSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.m_width != m_sceneViewSize.x || spec.m_height != m_sceneViewSize.y))
		{
			m_pFramebuffer->Resize((uint32_t)m_sceneViewSize.x, (uint32_t)m_sceneViewSize.y);
		}

		if (m_sceneViewSize.x > 0.0f && m_sceneViewSize.y > 0.0f)
		{
			m_editorCamera.SetViewportSize(m_sceneViewSize.x, m_sceneViewSize.y);
			m_pActiveScene->OnViewportResize((uint32_t)m_sceneViewSize.x, (uint32_t)m_sceneViewSize.y);
		}
	}

	void SceneViewPanel::RenderPhysicsColliders()
	{
		if (m_pEditorLayer->ShouldShowPhysicsColliders())
		{
			if (m_pEditorLayer->GetEditorState() == EditorState::Play)
			{
				auto cameraView = m_pActiveScene->GetAllGameObjectsWith<TransformComponent, CameraComponent>();
				for (auto gameObjectWithCamera : cameraView)
				{
					auto [cameraTransformComponent, cameraComponent] = cameraView.get<TransformComponent, CameraComponent>(gameObjectWithCamera);

					if (!cameraComponent.m_isActive)
						continue;

					Renderer2D::GetInstance()->Begin2DScene(cameraComponent.m_camera, cameraTransformComponent.GetTransform());

					// Transforms
					{
						auto view = m_pActiveScene->GetAllGameObjectsWith<TransformComponent>();
						for (auto gameObject : view)
						{
							auto transformComponent = view.get<TransformComponent>(gameObject);

							Renderer2D::GetInstance()->DrawRect(transformComponent.GetTransform(), glm::vec4(0, 1, 1, 1));
						}
					}

					// Box Colliders
					{
						auto view = m_pActiveScene->GetAllGameObjectsWith<TransformComponent, BoxColliderComponent>();
						for (auto gameObject : view)
						{
							auto [transformComponent, colliderComponent] = view.get<TransformComponent, BoxColliderComponent>(gameObject);

							glm::vec3 colliderTranslation = glm::vec3(colliderComponent.m_offset, 0.001f);
							glm::vec3 colliderScale = glm::vec3(colliderComponent.m_size * 2.0f, 1.0f);
							glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), colliderTranslation)
								* glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f))
								* glm::scale(glm::mat4(1.0f), colliderScale);

							Renderer2D::GetInstance()->DrawRect(transformComponent.GetTransform() * colliderTransform, glm::vec4(0, 1, 0, 1));
						}
					}

					// Circle Colliders
					{
						auto view = m_pActiveScene->GetAllGameObjectsWith<TransformComponent, CircleColliderComponent>();
						for (auto gameObject : view)
						{
							auto [transformComponent, colliderComponent] = view.get<TransformComponent, CircleColliderComponent>(gameObject);

							glm::vec3 translation = transformComponent.m_translation + glm::vec3(colliderComponent.m_offset, 0.001f);
							glm::vec3 scale = transformComponent.m_scale * glm::vec3(colliderComponent.m_radius * 2.0f);

							glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
								* glm::scale(glm::mat4(1.0f), scale);

							Renderer2D::GetInstance()->DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
						}
					}

					Renderer2D::GetInstance()->End2DScene();
				}
			}
			else
			{
				Renderer2D::GetInstance()->Begin2DScene(m_editorCamera);

				// Transforms
				{
					auto view = m_pActiveScene->GetAllGameObjectsWith<TransformComponent>();
					for (auto gameObject : view)
					{
						auto transformComponent = view.get<TransformComponent>(gameObject);

						Renderer2D::GetInstance()->DrawRect(transformComponent.GetTransform(), glm::vec4(0, 1, 1, 1));
					}
				}

				// Box Colliders
				{
					auto view = m_pActiveScene->GetAllGameObjectsWith<TransformComponent, BoxColliderComponent>();
					for (auto gameObject : view)
					{
						auto [transformComponent, colliderComponent] = view.get<TransformComponent, BoxColliderComponent>(gameObject);

						glm::vec3 colliderTranslation = glm::vec3(colliderComponent.m_offset, 0.001f);
						glm::vec3 colliderScale = glm::vec3(colliderComponent.m_size * 2.0f, 1.0f);
						glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), colliderTranslation)
							* glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f))
							* glm::scale(glm::mat4(1.0f), colliderScale);

						Renderer2D::GetInstance()->DrawRect(transformComponent.GetTransform() * colliderTransform, glm::vec4(0, 1, 0, 1));
					}
				}

				// Circle Colliders
				{
					auto view = m_pActiveScene->GetAllGameObjectsWith<TransformComponent, CircleColliderComponent>();
					for (auto gameObject : view)
					{
						auto [transformComponent, colliderComponent] = view.get<TransformComponent, CircleColliderComponent>(gameObject);

						glm::vec3 translation = transformComponent.m_translation + glm::vec3(colliderComponent.m_offset, 0.001f);
						glm::vec3 scale = transformComponent.m_scale * glm::vec3(colliderComponent.m_radius * 2.0f);

						glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
							* glm::scale(glm::mat4(1.0f), scale);

						Renderer2D::GetInstance()->DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
					}
				}

				Renderer2D::GetInstance()->End2DScene();
			}
		}
	}

	bool SceneViewPanel::OnMouseButtonPressed(MouseButtonPressedEvent& evnt)
	{
		if (evnt.GetMouseButton() == MouseCode::Left)
		{
			if (m_isPanelHovered && !IsKeyPressed(KeyCode::LAlt))
				m_pEditorLayer->SetSelectedGameObject(m_hoveredGameObject);
		}
		return false;
	}

	bool SceneViewPanel::OnKeyPressed(KeyPressedEvent& evnt)
	{
		bool control = IsKeyDown(KeyCode::LControl) || IsKeyDown(KeyCode::RControl);
		bool shift = IsKeyDown(KeyCode::LShift) || IsKeyDown(KeyCode::RShift);

		switch (evnt.GetKeyCode())
		{
			case KeyCode::R:
			{
				m_editorCamera = {};
				break;
			}
		}

		return false;
	}

	void SceneViewPanel::BeginRenderSceneView()
	{
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
	
	void SceneViewPanel::HandleMousePicking()
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
				m_hoveredGameObject = GameObject((entt::entity)gameObjectID, m_pActiveScene.get());

			m_pEditorLayer->SetHoveredGameObject(m_hoveredGameObject);
		}
	}
}