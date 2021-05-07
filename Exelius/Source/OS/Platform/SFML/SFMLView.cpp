#include "EXEPCH.h"
#include "SFMLView.h"

#include <SFML/Graphics/View.hpp>

namespace Exelius
{
	SFMLView::SFMLView()
		: m_pSFMLView(nullptr)
	{
		m_pSFMLView = new sf::View();
		EXE_ASSERT(m_pSFMLView);
	}

	SFMLView::SFMLView(const FRectangle& rectangle)
		: m_pSFMLView(nullptr)
	{
		sf::FloatRect sfmlRect;
		sfmlRect.top = rectangle.m_top;
		sfmlRect.left = rectangle.m_left;
		sfmlRect.width = rectangle.m_width;
		sfmlRect.height = rectangle.m_height;

		m_pSFMLView = new sf::View(sfmlRect);
		EXE_ASSERT(m_pSFMLView);
	}

	SFMLView::SFMLView(const Vector2f& center, const Vector2f& size)
		: m_pSFMLView(nullptr)
	{
		sf::Vector2f sfmlCenter;
		sfmlCenter.x = center.x;
		sfmlCenter.y = center.y;

		sf::Vector2f sfmlSize;
		sfmlSize.x = size.w;
		sfmlSize.y = size.h;

		m_pSFMLView = new sf::View(sfmlCenter, sfmlSize);
		EXE_ASSERT(m_pSFMLView);
	}

	void SFMLView::SetCenter(float x, float y)
	{
		EXE_ASSERT(m_pSFMLView);
		m_pSFMLView->setCenter(x, y);
	}

	void SFMLView::SetCenter(const Vector2f& center)
	{
		EXE_ASSERT(m_pSFMLView);
		SetCenter(center.x, center.y);
	}

	void SFMLView::SetSize(float width, float height)
	{
		EXE_ASSERT(m_pSFMLView);
		m_pSFMLView->setSize(width, height);
	}

	void SFMLView::SetSize(const Vector2f& size)
	{
		EXE_ASSERT(m_pSFMLView);
		SetSize(size.w, size.h);
	}

	void SFMLView::SetRotation(float angle)
	{
		EXE_ASSERT(m_pSFMLView);
		m_pSFMLView->setRotation(angle);
	}

	void SFMLView::SetViewport(const FRectangle& viewport)
	{
		EXE_ASSERT(m_pSFMLView);

		sf::FloatRect sfmlViewport;
		sfmlViewport.top = viewport.m_top;
		sfmlViewport.left = viewport.m_left;
		sfmlViewport.width = viewport.m_width;
		sfmlViewport.height = viewport.m_height;

		m_pSFMLView->setViewport(sfmlViewport);
	}

	void SFMLView::Reset(const FRectangle& rectangle)
	{
		EXE_ASSERT(m_pSFMLView);

		sf::FloatRect sfmlRect;
		sfmlRect.top = rectangle.m_top;
		sfmlRect.left = rectangle.m_left;
		sfmlRect.width = rectangle.m_width;
		sfmlRect.height = rectangle.m_height;

		m_pSFMLView->reset(sfmlRect);
	}

	Vector2f SFMLView::GetCenter() const
	{
		EXE_ASSERT(m_pSFMLView);

		Vector2f vect;
		vect.x = m_pSFMLView->getCenter().x;
		vect.y = m_pSFMLView->getCenter().y;

		return vect;
	}

	Vector2f SFMLView::GetSize() const
	{
		EXE_ASSERT(m_pSFMLView);

		Vector2f vect;
		vect.w = m_pSFMLView->getSize().x;
		vect.h = m_pSFMLView->getSize().y;

		return vect;
	}

	float SFMLView::GetRotation() const
	{
		EXE_ASSERT(m_pSFMLView);
		return m_pSFMLView->getRotation();
	}

	FRectangle SFMLView::GetViewport() const
	{
		EXE_ASSERT(m_pSFMLView);
		
		FRectangle rect;
		rect.m_top = m_pSFMLView->getViewport().top;
		rect.m_left = m_pSFMLView->getViewport().left;
		rect.m_width = m_pSFMLView->getViewport().width;
		rect.m_height = m_pSFMLView->getViewport().height;

		return rect;
	}

	void SFMLView::Move(float xOffset, float yOffset)
	{
		EXE_ASSERT(m_pSFMLView);
		m_pSFMLView->move(xOffset, yOffset);
	}

	void SFMLView::Move(const Vector2f& offset)
	{
		EXE_ASSERT(m_pSFMLView);
		Move(offset.x, offset.y);
	}

	void SFMLView::Rotate(float angleOffset)
	{
		EXE_ASSERT(m_pSFMLView);
		m_pSFMLView->rotate(angleOffset);
	}

	void SFMLView::Zoom(float factor)
	{
		EXE_ASSERT(m_pSFMLView);
		m_pSFMLView->zoom(factor);
	}
}