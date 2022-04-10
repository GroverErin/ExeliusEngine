#include "EXEPCH.h"
#include "ContactListener.h"
#include "source/engine/scenesystem/Scene.h"
#include "source/engine/gameobjects/GameObject.h"

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

		auto objectA = static_cast<entt::entity>(pBodyA->GetUserData().pointer);
		auto objectB = static_cast<entt::entity>(pBodyB->GetUserData().pointer);

		GameObject gameObjectA = GameObject{ objectA, m_pOwningScene };
		GameObject gameObjectB = GameObject{ objectB, m_pOwningScene };
	}

	void ContactListener::EndContact(b2Contact* pContact)
	{
		EXE_ASSERT(m_pOwningScene);
		b2Fixture* pFixtureA = pContact->GetFixtureA();
		b2Fixture* pFixtureB = pContact->GetFixtureB();

		b2Body* pBodyA = pFixtureA->GetBody();
		b2Body* pBodyB = pFixtureB->GetBody();

		auto objectA = static_cast<entt::entity>(pBodyA->GetUserData().pointer);
		auto objectB = static_cast<entt::entity>(pBodyB->GetUserData().pointer);

		GameObject gameObjectA = GameObject{ objectA, m_pOwningScene };
		GameObject gameObjectB = GameObject{ objectB, m_pOwningScene };
	}

	void ContactListener::PreSolve(b2Contact* pContact, const b2Manifold* pOldManifold)
	{
		EXE_ASSERT(m_pOwningScene);
	}

	void ContactListener::PostSolve(b2Contact* pContact, const b2ContactImpulse* pImpulse)
	{
		EXE_ASSERT(m_pOwningScene);
	}
}