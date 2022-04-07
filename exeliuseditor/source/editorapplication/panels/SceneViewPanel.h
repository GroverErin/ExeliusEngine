#pragma once
#include <include/Exelius.h>

#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class SceneViewPanel
	{
		SharedPtr<Framebuffer> m_pFramebuffer;

		GameObject m_hoveredGameObject;

		glm::vec2 m_sceneViewSize;
		glm::vec2 m_sceneViewBounds[2];

		bool m_isSceneViewFocused;
		bool m_isSceneViewHovered;

	public:
		SceneViewPanel();

		void InitializePanel();

		void OnImGuiRender();

		void BeginRenderSceneView();
		void EndRenderSceneView();

		void ResizeSceneViewport();

		void HandleMousePicking(const SharedPtr<Scene>& pActiveScene);

		GameObject GetHoveredGameObject() { return m_hoveredGameObject; }

		bool IsSceneViewHovered() const { return m_isSceneViewHovered; }

		const glm::vec2& GetSize() const { return m_sceneViewSize; }
	};
}
