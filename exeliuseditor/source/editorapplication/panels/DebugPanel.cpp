#include "DebugPanel.h"
#include <imgui.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void DebugPanel::OnImGuiRender(GameObject hoveredGameObject)
	{
		ImGui::Begin("Debug");

		eastl::string gameObjectName = "None";

		// We have to do this annoying check because when scenes start and stop, this gameobject doesn't get cleared.
		if (hoveredGameObject && hoveredGameObject.HasComponent<NameComponent>())
			gameObjectName = hoveredGameObject.GetComponent<NameComponent>().m_name;
		ImGui::Text("Hovered GameObject: %s", gameObjectName.c_str());

		ImGui::Separator();
		float dt = Time.DeltaTime * 1000.0f;
		ImGui::Text("DeltaTime: %.3f", dt);
		ImGui::Text("FPS: %.1f", 1.0f / Time.DeltaTime);

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
