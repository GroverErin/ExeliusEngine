#pragma once
#include <include/Exelius.h>

#include "panels/EditorPanel.h"
#include "EditorHelpers.h"

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
		std::filesystem::path m_editorScenePath;

		SharedPtr<Scene> m_pActiveScene;
		SharedPtr<Scene> m_pEditorScene;

		eastl::vector<EditorPanel*> m_editorPanels;

		EditorState m_editorState;

		// TODO: Find a better way to allow inter-panel communication.
		GameObject m_selectedGameObject;
		GameObject m_hoveredGameObject;

		// This is nasty. But again, I need better inter-panel communication.
		GameObject m_gameObjectToSaveAsPrefab;

		bool m_drawImGuiDemo;
		bool m_showPhysicsColliders;
		bool m_drawStyleEditor;

	public:

		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() final override;
		virtual void OnDetach() final override;
		void OnUpdate() final override;
		virtual void OnImGuiRender() final override;
		void OnEvent(Event& evnt) final override;

		void OpenScene(const std::filesystem::path& path);

		bool ShouldShowPhysicsColliders() const { return m_showPhysicsColliders; }

		EditorState GetEditorState() const { return m_editorState; }

		// TODO: Find a better way to allow inter-panel communication.
		GameObject GetHoveredGameObject() const { return m_hoveredGameObject; }
		void SetHoveredGameObject(GameObject hoveredGameObject) { m_hoveredGameObject = hoveredGameObject; }
		GameObject GetSelectedGameObject() const { return m_selectedGameObject; }
		void SetSelectedGameObject(GameObject selectedGameObject) { m_selectedGameObject = selectedGameObject; }
		GameObject GetPrefabGameObjectToSave() const { return m_gameObjectToSaveAsPrefab; }
		void SetPrefabGameObjectToSave(GameObject prefabGameObject) { m_gameObjectToSaveAsPrefab = prefabGameObject; }

	private:

		void InitializeImGuiDockspace();
		void DrawMenuToolbar();

		bool OnKeyPressed(KeyPressedEvent& evnt);

		void NewScene();

		void OpenScene();

		void SaveScene();
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();
	};
}