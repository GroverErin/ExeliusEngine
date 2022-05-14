#pragma once
#include "source/engine/gameobjects/GameObject.h"

#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include <EASTL/vector.h>
#include <EASTL/functional.h>

class b2World;

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Scene;
	class ContactListener;
	class DebugDraw;

	struct CollisionData
	{
		GameObject m_thisGameObject;
		GameObject m_otherGameObject;
		bool m_isCollisionEnter;

		CollisionData(GameObject thisGameObject, GameObject otherGameObject, bool isCollisionEnter)
			: m_thisGameObject(thisGameObject)
			, m_otherGameObject(otherGameObject)
			, m_isCollisionEnter(isCollisionEnter)
		{
			//
		}
	};

	class PhysicsSystem
	{
	public:
		using CollisionCallback = eastl::function<void(CollisionData)>;
	private:
		b2World* m_pPhysicsWorld;
		Scene* m_pOwningScene;
		ContactListener* m_pContactListener;
		DebugDraw* m_pDebugDraw;
		glm::vec2 m_globalGravity;
		int32_t m_velocityIterations;
		int32_t m_positionIterations;

		eastl::vector<CollisionCallback> m_onCollisionCallbacks;

		eastl::vector<CollisionData> m_contacts;

	public:
		PhysicsSystem(Scene* pOwningScene);
		~PhysicsSystem();

		void InitializeRuntimePhysics();

		void UpdateRuntimePhysics();

		void StopRuntimePhysics();

		void TryAddRuntimeBody(GameObject owningGameObject);

		void TryAddRuntimeBoxCollider(GameObject owningGameObject);
		void TryAddRuntimeCircleCollider(GameObject owningGameObject);

		void AddCollisionCallback(CollisionCallback callback);

		void OnContact(CollisionData collisionData);

		void DebugDrawPhysics();
	};
}