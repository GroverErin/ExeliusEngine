#include "EXEPCH.h"
#include "Source/Engine/GameObjectSystem/Components/ExeliusComponentFactory.h"
#include "Source/Engine/GameObjectSystem/GameObjectSystem.h"

#include "Source/Engine/GameObjectSystem/Components/ComponentTypes/TransformComponent.h"
#include "Source/Engine/GameObjectSystem/Components/ComponentTypes/SpriteComponent.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
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
		Log log("GameObjectSystem");
		log.Trace("Attempting to create Component: {}", componentName.Get().c_str());

		Handle newHandle;
		bool initSucceeded = false;

		if (componentName == TransformComponent::kType)
		{
			newHandle = pGameObjectSystem->CreateComponent<TransformComponent>();

			if (!newHandle.IsValid())
			{
				log.Error("{}: Component failed to be created.", componentName.Get().c_str());
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
				log.Error("{}: Component failed to be created.", componentName.Get().c_str());
				pGameObjectSystem->ReleaseComponent(componentName, newHandle);
				return {}; // Invalid.
			}

			auto& newComponent = pGameObjectSystem->GetComponent<SpriteComponent>(newHandle);
			initSucceeded = newComponent.Initialize(pOwningObject, componentData);
		}

		if (!initSucceeded)
		{
			log.Error("{}: Component failed to initialize.", componentName.Get().c_str());
			pGameObjectSystem->ReleaseComponent(componentName, newHandle);
			return {}; // Invalid.
		}

		log.Trace("Completed Component Creation.");
		return newHandle;
	}
}