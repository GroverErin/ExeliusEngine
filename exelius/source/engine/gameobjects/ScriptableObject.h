#pragma once
#include "GameObject.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class ScriptableObject
	{
		friend class Scene;
		GameObject m_gameObject;

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate() {}

	public:
		virtual ~ScriptableObject() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_gameObject.GetComponent<T>();
		}
	};
}
