#pragma once
#include <include/Exelius.h>

#include "Panels/SceneViewPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/DebugPanel.h"

#include <glm/glm.hpp>
#include <filesystem>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class KeyPressedEvent;
	class MouseButtonPressedEvent;
	class Scene;

	class EditorLayer
		: public Layer
	{
		enum class SceneState
		{
			Edit = 0,
			Play = 1
		};
		SceneState m_sceneState;

		std::filesystem::path m_editorScenePath;
		EditorCamera m_editorCamera;

		SharedPtr<Scene> m_pActiveScene;
		SharedPtr<Scene> m_pEditorScene;

		bool m_isEditorActive;

		SceneViewPanel m_sceneViewPanel;
		SceneHierarchyPanel m_sceneHierarchyPanel;
		InspectorPanel m_inspectorPanel;
		DebugPanel m_debugPanel;

	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() final override;
		void OnUpdate() final override;
		virtual void OnImGuiRender() final override;
		void OnEvent(Event& evnt) final override;

		void OpenScene(const std::filesystem::path& path);
	private:
		void ResizeSceneView();

		void InitializeImGuiDockspace();
		void DrawMenuToolbar();

		bool OnKeyPressed(KeyPressedEvent& evnt);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& evnt);

		void NewScene();

		void OpenScene();

		void SaveScene();
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();

		void OnDuplicateGameObject();
	};
}