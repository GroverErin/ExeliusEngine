#include "EXEPCH.h"
#include "Source/Engine/Game/GameObjectSystem/GameObjectSystem.h"
#include "Source/Engine/Game/GameObjectSystem/GameObject.h"

#include "Source/Engine/Resources/ResourceRetrieval.h"

namespace Exelius
{
	/// <summary>
	/// Constructor - initializes member values.
	/// </summary>
	GameObjectSystem::GameObjectSystem()
		: m_nextObjectID(0)
		, m_pComponentFactory(nullptr)
	{
		//
	}

	/// <summary>
	/// Sets the component factory to use when creating components.
	/// The user defined component factory should be passed into this
	/// function to allow the system to create user components.
	/// </summary>
	/// <param name="pComponentFactor">Component factory to use. A factory MUST be set.</param>
	/// <returns>True if Initialization succeeded, false if failed.</returns>
	bool GameObjectSystem::Initialize(ComponentFactory* pComponentFactory)
	{
		// A component factory MUST be set.
		EXE_ASSERT(pComponentFactory);
		m_pComponentFactory = pComponentFactory;

		// The initialize function should register all of the components.
		return m_pComponentFactory->Initialize();
	}

	/// <summary>
	/// Creates an empty GameObject. This Object will be given a default name
	/// and will have no components.
	/// </summary>
	/// <returns>
	/// ID of the created object.
	/// ID will be equal to 'eastl::numeric_limits(uint32_t)::max()' upon failure.
	/// </returns>
	GameObjectSystem::GameObjectID GameObjectSystem::CreateGameObject()
	{
		// Get a free ID. Add new object to the list.
		GameObjectID id = GetNextObjectId();
		m_gameObjects.try_emplace(id, new GameObject(id));

		// Get newly created object.
		auto* pNewObject = m_gameObjects.at(id);
		EXE_ASSERT(pNewObject);

		// Empty intialize the object.
		// This means that no JSON file was used to attach components to this object.
		// This is hard coded not to fail, as it is empty.
		EXE_ASSERT(pNewObject->Initialize());

		return id;
	}

	/// <summary>
	/// Creates a GameObject from a resource that has already been aquired by the ResourceManager.
	/// </summary>
	/// <param name="pResource">JSON File resource containing object data.</param>
	/// <returns>
	/// ID of the created object.
	/// ID will be equal to 'eastl::numeric_limits(uint32_t)::max()' upon failure.
	/// </returns>
	GameObjectSystem::GameObjectID GameObjectSystem::CreateGameObject(TextFileResource* pResource)
	{
		// If the resource passed in was null, then the resource is either not loaded or invalid.
		if (!pResource)
		{
			EXELOG_ENGINE_ERROR("Failed to create GameObject: Resource was nullptr.");
			return kInvalidID;
		}

		// Get a free ID. Add new object to the list.
		GameObjectID id = GetNextObjectId();
		m_gameObjects.try_emplace(id, new GameObject(id));

		// Get newly created object.
		auto* pNewObject = m_gameObjects.at(id);
		EXE_ASSERT(pNewObject);

		if (!pNewObject->Initialize(pResource))
		{
			EXELOG_ENGINE_ERROR("Failed to initialize GameObject from '{}'", pResource->GetResourceID().Get().c_str());
			return kInvalidID;
		}
		return id;
	}

	/// <summary>
	/// Creates a GameObject from a ResourceID.
	/// This resource may not yet be loaded. If not, then
	/// forceLoad must be set to true in order to successfully
	/// create the GameObject.
	/// </summary>
	/// <param name="pResource">ResourceID referring to a JSON file containing object data.</param>
	/// <returns>
	/// ID of the created object.
	/// ID will be equal to 'eastl::numeric_limits(uint32_t)::max()' upon failure.
	/// </returns>
	GameObjectSystem::GameObjectID GameObjectSystem::CreateGameObject(const ResourceID& resourceID, bool forceLoad)
	{
		auto* pResourceManager = ResourceManager::GetInstance();
		EXE_ASSERT(pResourceManager);

		if (!pResourceManager->IsResourceLoaded(resourceID))
			return kInvalidID;

		return CreateGameObject(GetResourceAs<TextFileResource>(resourceID, false));
	}

	GameObject* GameObjectSystem::GetGameObject(GameObjectID objectId)
	{
		auto* pGameObject = m_gameObjects[objectId];

		if (pGameObject)
			return pGameObject;

		EXELOG_ENGINE_WARN("GameObject with ID '{}' does not exist.", objectId);
		return nullptr;
	}

	void GameObjectSystem::ReleaseComponent(const Component::Type& componentType, Handle handle)
	{
		auto found = m_componentLists.find(componentType);

		if (found != m_componentLists.end())
		{
			found->second->DestroyComponent(handle);
		}
	}

	GameObjectSystem::GameObjectID GameObjectSystem::GetNextObjectId()
	{
		if (m_freeObjectIDs.empty())
			return m_nextObjectID++;

		GameObjectID id = m_freeObjectIDs.front();
		m_freeObjectIDs.pop_front();
		return id;
	}

	void GameObjectSystem::DestroyGameObject(GameObjectID gameObjectID)
	{
		auto found = m_gameObjects.find(gameObjectID);

		if (found != m_gameObjects.end())
		{
			found->second->RemoveComponents();

			delete found->second;
			found->second = nullptr;

			m_gameObjects.erase(gameObjectID);
			m_freeObjectIDs.push_back(gameObjectID);
		}
	}

	void GameObjectSystem::Update()
	{
		for (auto& componentPair : m_componentLists)
		{
			componentPair.second->UpdateComponents();
		}
	}

	void GameObjectSystem::Render()
	{
		for (auto& componentPair : m_componentLists)
		{
			componentPair.second->RenderComponents();
		}
	}

	Handle GameObjectSystem::CreateComponentFromFactory(const Component::Type& componentType, GameObject* pOwningObject, const rapidjson::Value& componentData)
	{
		EXE_ASSERT(m_pComponentFactory);

		return m_pComponentFactory->CreateComponent(componentType, pOwningObject, componentData);
	}
}