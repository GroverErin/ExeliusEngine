#include "SceneHierarchyPanel.h"
#include "editorapplication/EditorLayer.h"
#include "include/Input.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <filesystem>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	SceneHierarchyPanel::SceneHierarchyPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene)
		: EditorPanel(pEditorLayer, pActiveScene, "Scene Hierarchy")
		, m_isRenamingGameObject(false)
	{
		//
	}

	void SceneHierarchyPanel::InitializePanel()
	{
	}

	void SceneHierarchyPanel::UpdatePanel()
	{
		if (m_selectedGameObject != m_pEditorLayer->GetSelectedGameObject())
			m_selectedGameObject = m_pEditorLayer->GetSelectedGameObject();
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_isPanelSelected = ImGui::IsWindowFocused();
		m_isPanelHovered = ImGui::IsWindowHovered();

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
				if (!pTextFile || !m_pActiveScene->DeserializeGameObject(prefabPath.string().c_str()))
				{
					EXE_LOG_CATEGORY_WARN("Editor", "Scene Hierarchy cannot accept '{}' as a prefab.", newPrefab.GetID().Get().c_str());
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (m_pActiveScene)
		{
			for (auto gameObject : m_pActiveScene->GetAllGameObjects())
			{
				DrawGameObjectNodeTree(gameObject);
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				m_selectedGameObject = {};
				m_renamingGameObject = {};
				m_isRenamingGameObject = false;
			}

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("Create Empty GameObject"))
					m_pActiveScene->CreateGameObject("GameObject");

				ImGui::EndPopup();
			}

		}
		ImGui::End();
	}

	void SceneHierarchyPanel::OnEvent(Event& evnt)
	{
		if (!m_isPanelSelected && !m_isPanelHovered)
			return;


		if (evnt.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent* pKeyPressed = static_cast<KeyPressedEvent*>(&evnt);
			evnt.m_isHandled = false;

			bool control = IsKeyDown(KeyCode::LControl) || IsKeyDown(KeyCode::RControl);
			//bool shift = IsKeyDown(KeyCode::LShift) || IsKeyDown(KeyCode::RShift);

			switch (pKeyPressed->GetKeyCode())
			{
				// Scene Commands
				case KeyCode::D:
				{
					if (control)
					{
						DuplicateGameObject();
						evnt.m_isHandled = true;
					}
					break;
				}
			}
		}
	}

	void SceneHierarchyPanel::DrawGameObjectNodeTree(GameObject gameObject)
	{
		auto& tag = gameObject.GetComponent<NameComponent>().m_name;
		ImGuiTreeNodeFlags flags = ((m_selectedGameObject == gameObject && m_renamingGameObject != gameObject) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		if (m_isRenamingGameObject && gameObject == m_renamingGameObject)
		{
			if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)gameObject, flags, ""))
				ImGui::TreePop(); // Without this, crash occurs when arrow is clicked.

			char buffer[256]; // TODO: This limit should be imposed in the actual component.
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));

			// Take up the entire width.
			ImGui::SameLine();
			ImGui::PushItemWidth(-1);
			// ImGui hides anything after '##'
			if (ImGui::InputText("##NameComponent", buffer, sizeof(buffer), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
			{
				gameObject.GetComponent<NameComponent>().m_name = eastl::string(buffer);
				m_isRenamingGameObject = false;
				m_renamingGameObject = {};
			}
			ImGui::PopItemWidth();
		}
		else
		{
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
			flags |= ImGuiTreeNodeFlags_SpanFullWidth;
			if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)gameObject, flags, tag.c_str()))
				ImGui::TreePop(); // Without this, crash occurs when arrow is clicked.
		}

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("Prefab", &gameObject, sizeof(GameObject));
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemClicked() && gameObject != m_renamingGameObject)
		{
			m_selectedGameObject = gameObject;
			m_pEditorLayer->SetSelectedGameObject(m_selectedGameObject);
			m_renamingGameObject = {};
			m_isRenamingGameObject = false;
		}

		bool isGameObjectDeleted = false;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete GameObject"))
				isGameObjectDeleted = true;

			if (ImGui::MenuItem("Rename GameObject"))
			{
				m_isRenamingGameObject = true;
				m_renamingGameObject = gameObject;
			}

			if (ImGui::MenuItem("Duplicate GameObject"))
				m_pActiveScene->DuplicateGameObject(gameObject, tag + " (Copy)");

			if (ImGui::MenuItem("Save As Prefab"))
				m_pEditorLayer->SetPrefabGameObjectToSave(gameObject);

			ImGui::EndPopup();
		}

		if (isGameObjectDeleted)
		{
			m_pActiveScene->DestroyGameObject(gameObject);
			if (m_selectedGameObject == gameObject)
				m_selectedGameObject = {};
			m_pEditorLayer->SetSelectedGameObject(m_selectedGameObject);
		}
	}

	void SceneHierarchyPanel::DuplicateGameObject()
	{
		if (m_pEditorLayer->GetEditorState() != EditorState::Edit)
			return;

		if (m_selectedGameObject)
			m_pActiveScene->DuplicateGameObject(m_selectedGameObject);
	}
}