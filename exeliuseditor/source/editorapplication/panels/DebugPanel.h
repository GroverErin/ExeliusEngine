#pragma once
#include "EditorPanel.h"

#include <include/Exelius.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class DebugPanel
		: public EditorPanel
	{
	public:
		DebugPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene);

		virtual void OnImGuiRender() final override;
	};
}