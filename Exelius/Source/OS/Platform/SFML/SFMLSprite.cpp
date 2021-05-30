#include "EXEPCH.h"
#include "SFMLSprite.h"
#include "Source/OS/Interface/Graphics/Texture.h"

#include "Source/Render/RenderManager.h"
#include "Source/OS/Interface/Graphics/Window.h"

#include "SFML/Graphics/Sprite.hpp"

namespace Exelius
{
	SFMLSprite::SFMLSprite()
		: m_pSFMLSprite(nullptr)
	{
		m_pSFMLSprite = new sf::Sprite();
		EXE_ASSERT(m_pSFMLSprite);
	}

	SFMLSprite::SFMLSprite(Texture& texture)
		: m_pSFMLSprite(nullptr)
	{
		m_pSFMLSprite = new sf::Sprite(*(texture.GetNativeTexture().GetSFMLTexture()));
		EXE_ASSERT(m_pSFMLSprite);
	}

	SFMLSprite::SFMLSprite(Texture& texture, const IRectangle& rectangle)
		: m_pSFMLSprite(nullptr)
	{
		sf::IntRect sfmlRect(rectangle.GetPosition().x, rectangle.GetPosition().y, rectangle.GetSize().w, rectangle.GetSize().y);
		m_pSFMLSprite = new sf::Sprite(*(texture.GetNativeTexture().GetSFMLTexture()), sfmlRect);
		EXE_ASSERT(m_pSFMLSprite);
	}

	SFMLSprite::~SFMLSprite()
	{
		delete m_pSFMLSprite;
		m_pSFMLSprite = nullptr;
	}

	IRectangle SFMLSprite::GetTextureRectangle() const
	{
		EXE_ASSERT(m_pSFMLSprite);

		IRectangle rect;
		rect.m_left = m_pSFMLSprite->getTextureRect().left;
		rect.m_top = m_pSFMLSprite->getTextureRect().top;
		rect.m_width = m_pSFMLSprite->getTextureRect().width;
		rect.m_height = m_pSFMLSprite->getTextureRect().height;

		return rect;
	}

	void SFMLSprite::SetTextureRect(const IRectangle& rectangle)
	{
		EXE_ASSERT(m_pSFMLSprite);
		sf::IntRect rect;
		rect.left = rectangle.m_left;
		rect.top = rectangle.m_top;
		rect.width = rectangle.m_width;
		rect.height = rectangle.m_height;

		m_pSFMLSprite->setTextureRect(rect);
	}

	void SFMLSprite::SetTexture(Texture& texture, bool resetRectangle)
	{
		EXE_ASSERT(m_pSFMLSprite);
		//TODO: ERROR CHECKING NEEDED.
		m_pSFMLSprite->setTexture(*(texture.GetNativeTexture().GetSFMLTexture()), resetRectangle);
	}

	Color SFMLSprite::GetColor() const
	{
		EXE_ASSERT(m_pSFMLSprite);
		uint8_t red = m_pSFMLSprite->getColor().r;
		uint8_t green = m_pSFMLSprite->getColor().g;
		uint8_t blue = m_pSFMLSprite->getColor().b;
		uint8_t alpha = m_pSFMLSprite->getColor().a;
		return Color(red, green, blue, alpha);
	}

	void SFMLSprite::SetColor(const Color& color)
	{
		EXE_ASSERT(m_pSFMLSprite);
		sf::Color sfColor;
		sfColor.r = color.r;
		sfColor.g = color.g;
		sfColor.b = color.b;
		sfColor.a = color.a;
		m_pSFMLSprite->setColor(sfColor);
 	}

	FRectangle SFMLSprite::GetLocalBounds() const
	{
		EXE_ASSERT(m_pSFMLSprite);
		
		sf::FloatRect sfRect = m_pSFMLSprite->getLocalBounds();
		FRectangle rect;
		rect.m_left = sfRect.left;
		rect.m_top = sfRect.top;
		rect.m_width = sfRect.width;
		rect.m_height = sfRect.height;

		return rect;
	}

