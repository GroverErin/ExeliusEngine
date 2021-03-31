#include "EXEPCH.h"
#include "Source/Engine/Game/GameObjects/GameObjectFactory.h"
#include "Source/Engine/Game/GameObjects/GameObject.h"

namespace Exelius
{
	GameObject* GameObjectFactory::CreateGameObject(JSONResource* pResource)
	{
		EXE_ASSERT(pResource);

		GameObject* pNewObject = new GameObject(GetNextObjectId());
		if (!pNewObject->Initialize(pResource))
		{
			EXELOG_ENGINE_ERROR("Failed to initialize GameObject from Resource: ");
			return nullptr;
		}
		return pNewObject;
	}
}