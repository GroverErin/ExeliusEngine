#include "EXEPCH.h"
#include "Source/Engine/Game/GameObjects/GameObject.h"
#include "Source/Engine/Game/Components/Component.h"
#include "Source/Engine/Resources/JSONResource.h"

namespace Exelius
{
	bool GameObject::Initialize([[maybe_unused]] JSONResource* pResource)
	{
		// TODO: Parse the JSON file here.
		return true;
	}

	void GameObject::Destroy()
	{
		for (auto* pComponent : m_components)
		{
			delete pComponent;
			pComponent = nullptr;
		}

		m_components.clear();
	}

	void GameObject::Update(float deltaTime)
	{
		for (auto* pComponent : m_components)
		{
			pComponent->Update(deltaTime);
		}
	}

	void GameObject::Render()
	{
		for (auto* pComponent : m_components)
		{
			pComponent->Render();
		}
	}

	void GameObject::AddComponent(Component* pComponent)
	{
		if (!pComponent)
		{
			EXELOG_ENGINE_ERROR("Cannot add component: Component was nullptr.");
			return;
		}

		m_components.emplace_back(pComponent);
	}
}