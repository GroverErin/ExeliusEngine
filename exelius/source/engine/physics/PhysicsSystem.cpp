#include "EXEPCH.h"
#include "PhysicsSystem.h"
#include "box2d/ContactListener.h"
#include "box2d/DebugDraw.h"

#include "source/engine/scenesystem/Scene.h"
#include "source/engine/gameobjects/GameObject.h"
#include "include/Time.h"

#include "source/engine/gameobjects/components/TransformComponent.h"
#include "source/engine/gameobjects/components/RigidbodyComponent.h"
#include "source/engine/gameobjects/components/BoxColliderComponent.h"
#include "source/engine/gameobjects/components/CircleColliderComponent.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	static b2BodyType ConvertRigidbodyTypeToBox2DBody(RigidbodyComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case RigidbodyComponent::BodyType::Static: return b2_staticBody;
			case RigidbodyComponent::BodyType::Dynamic: return b2_dynamicBody;
			case RigidbodyComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		EXE_ASSERT(false);
		return b2_staticBody;
	}

	PhysicsSystem::PhysicsSystem(Scene* pOwningScene)
		: m_pPhysicsWorld(nullptr)
		, m_pOwningScene(pOwningScene)
		, m_pContactListener(nullptr)
		, m_pDebugDraw(nullptr)
		, m_globalGravity(0.0f, -9.8f)
		, m_velocityIterations(6)
		, m_positionIterations(2)
	{
		EXE_ASSERT(m_pOwningScene);
	}
	
	PhysicsSystem::~PhysicsSystem()
	{
		StopRuntimePhysics();
	}

	void PhysicsSystem::InitializeRuntimePhysics()
	{
		EXE_ASSERT(m_pOwningScene);

		// Create new physics world. Since we are running on a copy of the scene from the editor.
		m_pPhysicsWorld = EXELIUS_NEW(b2World({ m_globalGravity.x, m_globalGravity.y }));
		EXE_ASSERT(m_pPhysicsWorld);

		m_pContactListener = EXELIUS_NEW(ContactListener(m_pOwningScene));
		EXE_ASSERT(m_pContactListener);

		m_pDebugDraw = EXELIUS_NEW(DebugDraw());
		EXE_ASSERT(m_pDebugDraw);
		m_pDebugDraw->SetFlags(b2Draw::e_aabbBit | b2Draw::e_shapeBit);

		m_pPhysicsWorld->SetContactListener(m_pContactListener);
		m_pPhysicsWorld->SetDebugDraw(m_pDebugDraw);

		auto view = m_pOwningScene->GetAllGameObjectsWith<RigidbodyComponent>();
		for (auto objectWithRigidBody : view)
		{
			GameObject gameObject(objectWithRigidBody, m_pOwningScene);
			TransformComponent& transform = gameObject.GetComponent<TransformComponent>();
			auto& translation = transform.m_translation;
			auto& scale = transform.m_scale;

			RigidbodyComponent& rigidBody = gameObject.GetComponent<RigidbodyComponent>();

			// Define the Box2D body def from our RigidBody and Transform.
			b2BodyDef bodyDef;
			bodyDef.type = ConvertRigidbodyTypeToBox2DBody(rigidBody.m_bodyType);
			bodyDef.position.Set(translation.x, translation.y);
			bodyDef.angle = transform.m_rotation.z;
			bodyDef.userData.pointer = (uintptr_t)(objectWithRigidBody);

			// Add the new body to the physics world.
			b2Body* pBody = m_pPhysicsWorld->CreateBody(&bodyDef);
			EXE_ASSERT(pBody);

			pBody->SetFixedRotation(rigidBody.m_isFixedRotation);

			if (rigidBody.m_isEffectedByGravity)
				pBody->SetGravityScale(rigidBody.m_gravityScale);
			else
				pBody->SetGravityScale(0.0f);

			rigidBody.m_pRuntimeBody = pBody;

			// If the object has a Box Collider...
			if (gameObject.HasComponent<BoxColliderComponent>())
			{
				BoxColliderComponent& boxCollider = gameObject.GetComponent<BoxColliderComponent>();
				auto& size = boxCollider.m_size;

				b2PolygonShape boxShape;
				boxShape.SetAsBox(size.x * scale.x, size.y * scale.y, b2Vec2(boxCollider.m_offset.x, boxCollider.m_offset.y), glm::radians(0.0f));

				// Define and Add the Box2D Fixture to the body.
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = boxCollider.m_density;
				fixtureDef.friction = boxCollider.m_friction;
				fixtureDef.restitution = boxCollider.m_restitution;
				fixtureDef.restitutionThreshold = boxCollider.m_restitutionThreshold;
				boxCollider.m_pRuntimeFixture = pBody->CreateFixture(&fixtureDef);
			}

			// If the object has a Circle Collider...
			if (gameObject.HasComponent<CircleColliderComponent>())
			{
				CircleColliderComponent& circleCollider = gameObject.GetComponent<CircleColliderComponent>();
				auto& offset = circleCollider.m_offset;

				b2CircleShape circleShape;
				circleShape.m_p.Set(offset.x, offset.y);
				circleShape.m_radius = scale.x * circleCollider.m_radius;

				// Define and Add the Box2D Fixture to the body.
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = circleCollider.m_density;
				fixtureDef.friction = circleCollider.m_friction;
				fixtureDef.restitution = circleCollider.m_restitution;
				fixtureDef.restitutionThreshold = circleCollider.m_restitutionThreshold;
				circleCollider.m_pRuntimeFixture = pBody->CreateFixture(&fixtureDef);
			}
		}
	}

	void PhysicsSystem::UpdateRuntimePhysics()
	{
		EXE_ASSERT(m_pOwningScene);
		EXE_ASSERT(m_pPhysicsWorld);

		m_contacts.clear();
		m_pPhysicsWorld->Step(Time.DeltaTime, m_velocityIterations, m_positionIterations);

		for (auto& contact : m_contacts)
		{
			for (CollisionCallback& callback : m_onCollisionCallbacks)
			{
				if (callback)
					callback(contact);
			}
		}

		// Retrieve gameobject transforms from Box2D after the simulation.
		auto view = m_pOwningScene->GetAllGameObjectsWith<RigidbodyComponent>();
		for (auto gameObjectWithRigidBody : view)
		{
			GameObject gameObject = { gameObjectWithRigidBody, m_pOwningScene };
			TransformComponent& transform = gameObject.GetComponent<TransformComponent>();
			auto& translation = transform.m_translation;
			RigidbodyComponent& rigidBody = gameObject.GetComponent<RigidbodyComponent>();

			// Update the transform with Box2D simulated values.
			b2Body* pBody = (b2Body*)rigidBody.m_pRuntimeBody;
			EXE_ASSERT(pBody);

			const auto& position = pBody->GetPosition();
			translation.x = position.x;
			translation.y = position.y;
			transform.m_rotation.z = pBody->GetAngle();
		}
	}

	void PhysicsSystem::StopRuntimePhysics()
	{
		EXELIUS_DELETE(m_pPhysicsWorld);
		EXELIUS_DELETE(m_pContactListener);
		EXELIUS_DELETE(m_pDebugDraw);
		m_pOwningScene = nullptr;
	}

	void PhysicsSystem::TryAddRuntimeBody(GameObject owningGameObject)
	{
		EXE_ASSERT(m_pOwningScene);
		if (!m_pPhysicsWorld)
			return; // We aren't currently running the simulation, so we can skip this.

		TransformComponent& transform = owningGameObject.GetComponent<TransformComponent>();
		auto& translation = transform.m_translation;

		RigidbodyComponent& rigidBody = owningGameObject.GetComponent<RigidbodyComponent>();

		// Define the Box2D body def from our RigidBody and Transform.
		b2BodyDef bodyDef;
		bodyDef.type = ConvertRigidbodyTypeToBox2DBody(rigidBody.m_bodyType);
		bodyDef.position.Set(translation.x, translation.y);
		bodyDef.angle = transform.m_rotation.z;
		bodyDef.userData.pointer = (uintptr_t)(owningGameObject.operator unsigned int());

		// Add the new body to the physics world.
		b2Body* pBody = m_pPhysicsWorld->CreateBody(&bodyDef);
		EXE_ASSERT(pBody);

		pBody->SetFixedRotation(rigidBody.m_isFixedRotation);

		if (rigidBody.m_isEffectedByGravity)
			pBody->SetGravityScale(rigidBody.m_gravityScale);
		else
			pBody->SetGravityScale(0.0f);

		rigidBody.m_pRuntimeBody = pBody;
	}

	void PhysicsSystem::TryAddRuntimeBoxCollider(GameObject owningGameObject)
	{
		if (!m_pPhysicsWorld)
			return; // We aren't currently running the simulation, so we can skip this.

		TransformComponent& transform = owningGameObject.GetComponent<TransformComponent>();
		auto& scale = transform.m_scale;

		RigidbodyComponent& rigidBody = owningGameObject.GetComponent<RigidbodyComponent>();
		b2Body* pBody = (b2Body*)rigidBody.m_pRuntimeBody;

		// If the object has a Box Collider...
		if (pBody && owningGameObject.HasComponent<BoxColliderComponent>())
		{
			BoxColliderComponent& boxCollider = owningGameObject.GetComponent<BoxColliderComponent>();
			auto& size = boxCollider.m_size;

			b2PolygonShape boxShape;
			boxShape.SetAsBox(size.x * scale.x, size.y * scale.y);

			// Define and Add the Box2D Fixture to the body.
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = boxCollider.m_density;
			fixtureDef.friction = boxCollider.m_friction;
			fixtureDef.restitution = boxCollider.m_restitution;
			fixtureDef.restitutionThreshold = boxCollider.m_restitutionThreshold;
			boxCollider.m_pRuntimeFixture = pBody->CreateFixture(&fixtureDef);
		}
	}

	void PhysicsSystem::TryAddRuntimeCircleCollider(GameObject owningGameObject)
	{
		if (!m_pPhysicsWorld)
			return; // We aren't currently running the simulation, so we can skip this.

		TransformComponent& transform = owningGameObject.GetComponent<TransformComponent>();
		auto& scale = transform.m_scale;

		RigidbodyComponent& rigidBody = owningGameObject.GetComponent<RigidbodyComponent>();
		b2Body* pBody = (b2Body*)rigidBody.m_pRuntimeBody;

		// If the object has a Circle Collider...
		if (pBody && owningGameObject.HasComponent<CircleColliderComponent>())
		{
			CircleColliderComponent& circleCollider = owningGameObject.GetComponent<CircleColliderComponent>();
			auto& offset = circleCollider.m_offset;

			b2CircleShape circleShape;
			circleShape.m_p.Set(offset.x, offset.y);
			circleShape.m_radius = scale.x * circleCollider.m_radius;

			// Define and Add the Box2D Fixture to the body.
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleShape;
			fixtureDef.density = circleCollider.m_density;
			fixtureDef.friction = circleCollider.m_friction;
			fixtureDef.restitution = circleCollider.m_restitution;
			fixtureDef.restitutionThreshold = circleCollider.m_restitutionThreshold;
			circleCollider.m_pRuntimeFixture = pBody->CreateFixture(&fixtureDef);
		}
	}

	void PhysicsSystem::AddCollisionCallback(CollisionCallback callback)
	{
		m_onCollisionCallbacks.emplace_back(callback);
	}
	
	void PhysicsSystem::OnContact(CollisionData collisionData)
	{
		m_contacts.emplace_back(collisionData);
	}
	
	void PhysicsSystem::DebugDrawPhysics()
	{
		if (m_pPhysicsWorld)
			m_pPhysicsWorld->DebugDraw();
	}
}