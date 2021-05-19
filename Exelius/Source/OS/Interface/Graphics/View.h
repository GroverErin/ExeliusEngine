#pragma once
#include "Source/Utility/Math/Rectangle.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Templated view class using CRTP.
	/// https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
	/// </summary>
	template <class ImplView>
	class _View
	{
		ImplView m_impl;
	public:
		_View()
			: m_impl()
		{
			//
		}

		_View(const FRectangle& rectangle)
			: m_impl(rectangle)
		{
			//
		}

		_View(const Vector2f& center, const Vector2f& size)
			: m_impl(center, size)
		{
			//
		}

		void SetCenter(float x, float y)
		{
			m_impl.SetCenter(x, y);
		}

		void SetCenter(const Vector2f& center)
		{
			m_impl.SetCenter(center);
		}

		void SetSize(float width, float height)
		{
			m_impl.SetSize(width, height);
		}

		void SetSize(const Vector2f& size)
		{
			m_impl.SetSize(size);
		}

		void SetRotation(float angle)
		{
			m_impl.SetRotation(angle);
		}

		void SetViewport(const FRectangle& viewport)
		{
			m_impl.SetViewport(viewport);
		}

		void Reset(const FRectangle& rectangle)
		{
			m_impl.Reset(rectangle);
		}

		Vector2f GetCenter() const
		{
			return m_impl.GetCenter();
		}

		Vector2f GetSize() const
		{
			return m_impl.GetSize();
		}

		float GetRotation() const
		{
			return m_impl.GetRotation();
		}

		FRectangle GetViewport() const
		{
			return m_impl.GetViewport();
		}

		void Move(float xOffset, float yOffset)
		{
			m_impl.Move(xOffset, yOffset);
		}

		void Move(const Vector2f& offset)
		{
			m_impl.Move(offset);
		}

		void Rotate(float angleOffset)
		{
			m_impl.Rotate(angleOffset);
		}

		void Zoom(float factor)
		{
			m_impl.Zoom(factor);
		}

		const ImplView& GetNativeView() const { return m_impl; }
	};
}

#if EXELIUS_RENDERER == SFML_RENDERER
	#include "Source/OS/Platform/SFML/SFMLView.h"
	namespace Exelius
	{
		using View = _View<SFMLView>;
	}
#else
	#error "Unknown Render Skin Implementation."
#endif