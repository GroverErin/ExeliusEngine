#pragma once
#include "EditorPanel.h"

#include <include/Exelius.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class InspectorPanel
		: public EditorPanel
	{
	public:
		InspectorPanel(EditorLayer* pEditorLayer, const SharedPtr<Scene>& pActiveScene);

		virtual void OnImGuiRender() final override;

	private:
		void DrawComponents(GameObject gameObject);

		void DrawAddComponentPanel(GameObject gameObject);

		void DrawVector3(const eastl::string& label, glm::vec3& vectorToDraw, float columnWidth = 100.0f);
	};
}