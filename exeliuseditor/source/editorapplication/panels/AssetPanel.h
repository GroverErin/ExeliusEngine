#pragma once
#include "EditorPanel.h"

#include <include/Exelius.h>
#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class AssetPanel
		: public EditorPanel
	{
		eastl::string m_currentFilePath;

		ResourceHandle m_emptyFolderIcon;
		ResourceHandle m_folderIcon;

		ResourceHandle m_textIcon;
		ResourceHandle m_luaIcon;
		ResourceHandle m_prefabIcon;
		ResourceHandle m_sceneIcon;
		ResourceHandle m_audioIcon;
		ResourceHandle m_imageIcon;
		ResourceHandle m_fontIcon;
		ResourceHandle m_zeroSizeIcon;
		ResourceHandle m_unknownFileTypeIcon;

	public:
		AssetPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene);

		virtual void UpdatePanel() final override;

		virtual void OnImGuiRender() final override;
	};
}