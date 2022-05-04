#pragma once
#include <include/Exelius.h>

#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class AssetPanel
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
		AssetPanel();

		void OnImGuiRender(const SharedPtr<Scene>& pActiveScene);
	};
}