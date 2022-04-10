#pragma once

#include <box2d/b2_world_callbacks.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Scene;

	class ContactListener
		: public b2ContactListener
	{
		Scene* m_pOwningScene;

	public:
		ContactListener(Scene* pOwningScene);
		virtual ~ContactListener() = default;

		virtual void BeginContact(b2Contact* pContact) override;

		virtual void EndContact(b2Contact* pContact) override;

		virtual void PreSolve(b2Contact* pContact, const b2Manifold* pOldManifold) override;

		virtual void PostSolve(b2Contact* pContact, const b2ContactImpulse* pImpulse) override;
	};
}