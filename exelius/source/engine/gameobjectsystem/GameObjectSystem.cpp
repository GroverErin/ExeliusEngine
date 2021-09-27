#include "EXEPCH.h"
#include "source/engine/gameobjectsystem/GameObjectSystem.h"
#include "source/engine/gameobjectsystem/components/ComponentFactory.h"
#include "source/engine/gameobjectsystem/GameObject.h"
#include "source/resource/ResourceLoader.h"
#include "source/engine/resources/resourcetypes/TextFileResource.h"

#include "source/resource/ResourceHandle.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
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
	/// Destructor - Destroys all Components and GameObjects.
	/// </summary>
	GameObjectSystem::~GameObjectSystem()
	{
		// Don't delete, this lives on the Application/Engine.
		// I have decided that the destruction of the factory
		// makes more sense to happen in the same class that
		// created it. TODO: Consider making it a smart ptr.
		m_pComponentFactory = nullptr;

		for (auto& gameObjectPair : m_gameObjects)
		{
			gameObjectPair.second->RemoveComponents();
		}

		m_gameObjects.clear();

		for (auto& componentListPair : m_componentLists)
		{
			delete componentListPair.second;
			componentListPair.second = nullptr;
		}

		m_componentLists.clear();
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
	/// Creates a GameObject from a ResourceID.
	/// This resource may not yet be loaded. If not, then
	/// forceLoad must be set to true in order to successfully
	/// create the GameObject.
	/// 
	/// @todo
	/// There is an issue presented here, if the given resource
	/// is currently loading then it will fail this function.
	/// Currently triggering an assertion.
	/// </summary>
	/// <param name="pResource">ResourceID referring to a JSON file containing object data.</param>
	/// <returns>
	/// ID of the created object.
	/// ID will be equal to kInvalidGameObjectID upon failure.
	/// </returns>
	GameObjectID GameObjectSystem::CreateGameObject(const ResourceID& resourceID, CreationMode createMode)
	{
		Log log("GameObjectSystem");
		EXE_ASSERT(resourceID.IsValid());

		// Increments reference count *if* resource exists.
		// Note: The resource could be currently Loading.
		ResourceHandle gameObjectData(resourceID);

		// If the resource is not loaded and we opted not to do so, then bail.
		if (!gameObjectData.IsReferenceHeld() && (createMode == CreationMode::kDoNotLoad))
		{
			log.Warn("GameObject from '{}' did not exist and will not be created.");
			return kInvalidGameObjectID;
		}

		GameObjectID id = GetNextObjectId();
		EXE_ASSERT(id != kInvalidGameObjectID);

		log.Info("Creating GameObject from '{}' with ID: {}", resourceID.Get().c_str(), id);

		// Create and Get the new object.
		m_gameObjects.try_emplace(id, eastl::make_shared<GameObject>(id, createMode));
		auto& pNewObject = m_gameObjects.at(id);
		EXE_ASSERT(pNewObject);

		// If the resource was already loaded, then notify the gameobject.
		if (gameObjectData.IsReferenceHeld())
		{
			log.Info("GameObject resource has been loaded.");
			// The Object resource is already loaded. So, we need to return the new id and
			// tell the new game object that it's resource has loaded. We can call
			// OnResourceLoaded. We know it won't be unloaded here because
			// we have incremented the ref count already.
			pNewObject->OnResourceLoaded(resourceID);
			return id;
		}

		switch (createMode)
		{
		case CreationMode::kLoadImmediate:
			log.Info("GameObject resource loading on Main thread.");
			gameObjectData.LoadNow(pNewObject);
			break;
		case CreationMode::kQueueAndSignal:
			log.Info("GameObject resource queueing on resource thread. Thread signaled.");
			gameObjectData.QueueLoad(true, pNewObject);
			break;
		case CreationMode::kQueueNoSignal:
			log.Info("GameObject resource queueing on resource thread.");
			gameObjectData.QueueLoad(false, pNewObject);
			break;
		default:
			log.Fatal("GameObject creation failed.");
			EXE_ASSERT(false);
			break;
		}

		// We lock here because this handle will fall out of scope
		// and that will leave the resource without any references
		// while it is potentially queued to load. GameObject
		// should unlock the resource in the OnResourceLoaded call.
		gameObjectData.LockResource();

		// Here, if this object does not hold a reference
		// then that means that the resource was currently
		// loading, thus failing to acquire the resource,
		// but also failing to call OnResourceLoaded for
		// this particular GameObject.
		// TODO: I am not yet sure how to handle this case.
		EXE_ASSERT(gameObjectData.IsReferenceHeld());

		// Due to the note above about locking the resource,
		// at this point the resource will be unlocked again
		// and the resource will fall out of scope, thus
		// causing it to be unloaded, as expected after
		// the gameobject has recieved its notification.
		return id;
	}

	/// <summary>
	/// Gets the GameObject with the given ID.
	/// 
	/// TODO:
	/// The handle system used by the Components should be used here, as there can be
	/// some invalidation of GameObjectID's that would go unchecked otherwise.
	/// </summary>
	/// <param name="objectId">GameObjectID for an object to be retrieved.</param>
	/// <returns>Pointer to a GameObject, nullptr if GameObject not found.</returns>
	const eastl::shared_ptr<GameObject> GameObjectSystem::GetGameObject(GameObjectID gameObjectID) const
	{
		Log log("GameObjectSystem");

		if (gameObjectID == kInvalidGameObjectID)
		{
			log.Warn("GameObjectID is invalid.");
			return nullptr;
		}

		// Look for GameObject in list with ID.
		auto found = m_gameObjects.find(gameObjectID);
		if (found == m_gameObjects.end())
		{
			log.Warn("GameObject with ID '{}' does not exist.", gameObjectID);
			return nullptr;
		}

		// This GameObject MUST exist.
		auto& pGameObject = found->second;
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
		Log log("GameObjectSystem");

		if (gameObjectID == kInvalidGameObjectID)
		{
			log.Warn("GameObjectID is invalid.");
			return;
		}

		// Look for gameobject with given ID.
		auto found = m_gameObjects.find(gameObjectID);

		if (found == m_gameObjects.end())
		{
			log.Warn("GameObject with ID '{}' does not exist.", gameObjectID);
			return;
		}

		// This GameObject MUST exist.
		auto& pGameObject = found->second;
		EXE_ASSERT(pGameObject);

		pGameObject->RemoveComponents();

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
		Log log("GameObjectSystem");

		if (!pOwningObject)
		{
			log.Error("Owning GameObject was nullptr.");
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
	GameObjectID GameObjectSystem::GetNextObjectId()
	{
		// Make a new Id if not available in the free list.
		if (m_freeObjectIDs.empty())
			return m_nextObjectID++;

		// Otherwise get the next free id.
		GameObjectID id = m_freeObjectIDs.front();
		m_freeObjectIDs.pop_front();

		EXE_ASSERT(id != kInvalidGameObjectID);
		return id;
	}
}