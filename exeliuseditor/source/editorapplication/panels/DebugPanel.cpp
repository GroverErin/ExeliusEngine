#include "DebugPanel.h"
#include "editorapplication/EditorLayer.h"

#include <imgui.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	DebugPanel::DebugPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene)
		: EditorPanel(pEditorLayer, pActiveScene, "Debug", false)
	{
		//
	}

	void DebugPanel::OnImGuiRender()
	{
		ImGui::Begin("Debug");

		m_isPanelSelected = ImGui::IsWindowFocused();
		m_isPanelHovered = ImGui::IsWindowHovered();

		eastl::string gameObjectName = "None";

		GameObject hoveredGameObject = m_pEditorLayer->GetHoveredGameObject();

		// We have to do this annoying check because when scenes start and stop, this gameobject doesn't get cleared.
		if (hoveredGameObject && hoveredGameObject.HasComponent<NameComponent>())
			gameObjectName = hoveredGameObject.GetComponent<NameComponent>().m_name;
		ImGui::Text("Hovered GameObject: %s", gameObjectName.c_str());

		ImGui::Separator();
		float dt = Time.DeltaTime * 1000.0f;
		ImGui::Text("DeltaTime: %.3f", dt);
		ImGui::Text("FPS: %.1f", 1.0f / Time.DeltaTime);

		ImGui::Separator();
		float udt = Time.DeltaTimeUnscaled * 1000.0f;
		ImGui::Text("Unscaled DeltaTime: %.3f", udt);
		ImGui::Text("Unscaled FPS: %.1f", 1.0f / Time.DeltaTimeUnscaled);

		ImGui::Separator();
		float et = Time.ElapsedTime;
		ImGui::Text("Elapsed Game Time: %.3f", et);

		ImGui::Separator();
		auto stats = Renderer2D::GetInstance()->GetRenderStats();
		ImGui::Text("Renderer2D Statistics:");
		ImGui::Text("\tDraw Calls: %d", stats.m_drawCalls);
		ImGui::Text("\tQuad Count: %d", stats.m_quadCount);
		ImGui::Text("\tIndex Count: %d", stats.GetTotalIndexCount());
		ImGui::Text("\tVertex Count: %d", stats.GetTotalVertexCount());

		ImGui::End();
	}
}
