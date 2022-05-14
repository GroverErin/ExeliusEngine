#pragma once
#include "editorapplication/EditorHelpers.h"

#include <include/Exelius.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class EditorLayer;

	class EditorPanel
	{
	protected:
		EditorLayer* m_pEditorLayer;
		const SharedPtr<Scene>& m_pActiveScene;

		const char* m_pPanelName;
		const char* m_pPanelHotkey;

		bool m_isPanelHovered;
		bool m_isPanelSelected;
		bool m_isPanelOpen;
	public:
		EditorPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene, const char* pPanelName = "Unnamed Panel", bool isOpen = true, const char* pPanelHotkey = "")
			: m_pEditorLayer(pEditorLayer)
			, m_pActiveScene(pActiveScene)
			, m_pPanelName(pPanelName)
			, m_pPanelHotkey(pPanelHotkey)
			, m_isPanelHovered(false)
			, m_isPanelSelected(false)
			, m_isPanelOpen(isOpen)
		{
			EXE_ASSERT(m_pEditorLayer);
			EXE_ASSERT(m_pActiveScene);
		}

		virtual ~EditorPanel() = default;

		virtual void InitializePanel() {}
		virtual void UpdatePanel() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event&) {}

		const char* GetPanelName() const { return m_pPanelName; }
		void SetPanelName(const char* newPanelName) { m_pPanelName = newPanelName; }

		const char* GetPanelHotkey() const { return m_pPanelHotkey; }
		void SetPanelHotkey(const char* newPanelHotkey) { m_pPanelHotkey = newPanelHotkey; }

		bool IsPanelHovered() const { return m_isPanelHovered; }
		bool IsPanelSelected() const { return m_isPanelSelected; }

		bool IsPanelOpen() const { return m_isPanelOpen; }
		void SetPanelOpen(bool isPanelOpen = true) { m_isPanelOpen = isPanelOpen; }
	};
}