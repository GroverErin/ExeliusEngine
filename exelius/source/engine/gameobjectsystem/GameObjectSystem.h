#pragma once
#include "source/utility/generic/Singleton.h"
#include "source/resource/ResourceHelpers.h"
#include "source/engine/gameobjectsystem/GameObjectHelpers.h"

#include "source/engine/gameobjectsystem/components/ComponentList.h"
#include "source/engine/gameobjectsystem/components/Component.h"

#include "source/debug/Log.h"

#include <EASTL/deque.h>
#include <EASTL/unordered_map.h>
#include <EASTL/shared_ptr.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class GameObject;
	class ComponentFactory;

	/// <summary>
	/// The Game Object System manages the lifetime,
	/// identification, retrieval, and components of
	/// any instantiated GameObjects.
	/// 
	/// This systems is designed to be an inheretence-ECS
	/// hybrid, where the components are stored and updated
	/// in "systems" allowing for cache friendly updating
	/// and/or rendering. GameObjects retain a list of
	/// their components, so retrieval of components is
	/// straightforward.
	/// 
	/// @todo
	/// Consider what the real benfits are of this system when
	/// compared to traditional ECS or inheretence models.
	/// </summary>
	class GameObjectSystem
		: public Singleton<GameObjectSystem>
	{
		/// <summary>
		/// Log for the GameObjectSystem.
		/// </summary>
		Log m_gameObjectSystemLog;

		/// <summary>
		/// This value contains the ID for the next object to be created.
		/// </summary>
		GameObjectID m_nextObjectID;

		/// <summary>
		/// List of all the free object ID's. ID's are recycled.
		/// @todo
		/// Consider if this is the best container here.
		/// </summary>
		eastl::deque<GameObjectID> m_freeObjectIDs;

		/// <summary>
		/// The component factory as defined by either the Engine or the Client.
		/// @see Application
		/// </summary>
		ComponentFactory* m_pComponentFactory;

		/// <summary>
		/// Contains all lists of components, keyed by component type.
		/// This method allows the components to be separated by type,
		/// and 'ticked' in a cache-friendly way.
		/// </summary>
		eastl::unordered_map<Component::Type, ComponentListBase*> m_componentLists;

		/// <summary>
		/// Contains all gameobjects, keyed by object ID. GameObjects
		/// *shouldn't* need to be iterated over, but accessed by ID
		/// is likely to be common.
		/// </summary>
		eastl::unordered_map<GameObjectID, eastl::shared_ptr<GameObject>> m_gameObjects;

	public:
		/// <summary>
		/// Constructor - initializes member values.
		/// </summary>
		GameObjectSystem();
		GameObjectSystem(const GameObjectSystem&) = delete;
		GameObjectSystem(GameObjectSystem&&) = delete;
		GameObjectSystem& operator=(const GameObjectSystem&) = delete;

		/// <summary>
		/// Destructor - Destroys all Components and Gameobjects.
		/// </summary>
		virtual ~GameObjectSystem();

		/// <summary>
		/// Sets the component factory to use when creating components.
		/// The user defined component factory should be passed into this
		/// function to allow the system to create user components.
		/// 
		/// Calls the ComponentFactory Initialize function, defined by either
		/// the engine or the client.
		/// </summary>
		/// <param name="pComponentFactor">Component factory to use. A factory MUST be set.</param>
		/// <returns>True if initialization succeeded, false otherwise.</returns>
		bool Initialize(ComponentFactory* pComponentFactor);

		/// <summary>
		/// Creates a GameObject from a ResourceID.
		/// The resources used to create a GameObject
		/// may or may not be loaded currently, and
		/// the method for loading not/un-loaded
		/// resources is determined by the given
		/// CreationMode.
		/// 
		/// By default, if the resources for a GameObject
		/// is not loaded, then the resources will be
		/// queued and the loading thread will be signaled. 
		/// 
		/// @todo
		/// There is an issue presented here, if the given resource
		/// is currently loading then it will fail this function.
		/// Currently triggering an assertion.
		/// </summary>
		/// <param name="pResource">- The ResourceID referring to a JSON file containing object data.</param>
		/// <param name="createMode">- How the GameObjectSystem should handle object creation if the resource isn't loaded.</param>
		/// <returns>
		/// ID of the created object.
		/// ID will be equal to kInvalidGameObjectID upon failure.
		/// </returns>
		GameObjectID CreateGameObject(const ResourceID& resourceID, CreationMode createMode = CreationMode::kQueueAndSignal);

		/// <summary>
		/// Gets the GameObject with the given ID.
		/// 
		/// TODO:
		/// The handle system used by the Components should be used here, as there can be
		/// some invalidation of GameObjectID's that would go unchecked otherwise.
		/// </summary>
		/// <param name="objectId">GameObjectID for an object to be retrieved.</param>
		/// <returns>Pointer to a GameObject, nullptr if GameObject not found.</returns>
		const eastl::shared_ptr<GameObject> GetGameObject(GameObjectID gameObjectID) const;

		/// <summary>
		/// Completely destroys a GameObject and 'detatches' any components.
		/// Components are NOT Completely destroyed, but pooled for reuse.
		/// Pooled components are reset when reused to create a new object.
		/// </summary>
		/// <param name="gameObjectID">GameObjectID for an object to be destroyed.</param>
		void DestroyGameObject(GameObjectID gameObjectID);

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
		Handle CreateComponentFromFactory(const Component::Type& componentType,
			GameObject* pOwningObject, const rapidjson::Value& componentData);

		/// <summary>
		/// Releases a component back into the pool.
		/// This does NOT destroy the component NOR 
		/// does the data within get reset.
		/// </summary>
		/// <param name="componentType">The type of component to be Released.</param>
		/// <param name="handle">The handle to that component in the ComponentList of it's type.</param>
		void ReleaseComponent(const Component::Type& componentType, Handle handle);

		/// <summary>
		/// Updates all *Active* components that require updating.
		/// </summary>
		void Update();

		/// <summary>
		/// Renders all *Active* components that require rendering.
		/// </summary>
		void Render();

		/// <summary>
		/// Templated CreateComponent.
		/// This is the function that should be called by a Component Factory.
		/// This function does the following:
		///		- Creates the component of the templated type.
		///		- Adds the component to the ComponentList of the matching type.
		/// 
		/// Important:
		///		- No Initialization will occur.
		///		- Components are pooled and recycled. Since no initialization occurs,
		///			it is possible that a component retrieved will have data from previous 'lifecycle'.
		/// </summary>
		/// <returns>A Handle that refers to the Component in the ComponentList.</returns>
		template <class ComponentType>
		Handle CreateComponent(GameObject* pOwningObject)
		{
			EXE_ASSERT(ComponentType::kType.IsValid());
			Log log("GameObjectSystem");

			// Check if the map contains this componenttype as a key value pair.
			auto itr = m_componentLists.find(ComponentType::kType);

			if (itr == m_componentLists.end())
			{
				log.Warn("Component '{}' creation failed: No ComponentList defined.", ComponentType::kType.Get().c_str());
				return {}; // Invalid.
			}

			ComponentList<ComponentType>* pComponentList = static_cast<ComponentList<ComponentType>*>(itr->second);
			EXE_ASSERT(pComponentList);

			return pComponentList->EmplaceComponent(pOwningObject); // Should call emplace.
		}

		/// <summary>
		/// Gets the Component of the templated type with the given Handle.
		/// </summary>
		/// <param name="internalHandle">
		/// Handle of the component to retrieve.
		/// User is responsible for verifying that the Handle is valid before passing it.
		/// </param>
		/// <returns>A reference to the Component.</returns>
		template <class ComponentType>
		ComponentType& GetComponent(Handle internalHandle)
		{
			EXE_ASSERT(ComponentType::kType.IsValid());
			EXE_ASSERT(internalHandle.IsValid());

			// Look for the component with the type specified.
			auto found = m_componentLists.find(ComponentType::kType);
			EXE_ASSERT(found != m_componentLists.end());

			auto* pComponentList = m_componentLists[ComponentType::kType];
			EXE_ASSERT(pComponentList);

			// Cast to proper component type and get.
			return static_cast<ComponentList<ComponentType>*>(pComponentList)->GetComponent(internalHandle);
		}

		/// <summary>
		/// Checks to see if the Component of the templated type
		/// reffered to by the given Handle is valid.
		/// </summary>
		/// <param name="internalHandle">
		/// Handle of the component to check.
		/// User is responsible for verifying that the Handle is valid before passing it.
		/// </param>
		/// <returns>True the Component is valid, false if not.</returns>
		template <class ComponentType>
		bool IsValidComponent(Handle internalHandle)
		{
			EXE_ASSERT(ComponentType::kType.IsValid());

			if (!internalHandle.IsValid())
				return false;

			// Look for the component with the type specified.
			auto found = m_componentLists.find(ComponentType::kType);
			if (found == m_componentLists.end())
				return false;

			auto* pComponentList = m_componentLists[ComponentType::kType];

			if (!pComponentList)
				return false;

			// Cast to proper component type and check.
			return static_cast<ComponentList<ComponentType>*>(pComponentList)->IsValidComponent(internalHandle);
		}

		/// <summary>
		/// Registers the component to be creatable by the GameObjectSystem. Components that
		/// are not registered will NOT be Created, Rendered, or Updated if they have not
		/// registered.
		/// 
		/// This function should be called as soon as possible for each Component.
		/// Ideally it should be in the Initialize function of the ComponentFactory used.
		/// (Prior to attempting to create an object with each component!)
		///	
		/// TransformComponent Example:
		///		// In an early Initialization function. (replace the '{' '}' with the template angle brackets, documentation issue...)
		///		GameObjectSystem::GetInstance()->RegisterComponent{TransformComponent}(TransformComponent::kType, false, false);
		/// 
		/// NOTE:
		///		componentType must equal the Component kType value. Users should verify that this is true when defining new components.
		/// Example:
		///		assert(TransformComponent::kType = "TransformComponent") == (componentType = "TransformComponent");
		/// </summary>
		/// <param name="componentType">The Registered component Type. This is the Key for component lookups.</param>
		/// <param name="isUpdated">The type of component to be Released.</param>
		/// <param name="isRendered">The type of component to be Released.</param>
		template <class ComponentType>
		void RegisterComponent(const Component::Type& componentType, bool isUpdated = false, bool isRendered = false)
		{
			// ALL of the MUST be true.
			EXE_ASSERT(ComponentType::kType.IsValid());
			EXE_ASSERT(componentType.IsValid());
			EXE_ASSERT(componentType == ComponentType::kType);
			Log log("GameObjectSystem");

			auto found = m_componentLists.find(componentType);

			if (found == m_componentLists.end())
			{
				m_componentLists.try_emplace(componentType, new ComponentList<ComponentType>(isUpdated, isRendered));
			}
			else
			{
				log.Warn("ComponentList of type '{}' is already registered.", componentType.Get().c_str());
			}
		}
	private:

		/// <summary>
		/// Checks for a free GameObjectID.
		/// If one is not found, the m_nextObjectId is returned THEN incremented.
		/// </summary>
		/// <returns>The available GameObjectID for a new GameObject.</returns>
		GameObjectID GetNextObjectId();
	};
}