#include "EXEPCH.h"
#include "Source/Engine/Game/GameObjectSystem/Components/ExeliusComponentFactory.h"
#include "Source/Engine/Game/GameObjectSystem/GameObjectSystem.h"

#include "Source/Engine/Game/GameObjectSystem/Components/ComponentTypes/TransformComponent.h"
#include "Source/Engine/Game/GameObjectSystem/Components/ComponentTypes/SpriteComponent.h"

namespace Exelius
{
	bool ExeliusComponentFactory::Initialize()
	{
		auto* pGameObjectSystem = GameObjectSystem::GetInstance();

		// Register the Engine component types.
		pGameObjectSystem->RegisterComponent<TransformComponent>(TransformComponent::kType, false, false);
		pGameObjectSystem->RegisterComponent<SpriteComponent>(SpriteComponent::kType, false, true);

		return true;
	}

	Handle ExeliusComponentFactory::CreateComponent(const Component::Type& componentName, GameObject* pOwningObject, const rapidjson::Value& componentData)
	{
		auto* pGameObjectSystem = GameObjectSystem::GetInstance();
		EXE_ASSERT(pGameObjectSystem);
		EXE_ASSERT(componentName.IsValid());
		EXELOG_ENGINE_TRACE("Attempting to create Component: {}", componentName.Get().c_str());

		Handle newHandle;
		bool initSucceeded = false;

		if (componentName == TransformComponent::kType)
		{
			newHandle = pGameObjectSystem->CreateComponent<TransformComponent>();

			if (!newHandle.IsValid())
			{
				EXELOG_ENGINE_ERROR("{}: Component failed to be created.", componentName.Get().c_str());
				pGameObjectSystem->ReleaseComponent(componentName, newHandle);
				return {}; // Invalid.
			}

			auto& newComponent = pGameObjectSystem->GetComponent<TransformComponent>(newHandle);
			initSucceeded = newComponent.Initialize(pOwningObject, componentData);
		}
		else if (componentName == SpriteComponent::kType)
		{
			newHandle = pGameObjectSystem->CreateComponent<SpriteComponent>();

			if (!newHandle.IsValid())
			{
				EXELOG_ENGINE_ERROR("{}: Component failed to be created.", componentName.Get().c_str());
				pGameObjectSystem->ReleaseComponent(componentName, newHandle);
				return {}; // Invalid.
			}

			auto& newComponent = pGameObjectSystem->GetComponent<SpriteComponent>(newHandle);
			initSucceeded = newComponent.Initialize(pOwningObject, componentData);
		}

		if (!initSucceeded)
		{
			EXELOG_ENGINE_ERROR("{}: Component failed to initialize.", componentName.Get().c_str());
			pGameObjectSystem->ReleaseComponent(componentName, newHandle);
			return {}; // Invalid.
		}

		EXELOG_ENGINE_TRACE("Completed Component Creation.");
		return newHandle;
	}
}