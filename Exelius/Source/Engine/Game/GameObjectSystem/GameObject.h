#pragma once
#include <EASTL/string.h>
#include <EASTL/unordered_map.h>
#include <rapidjson/document.h>

#include "Source/Resource/ResourceListener.h"
#include "Source/Engine/Game/GameObjectSystem/Components/Component.h"
#include "Source/Engine/Game/GameObjectSystem/Components/ComponentHandle.h"

namespace Exelius
{
    class TextFileResource;

    class GameObject
        : public ResourceListener
	{
    private:
        /// <summary>
        /// Unique ID of this game object.
        /// Possibly consider combining this with name?
        /// </summary>
        uint32_t m_id;

        /// <summary>
        /// Is this game object enabled?
        /// </summary>
        bool m_enabled;

        /// <summary>
        /// The user defined name of a GameObject.
        /// Possibly consider combining this with ID?
        /// </summary>
        eastl::string m_name;

        /// <summary>
        /// List of the components (Handles to) by component type.
        /// </summary>
        eastl::unordered_map<Component::Type, Handle> m_components;

    public:
        /// <summary>
        /// Constructor - Initializes member variables.
        /// TODO: This should not be publicly accessable!
        /// </summary>
        /// <param name="id">The unique object ID given by the GameObjectSystem.</param>
        GameObject(uint32_t id);
        
        /*GameObject(const GameObject&) = delete;
        GameObject(GameObject&&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject& operator=(GameObject&&) = delete;
        ~GameObject() = default;*/

        /// <summary>
        /// Initialize this object using a JSON text file.
        /// This will set any values specified in the file,
        /// and will create (or get from the pool) any necessary
        /// components and intialize those.
        /// </summary>
        /// <param name="pResource">The JSON TextFile resource containing object data.</param>
        /// <returns>True on success, false on failure.</returns>
        bool Initialize(TextFileResource* pResource);

        /// <summary>
        /// Removes all the components attached to this GameObject.
        /// This returns them to their respective pools.
        /// </summary>
        void RemoveComponents();

        /// <summary>
        /// Check if this GameObject is enabled.
        /// This should typically mean that none of the components
        /// will be rendered or updated if not enabled.
        /// </summary>
        /// <returns>True if enabled, false if disabled.</returns>
        bool IsEnabled() const { return m_enabled; }

        /// <summary>
        /// Sets the state of the GameObject.
        /// </summary>
        /// <param name="isEnabled">
        /// True if the object should update and render, false if it should not update or render.
        /// More specifically, it will be the components, as the GameObjects do not tick.
        /// </param>
        void SetEnabled(bool isEnabled = true) { m_enabled = isEnabled; }
        
        /// <summary>
        /// Get the objects unique ID.
        /// </summary>
        /// <returns>This objects ID.</returns>
        uint32_t GetId() const { return m_id; }

        /// <summary>
        /// Get the objects name.
        /// </summary>
        /// <returns>Name of this object.</returns>
        const eastl::string& GetName() const { return m_name; }

        /// <summary>
        /// Adds a component of the templated type to game object.
        /// </summary>
        /// <returns>The Handle to the component.</returns>
        template <class ComponentType>
        Handle AddComponent()
        {
            EXE_ASSERT(ComponentType::kType.IsValid());

            Handle newHandle = GameObjectSystem::GetInstance()->AddComponent<ComponentType>();
            EXE_ASSERT(newHandle.IsValid());

            m_components[ComponentType::kType] = newHandle;
            return newHandle;
        }

        /// <summary>
        /// Get the ComponentHandle of the templated component type.
        /// A ComponentHandle is a safe wrapper around a component.
        /// </summary>
        /// <returns>
        /// The ComponentHandle used to interface with the component.
        /// Use the operator->() to interface with the component.
        /// The ComponentHandle will be invalid if there is a failure.
        /// </returns>
        template <class ComponentType>
        ComponentHandle<ComponentType> GetComponent()
        {
            EXE_ASSERT(ComponentType::kType.IsValid());

            auto found = m_components.find(ComponentType::kType);

            // Component was not found, return invalid ComponentHandle.
            if (found == m_components.end())
            {
                EXELOG_ENGINE_ERROR("Component of type '{}' was not found.", ComponentType::kType);
                return {}; // Invalid.
            }

            // Create the ComponentHandle from the available handle.
            return ComponentHandle<ComponentType>(m_components[ComponentType::kType]);
        }

        virtual bool OnResourceLoaded(const ResourceID& resourceID) final override;

    private:
        /// <summary>
        /// Parses and creates the components in the JSON object document.
        /// This will create (or get from the pool), attach, and initialize these components.
        /// </summary>
        /// <param name="jsonDoc">The JSON document with the Object data.</param>
        void ParseComponentArray(const rapidjson::Document& jsonDoc);
	};
}