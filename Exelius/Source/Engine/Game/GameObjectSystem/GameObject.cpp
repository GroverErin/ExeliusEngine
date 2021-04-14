#include "EXEPCH.h"
#include "Source/Engine/Game/GameObjectSystem/GameObject.h"
#include "Source/Engine/Resources/ResourceTypes/TextFileResource.h"

namespace Exelius
{
	GameObject::GameObject(uint32_t id)
		: m_id(id)
		, m_enabled(true)
		, m_name("Invalid")
	{
	}

	bool GameObject::Initialize()
	{
		// Name the object based on it's ID.
		m_name = "New GameObject (";
		//m_name += eastl::to_string(m_id);    <-- Why does this cause errors?!!
		m_name += ")";
		return true;
	}

	bool GameObject::Initialize(TextFileResource* pResource)
	{
		EXE_ASSERT(pResource);

		// TODO: Parse the JSON file here.
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
			EXELOG_ENGINE_INFO("No 'Name' field found. Setting object name to 'Unnamed'.");
			m_name = "Unnamed";
		}

		// Create and Initialize any Components.
		ParseComponentArray(jsonDoc);

		return true;
	}

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
			if (!newComponentHandle)
				continue;

			m_components.try_emplace(componentMember->name.GetString(), newComponentHandle);
		}
	}

	void GameObject::RemoveComponents()
	{
		auto* pGameObjectSystem = GameObjectSystem::GetInstance();
		for (auto& componentPair : m_components)
		{
			pGameObjectSystem->ReleaseComponent(componentPair.first, componentPair.second);
		}
		
		m_components.clear();
	}
}