	FRectangle SFMLSprite::GetGlobalBounds() const
	{
		EXE_ASSERT(m_pSFMLSprite);

		sf::FloatRect sfRect = m_pSFMLSprite->getGlobalBounds();
		FRectangle rect;
		rect.m_left = sfRect.left;
		rect.m_top = sfRect.top;
		rect.m_width = sfRect.width;
		rect.m_height = sfRect.height;

		return rect;
	}

	Vector2f SFMLSprite::GetPosition() const
	{
		EXE_ASSERT(m_pSFMLSprite);
		Vector2f position;
		position.x = m_pSFMLSprite->getPosition().x;
		position.y = m_pSFMLSprite->getPosition().y;
		return position;
	}

	void SFMLSprite::SetPosition(float x, float y)
	{
		EXE_ASSERT(m_pSFMLSprite);
		m_pSFMLSprite->setPosition(x, y);
	}

	void SFMLSprite::SetPosition(const Vector2f& position)
	{
		EXE_ASSERT(m_pSFMLSprite);
		sf::Vector2f sfPosition(position.x, position.y);

		m_pSFMLSprite->setPosition(sfPosition);
	}

	void SFMLSprite::Move(float xOffset, float yOffset)
	{
		EXE_ASSERT(m_pSFMLSprite);
		m_pSFMLSprite->move(xOffset, yOffset);
	}

	void SFMLSprite::Move(const Vector2f& offset)
	{
		EXE_ASSERT(m_pSFMLSprite);
		m_pSFMLSprite->move(offset.y, offset.y);
	}

	Vector2f SFMLSprite::GetScale() const
	{
		EXE_ASSERT(m_pSFMLSprite);
		Vector2f scale;
		scale.w = m_pSFMLSprite->getScale().x;
		scale.h = m_pSFMLSprite->getScale().y;
		return scale;
	}

	void SFMLSprite::SetScale(float xFactor, float yFactor)
	{
		EXE_ASSERT(m_pSFMLSprite);
		m_pSFMLSprite->setScale(xFactor, yFactor);
	}

	void SFMLSprite::SetScale(const Vector2f& factors)
	{
		EXE_ASSERT(m_pSFMLSprite);
		sf::Vector2f sfFactors(factors.x, factors.y);

		m_pSFMLSprite->setScale(sfFactors);
	}

	void SFMLSprite::Scale(float xFactorOffset, float yFactorOffset)
	{
		EXE_ASSERT(m_pSFMLSprite);
		m_pSFMLSprite->scale(xFactorOffset, yFactorOffset);
	}

	void SFMLSprite::Scale(const Vector2f& factorOffsets)
	{
		EXE_ASSERT(m_pSFMLSprite);
		sf::Vector2f sfFactorOffsets(factorOffsets.x, factorOffsets.y);

		m_pSFMLSprite->scale(sfFactorOffsets);
	}

	Vector2f SFMLSprite::GetOrigin() const
	{
		Vector2f origin;
		origin.x = m_pSFMLSprite->getOrigin().x;
		origin.y = m_pSFMLSprite->getOrigin().y;
		return origin;
	}

	void SFMLSprite::SetOrigin(float x, float y)
	{
		EXE_ASSERT(m_pSFMLSprite);
		m_pSFMLSprite->setOrigin(x, y);
	}

	void SFMLSprite::SetOrigin(const Vector2f& origin)
	{
		EXE_ASSERT(m_pSFMLSprite);
		sf::Vector2f sfOrigin(origin.x, origin.y);

		m_pSFMLSprite->setPosition(sfOrigin);
	}

	float SFMLSprite::GetRotation() const
	{
		EXE_ASSERT(m_pSFMLSprite);
		return m_pSFMLSprite->getRotation();
	}

	void SFMLSprite::SetRotation(float angle)
	{
		EXE_ASSERT(m_pSFMLSprite);
		m_pSFMLSprite->setRotation(angle);
	}

	void SFMLSprite::Rotate(float angleOffset)
	{
		EXE_ASSERT(m_pSFMLSprite);
		m_pSFMLSprite->rotate(angleOffset);
	}

	void SFMLSprite::Render()
	{
		EXE_ASSERT(m_pSFMLSprite);
		EXE_ASSERT(RenderManager::GetInstance());

		// RenderManager::GetInstance()->GetWindow()->GetNativeWindow().Draw(m_pSFMLSprite);
	}
}