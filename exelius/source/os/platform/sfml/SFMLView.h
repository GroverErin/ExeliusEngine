#pragma once
#include "source/utility/math/rectangle.h"

#include <cstddef>

/// <summary>
/// SFML namespace, used only in SFML specific code within Exelius.
/// </summary>
namespace sf
{
	class View;
}

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class SFMLView
	{
		sf::View* m_pSFMLView;
	public:
		SFMLView();
		SFMLView(const FRectangle& rectangle);
		SFMLView(const Vector2f& center, const Vector2f& size);

		// TODO:
		// Rule of 5
		~SFMLView();

		void SetCenter(float x, float y);
		void SetCenter(const Vector2f& center);

		void SetSize(float width, float height);
		void SetSize(const Vector2f& size);

		void SetRotation(float angle);

		void SetViewport(const FRectangle& viewport);

		void Reset(const FRectangle& rectangle);

		Vector2f GetCenter() const;

		Vector2f GetSize() const;

		float GetRotation() const;

		FRectangle GetViewport() const;

		void Move(float xOffset, float yOffset);
		void Move(const Vector2f& offset);

		void Rotate(float angleOffset);

		void Zoom(float factor);

		sf::View& GetSFMLView() const { return *m_pSFMLView; }
	};
}