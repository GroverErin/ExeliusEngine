#include "SceneHierarchyPanel.h"

#include <imgui.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	SceneHierarchyPanel::SceneHierarchyPanel()
		: m_pSceneContext(nullptr)
	{
		//
	}

	void SceneHierarchyPanel::SetContext(const SharedPtr<Scene>& context)
	{
		m_pSceneContext = context;
		m_selectedGameObject = {}; // Clear the selected object.
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_pSceneContext)
		{
			for (auto gameObject : m_pSceneContext->GetAllGameObjects())
			{
				DrawGameObjectNodeTree(gameObject);
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_selectedGameObject = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("Create Empty GameObject"))
					m_pSceneContext->CreateGameObject("GameObject");

				ImGui::EndPopup();
			}

		}
		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedGameObject(GameObject gameObject)
	{
		m_selectedGameObject = gameObject;
	}

	void SceneHierarchyPanel::DrawGameObjectNodeTree(GameObject gameObject)
	{
		auto& tag = gameObject.GetComponent<NameComponent>().m_name;

		ImGuiTreeNodeFlags flags = ((m_selectedGameObject == gameObject) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)gameObject, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectedGameObject = gameObject;
		}

		bool isGameObjectDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete GameObject"))
				isGameObjectDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags2 = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened2 = ImGui::TreeNodeEx((void*)9817239, flags2, tag.c_str());
			if (opened2)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (isGameObjectDeleted)
		{
			m_pSceneContext->DestroyGameObject(gameObject);
			if (m_selectedGameObject == gameObject)
				m_selectedGameObject = {};
		}
	}
}