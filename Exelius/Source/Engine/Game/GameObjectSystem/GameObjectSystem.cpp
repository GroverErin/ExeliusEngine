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
		EXE_ASSERT(id == kInvalidID);

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
		EXE_ASSERT(id == kInvalidID);

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
		EXE_ASSERT(resourceID.IsValid());

		bool isLoaded = pResourceManager->IsResourceLoaded(resourceID);

		// If the resource is not loaded and we aren't forcing it to load, then bail.
		if (!isLoaded && !forceLoad)
			return kInvalidID;
		else
			return CreateGameObject(GetResourceAs<TextFileResource>(resourceID, forceLoad));
	}

	/// <summary>
	/// Gets the GameObject with the given ID.
	/// TODO: This may need to be revisited. EDIT: This may not be an issue, please test.
	/// The handle system used by the Components should be used here, as there can be
	/// some invalidation of GameObjectID's that would go unchecked otherwise.
	/// </summary>
	/// <param name="objectId">GameObjectID for an object to be retrieved.</param>
	/// <returns>Pointer to a GameObject, nullptr if GameObject not found.</returns>
	GameObject* GameObjectSystem::GetGameObject(GameObjectID gameObjectID)
	{
		if (gameObjectID == kInvalidID)
		{
			EXELOG_ENGINE_WARN("GameObjectID is invalid.");
			return nullptr;
		}

		// Look for GameObject in list with ID.
		auto found = m_gameObjects.find(gameObjectID);
		if (found == m_gameObjects.end())
		{
			EXELOG_ENGINE_WARN("GameObject with ID '{}' does not exist.", gameObjectID);
			return nullptr;
		}

		// This GameObject MUST exist.
		auto* pGameObject = found->second;
		EXE_ASSERT(pGameObject);
		return pGameObject;
	}

	/// <summary>
	/// Completely destroys a GameObject and 'detatches' any components.
	/// Components are NOT Completely destroyed, but pooled for reuse.
	/// Pooled components are reset when reused to create a new object.
	/// </summary>
	/// <param name="gameObjectID">GameObjectID for an object to be destroyed.</param>
	void GameObjectSystem::DestroyGameObject(GameObjectID gameObjectID)
	{
		if (gameObjectID == kInvalidID)
		{
			EXELOG_ENGINE_WARN("GameObjectID is invalid.");
			return;
		}

		// Look for gameobject with given ID.
		auto found = m_gameObjects.find(gameObjectID);

		if (found == m_gameObjects.end())
		{
			EXELOG_ENGINE_WARN("GameObject with ID '{}' does not exist.", gameObjectID);
		}

		// This GameObject MUST exist.
		auto* pGameObject = found->second;
		EXE_ASSERT(pGameObject);

		pGameObject->RemoveComponents();

		delete pGameObject;
		pGameObject = nullptr;

		m_gameObjects.erase(gameObjectID);
		m_freeObjectIDs.push_back(gameObjectID);
	}

	/// <summary>
	/// Requests that the Component of the given type be created
	/// from the factory used by this GameObjectSystem.
	/// Adds the component to the ComponentList of the matching type.
	/// Attaches the GameObject as the owner of the Component.
	/// Initializes the component using the JSON data.
	/// </summary>
	/// <param name="componentType">
	/// Component Type to create.
	/// This should be a Text value used by the Component Factory to determine type.
	/// </param>
	/// <param name="pOwningObject">
	/// The GameObject that will be the 'Owner' of this component.
	/// Used to retrieve the GameObject from the Component.
	/// </param>
	/// <param name="componentData">
	/// The rapidjson value data retrieved from a .json file with Object/Component data for Initialization.
	/// </param>
	/// <returns>A Handle that refers to the Component in the ComponentList.</returns>
	Handle GameObjectSystem::CreateComponentFromFactory(const Component::Type& componentType, GameObject* pOwningObject, const rapidjson::Value& componentData)
	{
		EXE_ASSERT(m_pComponentFactory);
		EXE_ASSERT(componentType.IsValid());

		if (!pOwningObject)
		{
			EXELOG_ENGINE_ERROR("Owning GameObject was nullptr.");
			return {}; // Invalid.
		}

		// Defined component factory will create the desired component.
		return m_pComponentFactory->CreateComponent(componentType, pOwningObject, componentData);
	}

	/// <summary>
	/// Releases a component back into the pool.
	/// This does NOT destroy the component NOR 
	/// does the data within get reset.
	/// </summary>
	/// <param name="componentType">The type of component to be Released.</param>
	/// <param name="handle">The handle to that component in the ComponentList of it's type.</param>
	void GameObjectSystem::ReleaseComponent(const Component::Type& componentType, Handle handle)
	{
		EXE_ASSERT(componentType.IsValid());
		EXE_ASSERT(handle.IsValid());

		// Look for the component with the type specified.
		auto found = m_componentLists.find(componentType);

		if (found != m_componentLists.end())
		{
			// List must release the specific component by handle.
			found->second->ReleaseComponent(handle);
		}
	}

	/// <summary>
	/// Updates all *Active* components that require updating.
	/// </summary>
	void GameObjectSystem::Update()
	{
		for (auto& componentPair : m_componentLists)
		{
			EXE_ASSERT(componentPair.second);
			componentPair.second->UpdateComponents();
		}
	}

	/// <summary>
	/// Renders all *Active* components that require rendering.
	/// </summary>
	void GameObjectSystem::Render()
	{
		for (auto& componentPair : m_componentLists)
		{
			EXE_ASSERT(componentPair.second);
			componentPair.second->RenderComponents();
		}
	}

	/// <summary>
	/// Checks for a free GameObjectID.
	/// If one is not found, the m_nextObjectId is returned THEN incremented.
	/// </summary>
	/// <returns>The available GameObjectID for a new GameObject.</returns>
	GameObjectSystem::GameObjectID GameObjectSystem::GetNextObjectId()
	{
		// Make a new Id if not available in the free list.
		if (m_freeObjectIDs.empty())
			return m_nextObjectID++;

		// Otherwise get the next free id.
		GameObjectID id = m_freeObjectIDs.front();
		m_freeObjectIDs.pop_front();

		EXE_ASSERT(id == kInvalidID);
		return id;
	}
}