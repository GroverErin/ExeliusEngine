#include "AssetPanel.h"
#include "editorapplication/EditorLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	AssetPanel::AssetPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene)
		: EditorPanel(pEditorLayer, pActiveScene, "Assets")
		, m_currentFilePath("assets")
		, m_emptyFolderIcon("assets/textures/editoricons/emptyfoldericon.png")
		, m_folderIcon("assets/textures/editoricons/foldericon.png")
		, m_textIcon("assets/textures/editoricons/texticon.png")
		, m_luaIcon("assets/textures/editoricons/luaicon.png")
		, m_prefabIcon("assets/textures/editoricons/prefabicon.png")
		, m_sceneIcon("assets/textures/editoricons/sceneicon.png")
		, m_audioIcon("assets/textures/editoricons/audioicon.png")
		, m_imageIcon("assets/textures/editoricons/imageicon.png")
		, m_fontIcon("assets/textures/editoricons/fonticon.png")
		, m_zeroSizeIcon("assets/textures/editoricons/zerosizeicon.png")
		, m_unknownFileTypeIcon("assets/textures/editoricons/unknownicon.png")
	{
		// TODO: Queue these all together when threads are re-enabled.
		m_emptyFolderIcon.LoadNow();
		m_folderIcon.LoadNow();
		m_textIcon.LoadNow();
		m_luaIcon.LoadNow();
		m_prefabIcon.LoadNow();
		m_sceneIcon.LoadNow();
		m_audioIcon.LoadNow();
		m_imageIcon.LoadNow();
		m_fontIcon.LoadNow();
		m_zeroSizeIcon.LoadNow();
		m_unknownFileTypeIcon.LoadNow();
	}

	void AssetPanel::UpdatePanel()
	{
		GameObject potentialPrefab = m_pEditorLayer->GetPrefabGameObjectToSave();
		if (potentialPrefab)
		{
			eastl::string prefabFileName = potentialPrefab.GetName() + ".exobj";
			std::filesystem::path prefabPath = m_currentFilePath.c_str();
			prefabPath /= prefabFileName.c_str();

			// If it IS loaded, then the reference count would have been incremented.
			ResourceHandle newPrefab(prefabPath.string().c_str()); // New resource may or may not be loaded.

			if (!newPrefab.CreateNew(prefabPath.string().c_str()))
				EXE_LOG_CATEGORY_WARN("Editor", "Failed to create prefab resource '{}'", prefabPath.string().c_str());

			if (TextFileResource* pResource = newPrefab.GetAs<TextFileResource>())
			{
				pResource->SetRawText(m_pActiveScene->SerializeGameObject(potentialPrefab));
				newPrefab.SaveResource();
				m_pEditorLayer->SetPrefabGameObjectToSave({});
			}
			else
			{
				EXE_LOG_CATEGORY_WARN("Editor", "Failed to create prefab resource '{}'", prefabPath.string().c_str());
			}
		}
	}

	void AssetPanel::OnImGuiRender()
	{
		ImGui::Begin("Assets");

		m_isPanelSelected = ImGui::IsWindowFocused();
		m_isPanelHovered = ImGui::IsWindowHovered();

		ImRect windowRect = {
			{ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y},
			{ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMax().y}
		};

		if (ImGui::BeginDragDropTargetCustom(windowRect, ImGui::GetCurrentWindow()->ID))
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Prefab"))
			{
				GameObject* pGameObject = (GameObject*)payload->Data;

				eastl::string prefabFileName = pGameObject->GetName() + ".exobj";
				std::filesystem::path prefabPath = m_currentFilePath.c_str();
				prefabPath /= prefabFileName.c_str();

				// If it IS loaded, then the reference count would have been incremented.
				ResourceHandle newPrefab(prefabPath.string().c_str()); // New resource may or may not be loaded.

				if (!newPrefab.CreateNew(prefabPath.string().c_str()))
					EXE_LOG_CATEGORY_WARN("Editor", "Failed to create prefab resource '{}'", prefabPath.string().c_str());

				if (TextFileResource* pResource = newPrefab.GetAs<TextFileResource>())
				{
					pResource->SetRawText(m_pActiveScene->SerializeGameObject(*pGameObject));
					newPrefab.SaveResource();
				}
				else
				{
					EXE_LOG_CATEGORY_WARN("Editor", "Failed to create prefab resource '{}'", prefabPath.string().c_str());
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (m_currentFilePath != "assets")
		{
			if (ImGui::Button("Back"))
			{
				std::filesystem::path path(m_currentFilePath.c_str());
				m_currentFilePath = path.parent_path().string().c_str();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentFilePath.c_str()))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, "assets");
			eastl::string filenameString = relativePath.filename().string().c_str();

			ImGui::PushID(filenameString.c_str());
			ResourceID icon;
			uintmax_t fileSize = std::filesystem::file_size(path);

			eastl::string fileExtension = File::GetFileExtension(filenameString);

			if (directoryEntry.is_directory() && std::filesystem::is_empty(path))
				icon = m_emptyFolderIcon.GetID();
			else if (directoryEntry.is_directory())
				icon = m_folderIcon.GetID();
			else if (fileSize <= 0)
				icon = m_zeroSizeIcon.GetID();
			else if (fileExtension == "ttf")
				icon = m_fontIcon.GetID();
			else if (fileExtension == "wav" || fileExtension == "ogg" || fileExtension == "flac")
				icon = m_audioIcon.GetID();
			else if (fileExtension == "lua")
				icon = m_luaIcon.GetID();
			else if (fileExtension == "txt" || fileExtension == "glsl")
				icon = m_textIcon.GetID();
			else if (fileExtension == "exobj")
				icon = m_prefabIcon.GetID();
			else if (fileExtension == "excene")
				icon = m_sceneIcon.GetID();
			else if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "bmp")
				icon = m_imageIcon.GetID();
			else
				icon = m_unknownFileTypeIcon.GetID();

			ResourceHandle iconResource(icon);

			TextureResource* pTextureResource = iconResource.GetAs<TextureResource>();
			if (!pTextureResource)
				return;

			Texture* pTexture = pTextureResource->GetTexture();

			if (!pTexture)
				return;

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)pTexture->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("Asset", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			if (directoryEntry.is_directory())
			{
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Prefab"))
					{
						GameObject* pGameObject = (GameObject*)payload->Data;

						eastl::string prefabFileName = pGameObject->GetName() + ".exobj";
						std::filesystem::path prefabPath = path / prefabFileName.c_str();

						// If it IS loaded, then the reference count would have been incremented.
						ResourceHandle newPrefab(prefabPath.string().c_str()); // New resource may or may not be loaded.

						if (!newPrefab.CreateNew(prefabPath.string().c_str()))
							EXE_LOG_CATEGORY_WARN("Editor", "Failed to create prefab resource '{}'", prefabPath.string().c_str());

						if (TextFileResource* pResource = newPrefab.GetAs<TextFileResource>())
						{
							pResource->SetRawText(m_pActiveScene->SerializeGameObject(*pGameObject));
							newPrefab.SaveResource();
						}
						else
						{
							EXE_LOG_CATEGORY_WARN("Editor", "Failed to create prefab resource '{}'", prefabPath.string().c_str());
						}
					}
					ImGui::EndDragDropTarget();
				}
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				std::filesystem::path temppath(m_currentFilePath.c_str());

				if (directoryEntry.is_directory())
					temppath /= path.filename();

				m_currentFilePath = temppath.string().c_str();
			}
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete"))
				{
					if (directoryEntry.is_directory())
						std::filesystem::remove_all(path);
					std::filesystem::remove(path);
				}
				ImGui::EndPopup();
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::End();
	}
}
