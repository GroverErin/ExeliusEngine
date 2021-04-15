#include "EXEPCH.h"
#include "Source/Engine/Game/GameObjectSystem/Components/ExeliusComponentFactory.h"
#include "Source/Engine/Game/GameObjectSystem/GameObjectSystem.h"

#include "Source/Engine/Game/GameObjectSystem/Components/ComponentTypes/TransformComponent.h"

namespace Exelius
{
	namespace ExeliusComponents
	{
		using Type = eastl::string;

		inline static const Type kTransformComponent = "TransformComponent";
	}

	bool ExeliusComponentFactory::Initialize()
	{
		auto* pGameObjectSystem = GameObjectSystem::GetInstance();

		// Register the Engine component types.
		pGameObjectSystem->RegisterComponent<TransformComponent>(TransformComponent::kType, false, false);

		return true;
	}

	Handle ExeliusComponentFactory::CreateComponent(const Component::Type& componentName, GameObject* pOwningObject, const rapidjson::Value& componentData)
	{
		auto* pGameObjectSystem = GameObjectSystem::GetInstance();
		EXE_ASSERT(pGameObjectSystem);
		EXE_ASSERT(componentName.IsValid());

		Handle newHandle;
		bool initSucceeded = false;

		if (componentName == ExeliusComponents::kTransformComponent)
		{
			newHandle = pGameObjectSystem->CreateComponent<TransformComponent>();

			if (!newHandle.IsValid())
			{
				EXELOG_ENGINE_ERROR("{}: Component failed to be created.", componentName.Get().c_str());
				return {}; // Invalid.
			}

			auto& newComponent = pGameObjectSystem->GetComponent<TransformComponent>(newHandle);
			initSucceeded = newComponent.Initialize(pOwningObject, componentData);
		}

		if (!initSucceeded)
		{
			EXELOG_ENGINE_ERROR("{}: Component failed to initialize.", componentName.Get().c_str());
			// Should I return invalid here?
		}

		return newHandle;
	}
}