#pragma once
#include <include/Exelius.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class InspectorPanel
	{
	public:
		void OnImGuiRender(GameObject gameObject, const SharedPtr<Scene>& scene);

	private:
		void DrawComponents(GameObject gameObject, const SharedPtr<Scene>& scene);

		void DrawAddComponentPanel(GameObject gameObject);

		void DrawVector3(const eastl::string& label, glm::vec3& vectorToDraw, float columnWidth = 100.0f);
	};
}