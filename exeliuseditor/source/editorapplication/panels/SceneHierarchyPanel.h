#pragma once
#include <include/Exelius.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class SceneHierarchyPanel
	{
		SharedPtr<Scene> m_pSceneContext;
		GameObject m_selectedGameObject;
	public:
		SceneHierarchyPanel();

		void SetContext(const SharedPtr<Scene>& scene);

		void OnImGuiRender();

		GameObject GetSelectedGameObject() const { return m_selectedGameObject; }
		void SetSelectedGameObject(GameObject gameObject);
	private:
		void DrawGameObjectNodeTree(GameObject gameObject);
	};
}