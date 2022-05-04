#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <filesystem>

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

		ImRect windowRect = {
			{ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y},
			{ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMax().y}
		};

		if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path prefabPath = std::filesystem::path("assets") / path;

				// If it IS loaded, then the reference count would have been incremented.
				// Otherwise, we need to load it (which would increment the ref count).
				ResourceHandle newPrefab(prefabPath.string().c_str(), true); // New resource may or may not be loaded.

				TextFileResource* pTextFile = newPrefab.GetAs<TextFileResource>();

				// If it is loaded and is a valid resource for this component.
				if (!pTextFile || !m_pSceneContext->DeserializeGameObject(prefabPath.string().c_str()))
				{
					EXE_LOG_CATEGORY_WARN("Editor", "Scene Hierarchy cannot accept '{}' as a prefab.", newPrefab.GetID().Get().c_str());
				}
			}
			ImGui::EndDragDropTarget();
		}

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
		if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)gameObject, flags, tag.c_str()))
			ImGui::TreePop(); // Without this, crash occurs when arrow is clicked.

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("Prefab", &gameObject, sizeof(GameObject));
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemClicked())
		{
			m_selectedGameObject = gameObject;
		}

		bool isGameObjectDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete GameObject"))
				isGameObjectDeleted = true;

			// TODO: Add renaming functionality here.

			ImGui::EndPopup();
		}

		if (isGameObjectDeleted)
		{
			m_pSceneContext->DestroyGameObject(gameObject);
			if (m_selectedGameObject == gameObject)
				m_selectedGameObject = {};
		}
	}
}