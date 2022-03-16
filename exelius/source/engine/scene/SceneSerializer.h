#pragma once
#include "Scene.h"
#include "source/utility/generic/SmartPointers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class SceneSerializer
	{
		SharedPtr<Scene> m_pScene;
	public:
		SceneSerializer(const SharedPtr<Scene>& pScene);

		void Serialize(const eastl::string& filepath);
		void SerializeRuntime(const eastl::string& filepath);

		bool Deserialize(const eastl::string& filepath);
		bool DeserializeRuntime(const eastl::string& filepath);
	};
}