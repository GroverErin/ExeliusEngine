#include "EditorLayer.h"
#include "include/Input.h"

#include <source/utility/math/Math.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
		, m_pActiveScene(nullptr)
		, m_pEditorScene(nullptr)
		, m_isEditorActive(true)
		, m_sceneState(SceneState::Edit)
	{
		//
	}

	void EditorLayer::OnAttach()
	{
		m_pEditorScene = MakeShared<Scene>();
		m_pActiveScene = m_pEditorScene;

		m_sceneViewPanel.InitializePanel();
		m_sceneHierarchyPanel.SetContext(m_pActiveScene);

		m_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
	}

	void EditorLayer::OnUpdate()
	{
		ResizeSceneView();
		m_sceneViewPanel.BeginRenderSceneView();

		switch (m_sceneState)
		{
			case SceneState::Edit:
			{
				m_editorCamera.OnUpdate();
				m_pActiveScene->OnUpdateEditor(m_editorCamera);
				break;
			}
			case SceneState::Play:
			{
				m_pActiveScene->OnRuntimeUpdate();
				break;
			}
		}

		m_sceneViewPanel.HandleMousePicking(m_pActiveScene);
		m_sceneViewPanel.EndRenderSceneView();
	}

	void EditorLayer::OnImGuiRender()
	{
		InitializeImGuiDockspace();

		DrawMenuToolbar();
		static bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);

		m_sceneHierarchyPanel.OnImGuiRender();
		GameObject selectedGameObject = m_sceneHierarchyPanel.GetSelectedGameObject();
		m_inspectorPanel.OnImGuiRender(selectedGameObject);
		GameObject hoveredGameObject = m_sceneViewPanel.GetHoveredGameObject();
		m_debugPanel.OnImGuiRender(hoveredGameObject);
		m_sceneViewPanel.OnImGuiRender();

		ImGui::End(); // End Dockspace
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_editorCamera.OnEvent(e);

		if (e.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent* pKeyPressed = static_cast<KeyPressedEvent*>(&e);
			e.m_isHandled = false;
			OnKeyPressed(*pKeyPressed);
		}
		else if (e.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressedEvent* pMouseButton = static_cast<MouseButtonPressedEvent*>(&e);

			e.m_isHandled = false;
			OnMouseButtonPressed(*pMouseButton);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_sceneState != SceneState::Edit)
			OnSceneStop();

		if (File::GetFileExtension(path.string().c_str()) != "excene")
		{
			// Unable to load non scene file.
			return;
		}

		SharedPtr<Scene> newScene = MakeShared<Scene>();

		ResourceLoader::GetInstance()->LoadNow(path.string().c_str());
		if (newScene->DeserializeScene(path.string().c_str()))
		{
			m_pEditorScene = newScene;
			glm::vec2 sceneViewportSize = m_sceneViewPanel.GetSize();
			m_pEditorScene->OnViewportResize((uint32_t)sceneViewportSize.x, (uint32_t)sceneViewportSize.y);
			m_sceneHierarchyPanel.SetContext(m_pEditorScene);
			m_pActiveScene = m_pEditorScene;
			m_editorScenePath = path;
		}
	}

	void EditorLayer::ResizeSceneView()
	{
		m_sceneViewPanel.ResizeSceneViewport();
		glm::vec2 sceneViewportSize = m_sceneViewPanel.GetSize();

		if (sceneViewportSize.x > 0.0f && sceneViewportSize.y > 0.0f)
		{
			m_editorCamera.SetViewportSize(sceneViewportSize.x, sceneViewportSize.y);
			m_pActiveScene->OnViewportResize((uint32_t)sceneViewportSize.x, (uint32_t)sceneViewportSize.y);
		}
	}


	// This code is taken almost directly from the ImGui Demo Window.
	void EditorLayer::InitializeImGuiDockspace()
	{
		static bool dockspaceOpen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (dockspaceOpen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Exelius Editor Dockspace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (dockspaceOpen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("ExeliusDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;
	}

	void EditorLayer::DrawMenuToolbar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit"))
					Application::GetInstance()->CloseApplication();

				ImGui::EndMenu();
			}

			switch (m_sceneState)
			{
			case SceneState::Edit:
				if (ImGui::Button("Play"))
					OnScenePlay();
				break;
			case SceneState::Play:
				if (ImGui::Button("Stop"))
					OnSceneStop();
				break;
			}

			ImGui::EndMenuBar();
		}
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		bool control = IsKeyDown(KeyCode::LControl) || IsKeyDown(KeyCode::RControl);
		bool shift = IsKeyDown(KeyCode::LShift) || IsKeyDown(KeyCode::RShift);

		switch (e.GetKeyCode())
		{
			case KeyCode::N:
			{
				if (control)
					NewScene();

				break;
			}
			case KeyCode::O:
			{
				if (control)
					OpenScene();

				break;
			}
			case KeyCode::S:
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}

				break;
			}

			// Scene Commands
			case KeyCode::D:
			{
				if (control)
					OnDuplicateGameObject();
				break;
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == MouseCode::Left)
		{
			if (m_sceneViewPanel.IsSceneViewHovered() && !IsKeyPressed(KeyCode::LAlt))
				m_sceneHierarchyPanel.SetSelectedGameObject(m_sceneViewPanel.GetHoveredGameObject());
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_pActiveScene = MakeShared<Scene>();
		glm::vec2 sceneViewportSize = m_sceneViewPanel.GetSize();
		m_pActiveScene->OnViewportResize((uint32_t)sceneViewportSize.x, (uint32_t)sceneViewportSize.y);
		m_sceneHierarchyPanel.SetContext(m_pActiveScene);
		m_editorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		eastl::string filepath = Exelius::File::LaunchOpenFileDialog("Exelius Scene (*.excene)\0*.excene\0");
		if (!filepath.empty())
			OpenScene(std::filesystem::path("assets") / std::filesystem::path(filepath.c_str()));
	}

	void EditorLayer::SaveScene()
	{
		if (!m_editorScenePath.empty())
			m_pActiveScene->SerializeScene();
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		eastl::string filepath = File::LaunchSaveFileDialog("Exelius Scene (*.excene)\0*.excene\0");
		if (!filepath.empty())
		{
			m_pEditorScene->SerializeScene(filepath);
		}
	}

	void EditorLayer::OnScenePlay()
	{
		m_sceneState = SceneState::Play;

		m_pActiveScene = Scene::Copy(m_pEditorScene);
		m_pActiveScene->OnRuntimeStart();

		m_sceneHierarchyPanel.SetContext(m_pActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		m_sceneState = SceneState::Edit;
		m_pActiveScene->OnRuntimeStop();
		m_pActiveScene = m_pEditorScene;

		m_sceneHierarchyPanel.SetContext(m_pActiveScene);
	}

	void Exelius::EditorLayer::OnDuplicateGameObject()
	{
		if (m_sceneState != SceneState::Edit)
			return;

		GameObject selectedGameObject = m_sceneHierarchyPanel.GetSelectedGameObject();
		if (selectedGameObject)
			m_pEditorScene->DuplicateGameObject(selectedGameObject);
	}

}