#pragma once
#include <rapidjson/document.h>
#include "Source/Utility/String/StringIntern.h"

namespace Exelius
{
#define DEFINE_COMPONENT(COMPONENTTYPE) public: inline static const Type kType = #COMPONENTTYPE

	class GameObject;

	class Component
	{
	public:
		using Type = StringIntern;
		inline static const Type kInvalidType = "InvalidComponent";
	protected:
		GameObject* m_pOwner;
	public:
		Component()
			: m_pOwner(nullptr)
		{
		}

		virtual ~Component() = default;

		virtual bool Initialize(GameObject* pOwner)
		{
			assert(pOwner);
			m_pOwner = pOwner;
			return true;
		}

		virtual bool Initialize(GameObject* pOwner, [[maybe_unused]] const rapidjson::Value& jsonComponentData)
		{
			assert(pOwner);
			m_pOwner = pOwner;
			return true;
		}

		virtual void Update() {}
		virtual void Render() const {}

		const GameObject* GetOwner() { return m_pOwner; }
	};
}