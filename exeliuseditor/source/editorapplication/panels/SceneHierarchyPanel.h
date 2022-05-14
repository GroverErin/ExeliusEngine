#pragma once
#include "EditorPanel.h"

#include <include/Exelius.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class SceneHierarchyPanel
		: public EditorPanel
	{
		GameObject m_selectedGameObject;
		GameObject m_renamingGameObject;

		bool m_isRenamingGameObject;

	public:
		SceneHierarchyPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene);

		virtual void InitializePanel() final override;

		virtual void UpdatePanel() final override;

		virtual void OnImGuiRender() final override;

		virtual void OnEvent(Event& evnt) final override;

	private:
		void DrawGameObjectNodeTree(GameObject gameObject);

		void DuplicateGameObject();
	};
}