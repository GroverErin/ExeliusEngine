#pragma once

#include <glm/glm.hpp>

class b2World;

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Scene;
	class ContactListener;

	class PhysicsSystem
	{
		// Physics members.
		b2World* m_pPhysicsWorld;
		ContactListener* m_pContactListener;
		glm::vec2 m_globalGravity;
		int32_t m_velocityIterations;
		int32_t m_positionIterations;

	public:
		PhysicsSystem();
		~PhysicsSystem();

		void InitializeRuntimePhysics(Scene* pOwningScene);

		void UpdateRuntimePhysics(Scene* pOwningScene);

		void StopRuntimePhysics();
	};
}