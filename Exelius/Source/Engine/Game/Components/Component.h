#pragma once

namespace Exelius
{
	class GameObject;

	class Component
	{
		GameObject* m_pOwner;
	public:
		Component(GameObject* pOwner)
			: m_pOwner(pOwner)
		{
			//
		}
		Component() = delete;
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = delete;
		virtual ~Component() = default;

		virtual bool Initialize() = 0;
		virtual void Update([[maybe_unused]] float deltaTime) {}
		virtual void Render() {}

		//virtual int GetId() const = 0;
		GameObject* GetOwner() { return m_pOwner; }
	};
}