#include "EXEPCH.h"
#include "Source/Engine/Game/GameObjectSystem/GameObject.h"
#include "Source/Engine/Resources/ResourceTypes/TextFileResource.h"

namespace Exelius
{
	/// <summary>
	/// Constructor - Initializes member variables.
	/// TODO: This should not be publicly accessable!
	/// </summary>
	/// <param name="id">The unique object ID given by the GameObjectSystem.</param>
	GameObject::GameObject(uint32_t id)
		: m_id(id)
		, m_enabled(true)
		, m_name("Invalid")
	{
		EXE_ASSERT(m_id != GameObjectSystem::kInvalidID);
	}

	/// <summary>
	/// Empty initialization will intialize the name of the object using the
	/// unique ID. No components will be set.
	/// </summary>
	/// <returns>True on success, false on failure.</returns>
	bool GameObject::Initialize()
	{
		// Name the object based on it's ID.
		m_name = "New GameObject (";
		//m_name += eastl::to_string(m_id);															<-- Why does this cause errors?!!
		m_name += ")";
		return true;
	}

	/// <summary>
	/// Initialize this object using a JSON text file.
	/// This will set any values specified in the file,
	/// and will create (or get from the pool) any necessary
	/// components and intialize those.
	/// </summary>
	/// <param name="pResource">The JSON TextFile resource containing object data.</param>
	/// <returns>True on success, false on failure.</returns>
	bool GameObject::Initialize(TextFileResource* pResource)
	{
		EXE_ASSERT(pResource);
		
		const eastl::string& jsonText = pResource->GetRawText();

		rapidjson::Document jsonDoc;
		if (jsonDoc.Parse(jsonText.c_str()).HasParseError())
		{
			EXELOG_ENGINE_ERROR("Failed to Parse JSON.");
			return false;
		}

		EXE_ASSERT(jsonDoc.IsObject());

		// Set any game object specific values.
		auto nameMember = jsonDoc.FindMember("Name");

		if (nameMember != jsonDoc.MemberEnd())
		{
			EXE_ASSERT(nameMember->value.IsString());
			m_name = nameMember->value.GetString();
		}
		else
		{
			EXELOG_ENGINE_INFO("No 'Name' field found. Setting object name to a default value.");
			// Name the object based on it's ID.
			m_name = "New GameObject (";
			//m_name += eastl::to_string(m_id);														<-- Why does this cause errors?!!
			m_name += ")";
		}

		// Create and Initialize any Components.
		ParseComponentArray(jsonDoc);

		return true;
	}

	/// <summary>
	/// Removes all the components attached to this GameObject.
	/// This returns them to their respective pools.
	/// </summary>
	void GameObject::RemoveComponents()
	{
		auto* pGameObjectSystem = GameObjectSystem::GetInstance();
		EXE_ASSERT(pGameObjectSystem);

		for (auto& componentPair : m_components)
		{
			EXE_ASSERT(componentPair.first.IsValid());
			EXE_ASSERT(componentPair.second.IsValid());

			pGameObjectSystem->ReleaseComponent(componentPair.first, componentPair.second);
		}

		m_components.clear();
	}

	/// <summary>
	/// Parses and creates the components in the JSON object document.
	/// This will create (or get from the pool), attach, and initialize these components.
	/// </summary>
	/// <param name="jsonDoc">The JSON document with the Object data.</param>
	void GameObject::ParseComponentArray(const rapidjson::Document& jsonDoc)
	{
		auto* pGameObjectSystem = GameObjectSystem::GetInstance();
		EXE_ASSERT(pGameObjectSystem);

		// Find an Array with name 'Components'
		auto componentArrayMember = jsonDoc.FindMember("Components");

		// If the Array does not exist then bail.
		if (componentArrayMember == jsonDoc.MemberEnd())
		{
			EXELOG_ENGINE_INFO("No 'Components' field found.");
			return;
		}

		auto& componentArrayValue = componentArrayMember->value;

		// Make sure it's value is an Object (List of).
		EXE_ASSERT(componentArrayValue.IsObject());

		// For each Component in the Components List.
		for (auto componentMember = componentArrayValue.MemberBegin(); componentMember != componentArrayValue.MemberEnd(); ++componentMember)
		{
			Handle newComponentHandle = pGameObjectSystem->CreateComponentFromFactory(componentMember->name.GetString(), this, componentMember->value);
			if (!newComponentHandle.IsValid())
				continue;

			m_components.try_emplace(componentMember->name.GetString(), newComponentHandle);
		}
	}
}