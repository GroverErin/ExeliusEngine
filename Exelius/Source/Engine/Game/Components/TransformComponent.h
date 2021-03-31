#pragma once
#include "Source/Engine/Game/Components/Component.h"

namespace Exelius
{
	class Gameobject;

	class TransformComponent
		: public Component
	{
		float m_xPosition;
		float m_yPosition;
		float m_width;
		float m_height;
	public:
		TransformComponent(GameObject* pOwner)
			: Component(pOwner)
			, m_xPosition(0)
			, m_yPosition(0)
			, m_width(0)
			, m_height(0)
		{
			//
		}
		TransformComponent() = delete;
		TransformComponent(const TransformComponent&) = delete;
		TransformComponent(TransformComponent&&) = delete;
		TransformComponent& operator=(const TransformComponent&) = delete;
		virtual ~TransformComponent() = default;

		virtual bool Initialize() final override;

		float GetX() const { return m_xPosition; }
		void SetX(float x) { m_xPosition = x; }
		float GetY() const { return m_yPosition; }
		void SetY(float y) { m_yPosition = y; }
		float GetW() const { return m_width; }
		void SetW(float w) { m_width = w; }
		float GetH() const { return m_height; }
		void SetH(float h) { m_height = h; }
	};
}