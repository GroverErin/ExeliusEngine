#include "EditorLayer.h"
#include "include/Input.h"

#include "panels/AssetPanel.h"
#include "panels/DebugPanel.h"
#include "panels/GameViewPanel.h"
#include "panels/InspectorPanel.h"
#include "panels/SceneHierarchyPanel.h"
#include "panels/SceneViewPanel.h"

#include <imgui.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
		, m_pActiveScene(nullptr)
		, m_pEditorScene(nullptr)
		, m_drawImGuiDemo(false)
		, m_drawStyleEditor(false)
		, m_showPhysicsColliders(true)
		, m_editorState(EditorState::Edit)
	{
		//
	}

	void EditorLayer::OnAttach()
	{
		m_pEditorScene = MakeShared<Scene>();
		m_pActiveScene = m_pEditorScene;

		m_editorPanels.emplace_back(EXELIUS_NEW(AssetPanel(this, m_pActiveScene)));
		m_editorPanels.emplace_back(EXELIUS_NEW(DebugPanel(this, m_pActiveScene)));
		m_editorPanels.emplace_back(EXELIUS_NEW(GameViewPanel(this, m_pActiveScene)));
		m_editorPanels.emplace_back(EXELIUS_NEW(InspectorPanel(this, m_pActiveScene)));
		m_editorPanels.emplace_back(EXELIUS_NEW(SceneHierarchyPanel(this, m_pActiveScene)));
		m_editorPanels.emplace_back(EXELIUS_NEW(SceneViewPanel(this, m_pActiveScene)));

		for (auto* pPanel : m_editorPanels)
		{
			EXE_ASSERT(pPanel);

			pPanel->InitializePanel();
		}
	}

	void EditorLayer::OnDetach()
	{
		for (auto* pPanel : m_editorPanels)
		{
			EXE_ASSERT(pPanel);

			EXELIUS_DELETE(pPanel);
		}
		m_editorPanels.clear();
	}

	void EditorLayer::OnUpdate()
	{
		Renderer2D::GetInstance()->ResetRenderStats();
		for (auto* pPanel : m_editorPanels)
		{
			EXE_ASSERT(pPanel);
			if (pPanel->IsPanelOpen())
				pPanel->UpdatePanel();
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		InitializeImGuiDockspace();

		DrawMenuToolbar();

		for (auto* pPanel : m_editorPanels)
		{
			EXE_ASSERT(pPanel);
			if (pPanel->IsPanelOpen())
				pPanel->OnImGuiRender();
		}

		if (m_drawImGuiDemo)
			ImGui::ShowDemoWindow(&m_drawImGuiDemo);

		if (m_drawStyleEditor)
		{
			ImGui::Begin("Exelius Editor Theme Customizer", &m_drawStyleEditor);
			ImGui::ShowStyleEditor(&ImGui::GetStyle());
			ImGui::End();
		}

		ImGui::End(); // End Dockspace
	}

	void EditorLayer::OnEvent(Event& evnt)
	{
		if (evnt.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent* pKeyPressed = static_cast<KeyPressedEvent*>(&evnt);
			evnt.m_isHandled = false;
			OnKeyPressed(*pKeyPressed);
		}

		if (evnt.m_isHandled)
			return; // Bail because the editor itself used this event.

		for (auto* pPanel : m_editorPanels)
		{
			EXE_ASSERT(pPanel);
			pPanel->OnEvent(evnt);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_editorState != EditorState::Edit)
			OnSceneStop();

		eastl::string fileExtension = File::GetFileExtension(path.string().c_str());

		if (fileExtension != "excene" && fileExtension != "exobj")
		{
			EXE_LOG_CATEGORY_WARN("Editor", "Unable to load file '{}' as a Scene or Prefab.", path.string().c_str());
			return;
		}

		if (fileExtension == "excene")
		{
			ResourceHandle sceneToLoad(path.string().c_str());
			if (!sceneToLoad.IsReferenceHeld())
				sceneToLoad.LoadNow();
			SharedPtr<Scene> newScene = MakeShared<Scene>();

			if (newScene->DeserializeScene(path.string().c_str()))
			{
				m_pEditorScene = newScene;
				m_pActiveScene = m_pEditorScene;
				m_editorScenePath = path;
			}
		}
		else
		{
			// If it IS loaded, then the reference count would have been incremented.
			// Otherwise, we need to load it (which would increment the ref count).
			ResourceHandle newPrefab(path.string().c_str(), true); // New resource may or may not be loaded.

			TextFileResource* pTextFile = newPrefab.GetAs<TextFileResource>();

			// If it is loaded and is a valid resource for this component.
			if (!pTextFile || !m_pActiveScene->DeserializeGameObject(path.string().c_str()))
			{
				EXE_LOG_CATEGORY_WARN("Editor", "Scene Hierarchy cannot accept '{}' as a prefab.", newPrefab.GetID().Get().c_str());
			}
		}
	}

	// This code is taken almost directly from the ImGui Demo Window.
	void EditorLayer::InitializeImGuiDockspace()
	{
		static bool opt_padding = false;
		static bool dockspaceOpen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (dockspaceOpen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
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

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::BeginMenu("Editor Settings"))
				{
					if (ImGui::BeginMenu("Panels"))
					{
						ImGui::MenuItem("ImGui Demo", "", &m_drawImGuiDemo);

						for (auto* pPanel : m_editorPanels)
						{
							EXE_ASSERT(pPanel);
							if (ImGui::MenuItem(pPanel->GetPanelName(), pPanel->GetPanelHotkey(), pPanel->IsPanelOpen()))
								pPanel->SetPanelOpen(!pPanel->IsPanelOpen());
						}
						ImGui::EndMenu();
					}

					ImGui::MenuItem("Show Physics Colliders", "", &m_showPhysicsColliders);
					bool isFullscreen = Renderer2D::GetInstance()->GetWindow().IsFullscreen();
					if (ImGui::MenuItem("Fullscreen Mode", "", &isFullscreen))
						Renderer2D::GetInstance()->GetWindow().SetFullscreen(isFullscreen);

					if (ImGui::MenuItem("Open Theme Customizer"))
						m_drawStyleEditor = true;
					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Exit"))
					Application::GetInstance()->CloseApplication();

				ImGui::EndMenu();
			}

			switch (m_editorState)
			{
			case EditorState::Edit:
				if (ImGui::Button("Play"))
					OnScenePlay();
				break;
			case EditorState::Play:
				if (ImGui::Button("Stop"))
					OnSceneStop();
				break;
			}

			ImGui::EndMenuBar();
		}
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& evnt)
	{
		bool control = IsKeyDown(KeyCode::LControl) || IsKeyDown(KeyCode::RControl);
		bool shift = IsKeyDown(KeyCode::LShift) || IsKeyDown(KeyCode::RShift);

		switch (evnt.GetKeyCode())
		{
			case KeyCode::N:
			{
				if (control)
				{
					NewScene();
					evnt.m_isHandled = true;
				}

				break;
			}
			case KeyCode::O:
			{
				if (control)
				{
					OpenScene();
					evnt.m_isHandled = true;
				}

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

					evnt.m_isHandled = true;
				}

				break;
			}
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		if (m_editorState != EditorState::Edit)
			return;

		m_pActiveScene = MakeShared<Scene>();
		m_pEditorScene = m_pActiveScene;
		m_editorScenePath = std::filesystem::path();

		m_hoveredGameObject = {};
		m_selectedGameObject = {};
	}

	void EditorLayer::OpenScene()
	{
		if (m_editorState != EditorState::Edit)
			return;

		eastl::string filepath = Exelius::File::LaunchOpenFileDialog("Exelius Scene (*.excene)\0*.excene\0");
		if (!filepath.empty())
		{
			OpenScene(std::filesystem::path("assets") / std::filesystem::path(filepath.c_str()));
			m_hoveredGameObject = {};
			m_selectedGameObject = {};
		}
	}

	void EditorLayer::SaveScene()
	{
		if (m_editorState != EditorState::Edit)
			return;

		if (!m_editorScenePath.empty())
			m_pActiveScene->SerializeScene();
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		if (m_editorState != EditorState::Edit)
			return;

		eastl::string filepath = File::LaunchSaveFileDialog("Exelius Scene (*.excene)\0*.excene\0");
		if (!filepath.empty())
		{
			m_pEditorScene->SerializeScene(filepath);
		}
	}

	void EditorLayer::OnScenePlay()
	{
		m_editorState = EditorState::Play;

		m_pActiveScene = Scene::Copy(m_pEditorScene);
		m_pActiveScene->OnRuntimeStart();

		m_hoveredGameObject = {};
		m_selectedGameObject = {};
	}

	void EditorLayer::OnSceneStop()
	{
		m_editorState = EditorState::Edit;
		m_pActiveScene->OnRuntimeStop();
		m_pActiveScene = m_pEditorScene;

		m_hoveredGameObject = {};
		m_selectedGameObject = {};
	}

}
