#include "EXEPCH.h"
#include "ContactListener.h"
#include "source/engine/scenesystem/Scene.h"
#include "source/engine/gameobjects/GameObject.h"

#include "source/engine/physics/PhysicsSystem.h"

#include <box2d/b2_contact.h>

#include <entt/entt.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	ContactListener::ContactListener(Scene* pOwningScene)
		: m_pOwningScene(pOwningScene)
	{
		EXE_ASSERT(m_pOwningScene);
	}

	void ContactListener::BeginContact(b2Contact* pContact)
	{
		EXE_ASSERT(m_pOwningScene);
		b2Fixture* pFixtureA = pContact->GetFixtureA();
		b2Fixture* pFixtureB = pContact->GetFixtureB();

		b2Body* pBodyA = pFixtureA->GetBody();
		b2Body* pBodyB = pFixtureB->GetBody();

		entt::entity objectA = static_cast<entt::entity>(pBodyA->GetUserData().pointer);
		entt::entity objectB = static_cast<entt::entity>(pBodyB->GetUserData().pointer);

		GameObject gameObjectA(objectA, m_pOwningScene);
		GameObject gameObjectB(objectB, m_pOwningScene);

		CollisionData collisionA(gameObjectA, gameObjectB, true);
		CollisionData collisionB(gameObjectB, gameObjectA, true);

		m_pOwningScene->GetPhysicsSystem().OnContact(collisionA);
		m_pOwningScene->GetPhysicsSystem().OnContact(collisionB);
	}

	void ContactListener::EndContact(b2Contact* pContact)
	{
		EXE_ASSERT(m_pOwningScene);
		b2Fixture* pFixtureA = pContact->GetFixtureA();
		b2Fixture* pFixtureB = pContact->GetFixtureB();

		b2Body* pBodyA = pFixtureA->GetBody();
		b2Body* pBodyB = pFixtureB->GetBody();

		entt::entity objectA = static_cast<entt::entity>(pBodyA->GetUserData().pointer);
		entt::entity objectB = static_cast<entt::entity>(pBodyB->GetUserData().pointer);

		GameObject gameObjectA(objectA, m_pOwningScene);
		GameObject gameObjectB(objectB, m_pOwningScene);

		CollisionData collisionA(gameObjectA, gameObjectB, false);
		CollisionData collisionB(gameObjectB, gameObjectA, false);

		m_pOwningScene->GetPhysicsSystem().OnContact(collisionA);
		m_pOwningScene->GetPhysicsSystem().OnContact(collisionB);
	}

	void ContactListener::PreSolve(b2Contact*, const b2Manifold*)
	{
		EXE_ASSERT(m_pOwningScene);
	}

	void ContactListener::PostSolve(b2Contact*, const b2ContactImpulse*)
	{
		EXE_ASSERT(m_pOwningScene);
	}
}