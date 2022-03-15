#include "EXEPCH.h"
#include "source/engine/gameobjectsystem/components/ExeliusComponentFactory.h"
#include "source/engine/gameobjectsystem/GameObjectSystem.h"

//#include "source/engine/gameobjectsystem/components/componenttypes/TransformComponent.h"
//#include "source/engine/gameobjectsystem/components/componenttypes/SpriteComponent.h"
//#include "source/engine/gameobjectsystem/components/componenttypes/UIComponent.h"
//#include "source/engine/gameobjectsystem/components/componenttypes/AudioComponent.h"
//#include "source/engine/gameobjectsystem/components/componenttypes/TilemapComponent.h"
//#include "source/engine/gameobjectsystem/components/componenttypes/AnimationComponent.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Constructor initialized the log.
	/// </summary>
	ExeliusComponentFactory::ExeliusComponentFactory()
	{
	}

	bool ExeliusComponentFactory::Initialize()
	{
		auto* pGameObjectSystem = GameObjectSystem::GetInstance();

		// Register the Engine component types.
		//pGameObjectSystem->RegisterComponent<TransformComponent>(TransformComponent::kType, false, false);
		//pGameObjectSystem->RegisterComponent<SpriteComponent>(SpriteComponent::kType, false, true);
		//pGameObjectSystem->RegisterComponent<UIComponent>(UIComponent::kType, true, true);
		//pGameObjectSystem->RegisterComponent<AudioComponent>(AudioComponent::kType, false, false);
		//pGameObjectSystem->RegisterComponent<TilemapComponent>(TilemapComponent::kType, true, true);
		//pGameObjectSystem->RegisterComponent<AnimationComponent>(AnimationComponent::kType, true, true);

		return true;
	}

	//Todo fix naming of variables
	Handle ExeliusComponentFactory::CreateComponent(const Component::Type& componentName, GameObject* pOwningObject, const rapidjson::Value& componentData)
	{
		auto* pGameObjectSystem = GameObjectSystem::GetInstance();
		EXE_ASSERT(pGameObjectSystem);

		Handle newHandle;
		bool initSucceeded = false;

		//if (componentName == TransformComponent::kType)
		//{
		//	newHandle = pGameObjectSystem->CreateComponent<TransformComponent>(pOwningObject);

		//	if (!newHandle.IsValid())
		//	{
		//		m_gameObjectSystemLog.Error("TransformComponent failed to be created.");
		//		pGameObjectSystem->ReleaseComponent(componentName, newHandle);
		//		return {}; // Invalid.
		//	}

		//	auto& newComponent = pGameObjectSystem->GetComponent<TransformComponent>(newHandle);
		//	initSucceeded = newComponent.Initialize(componentData);
		//}
		//else if (componentName == SpriteComponent::kType)
		//{
		//	newHandle = pGameObjectSystem->CreateComponent<SpriteComponent>(pOwningObject);

		//	if (!newHandle.IsValid())
		//	{
		//		m_gameObjectSystemLog.Error("SpriteComponent failed to be created.");
		//		pGameObjectSystem->ReleaseComponent(componentName, newHandle);
		//		return {}; // Invalid.
		//	}

		//	auto& newComponent = pGameObjectSystem->GetComponent<SpriteComponent>(newHandle);
		//	initSucceeded = newComponent.Initialize(componentData);
		//}
		//else if (componentName == UIComponent::kType)
		//{
		//	newHandle = pGameObjectSystem->CreateComponent<UIComponent>(pOwningObject);

		//	if (!newHandle.IsValid())
		//	{
		//		m_gameObjectSystemLog.Error("UIComponent failed to be created.");
		//		pGameObjectSystem->ReleaseComponent(componentName, newHandle);
		//		return {}; // Invalid.
		//	}

		//	auto& newComponent = pGameObjectSystem->GetComponent<UIComponent>(newHandle);
		//	initSucceeded = newComponent.Initialize(componentData);
		//}
		//else if (componentName == AudioComponent::kType)
		//{
		//	newHandle = pGameObjectSystem->CreateComponent<AudioComponent>(pOwningObject);

		//	if (!newHandle.IsValid())
		//	{
		//		m_gameObjectSystemLog.Error("AudioComponent failed to be created.");
		//		pGameObjectSystem->ReleaseComponent(componentName, newHandle);
		//		return {}; // Invalid.
		//	}

		//	auto& newComponent = pGameObjectSystem->GetComponent<AudioComponent>(newHandle);
		//	initSucceeded = newComponent.Initialize(componentData);
		//}
		//else if (componentName == TilemapComponent::kType)
		//{
		//	newHandle = pGameObjectSystem->CreateComponent<TilemapComponent>(pOwningObject);

		//	if (!newHandle.IsValid())
		//	{
		//		m_gameObjectSystemLog.Error("TilemapComponent failed to be created.");
		//		pGameObjectSystem->ReleaseComponent(componentName, newHandle);
		//		return {}; // Invalid.
		//	}

		//	auto& newComponent = pGameObjectSystem->GetComponent<TilemapComponent>(newHandle);
		//	initSucceeded = newComponent.Initialize(componentData);
		//}
		//else if (componentName == AnimationComponent::kType)
		//{
		//	newHandle = pGameObjectSystem->CreateComponent<AnimationComponent>(pOwningObject);

		//	if (!newHandle.IsValid())
		//	{
		//		m_gameObjectSystemLog.Error("AnimationComponent failed to be created.");
		//		pGameObjectSystem->ReleaseComponent(componentName, newHandle);
		//		return {}; // Invalid.
		//	}

		//	auto& newComponent = pGameObjectSystem->GetComponent<AnimationComponent>(newHandle);
		//	initSucceeded = newComponent.Initialize(componentData);
		//}
		//else
		//{
		//	m_gameObjectSystemLog.Error("Component failed to initialize: Component Type '{}' Does Not Exist.", componentName);
		//	return {}; // Invalid.
		//}

		if (!initSucceeded)
		{
			m_gameObjectSystemLog.Error("Component failed to initialize.");
			pGameObjectSystem->ReleaseComponent(componentName, newHandle);
			return {}; // Invalid.
		}

		m_gameObjectSystemLog.Trace("Completed Component Creation.");
		return newHandle;
	}
}