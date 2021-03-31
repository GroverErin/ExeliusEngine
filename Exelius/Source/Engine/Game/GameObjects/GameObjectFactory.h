#pragma once

namespace Exelius
{
	class GameObject;
	class JSONResource;

	class GameObjectFactory
	{
		int m_nextObjectID;

	public:
		GameObjectFactory()
			: m_nextObjectID(0)
		{
			//
		}

		GameObjectFactory(const GameObjectFactory&) = delete;
		GameObjectFactory(GameObjectFactory&&) = delete;
		GameObjectFactory& operator=(const GameObjectFactory&) = delete;
		~GameObjectFactory() = default;

		GameObject* CreateGameObject(JSONResource* pResource);

	private:

		//Returns THEN increments.
		int GetNextObjectId() { return m_nextObjectID++; }
	};
}