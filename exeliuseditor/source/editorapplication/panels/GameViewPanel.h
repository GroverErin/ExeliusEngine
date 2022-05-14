#pragma once
#include "EditorPanel.h"

#include <include/Exelius.h>
#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class GameViewPanel
		: public EditorPanel
	{
		eastl::vector<SharedPtr<Framebuffer>> m_framebuffers;

		glm::vec2 m_gameViewSize;
		glm::vec2 m_gameViewBounds[2];

	public:
		GameViewPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene);

		virtual void UpdatePanel() final override;

		virtual void OnImGuiRender() final override;
	};
}
