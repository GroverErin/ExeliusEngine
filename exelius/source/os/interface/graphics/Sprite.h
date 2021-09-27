#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/math/Rectangle.h"
#include "source/utility/generic/Color.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(Texture);

	/// <summary>
	/// Templated window class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplSprite>
	class _Sprite
	{
		ImplSprite m_impl;
	public:
		_Sprite()
			: m_impl()
		{
			//
		}

		_Sprite(Texture& texture)
			: m_impl(texture)
		{
			//
		}

		_Sprite(Texture& texture, const IRectangle& rectangle)
			: m_impl(texture, rectangle)
		{
			//
		}

		IRectangle GetTextureRectangle() const
		{
			return m_impl.GetTextureRectangle();
		}

		void SetTextureRect(const IRectangle& rectangle)
		{
			m_impl.SetTextureRect(rectangle);
		}

		void SetTexture(Texture& texture, bool resetRectangle = false)
		{
			m_impl.SetTexture(texture, resetRectangle);
		}

		Color GetColor() const
		{
			return m_impl.GetColor();
		}

		void SetColor(const Color& color)
		{
			m_impl.SetColor(color);
		}

		FRectangle GetLocalBounds() const
		{
			return m_impl.GetLocalBounds();
		}

		FRectangle GetGlobalBounds() const
		{
			return m_impl.GetGlobalBounds();
		}

		Vector2f GetPosition() const
		{
			return m_impl.GetPosition();
		}

		void SetPosition(float x, float y)
		{
			m_impl.SetPosition(x, y);
		}

		void SetPosition(const Vector2f& position)
		{
			m_impl.SetPosition(position);
		}

		void Move(float xOffset, float yOffset)
		{
			m_impl.Move(xOffset, yOffset);
		}

		void Move(const Vector2f& offset)
		{
			m_impl.Move(offset);
		}

		Vector2f GetScale() const
		{
			return m_impl.GetScale();
		}

		void SetScale(float xFactor, float yFactor)
		{
			m_impl.SetScale(xFactor, yFactor);
		}

		void SetScale(const Vector2f& factors)
		{
			m_impl.SetScale(factors);
		}

		void Scale(float xFactorOffset, float yFactorOffset)
		{
			m_impl.Scale(xFactorOffset, yFactorOffset);
		}

		void Scale(const Vector2f& factorOffsets)
		{
			m_impl.Scale(factorOffsets);
		}

		Vector2f GetOrigin() const
		{
			return m_impl.GetOrigin();
		}

		void SetOrigin(float x, float y)
		{
			m_impl.SetPosition(x, y);
		}

		void SetOrigin(const Vector2f& origin)
		{
			m_impl.SetPosition(origin);
		}

		float GetRotation() const
		{
			return m_impl.GetRotation();
		}

		void SetRotation(float angle)
		{
			m_impl.SetRotation(angle);
		}

		void Rotate(float angleOffset)
		{
			m_impl.Rotate(angleOffset);
		}

		void Render() { m_impl.Render(); }

		ImplSprite& GetNativeSprite() { return m_impl; }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "source/os/platform/sfml/SFMLSprite.h"
	namespace Exelius
	{
		using Sprite = _Sprite<SFMLSprite>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif