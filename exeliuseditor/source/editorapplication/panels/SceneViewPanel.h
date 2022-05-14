#pragma once
#include "EditorPanel.h"

#include <include/Exelius.h>
#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class SceneViewPanel
		: public EditorPanel
	{
		EditorCamera m_editorCamera;

		SharedPtr<Framebuffer> m_pFramebuffer;

		GameObject m_hoveredGameObject;

		glm::vec2 m_sceneViewSize;
		glm::vec2 m_sceneViewBounds[2];

	public:
		SceneViewPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene);

		virtual void InitializePanel() final override;

		virtual void UpdatePanel() final override;

		virtual void OnImGuiRender() final override;

		virtual void OnEvent(Event& evnt) final override;

	private:
		void BeginRenderSceneView();
		void EndRenderSceneView();

		void ResizeSceneView();

		void HandleMousePicking();

		void RenderPhysicsColliders();

		bool OnMouseButtonPressed(MouseButtonPressedEvent& evnt);
		bool OnKeyPressed(KeyPressedEvent& evnt);
	};
}
