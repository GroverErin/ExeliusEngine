#include "EXEPCH.h"
#include "UIElement.h"

#include "source/engine/ui/UIElementFactory.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	UIElement::UIElement(UIElement* pParentElement)
		: m_pParent(pParentElement)
		, m_childLayoutType(LayoutType::None)
	{
		//
	}

	bool UIElement::Initialize(const rapidjson::Value& jsonUIElementData)
	{
        // -----------------------------------------------------------------------------------------
        // Layout
        // -----------------------------------------------------------------------------------------

        auto rootLayoutType = jsonUIElementData.FindMember("LayoutType");

        if (rootLayoutType == jsonUIElementData.MemberEnd())
        {
            m_childLayoutType = UIElement::LayoutType::None;
        }
        else
        {
            EXE_ASSERT(rootLayoutType->value.IsNumber());
            int rootLayout = rootLayoutType->value.GetInt();
            m_childLayoutType = (UIElement::LayoutType)rootLayout;
        }

		// -----------------------------------------------------------------------------------------
		// Coordinates
		// -----------------------------------------------------------------------------------------

        m_relativeOriginX = ParseCoordinateData(jsonUIElementData, "originX");
        m_relativeOriginY = ParseCoordinateData(jsonUIElementData, "originY");
        m_relativeRegionX = ParseCoordinateData(jsonUIElementData, "x");
        m_relativeRegionY = ParseCoordinateData(jsonUIElementData, "y");
        m_relativeRegionW = ParseCoordinateData(jsonUIElementData, "w");
        m_relativeRegionH = ParseCoordinateData(jsonUIElementData, "h");

		// -----------------------------------------------------------------------------------------
		// Color
		// -----------------------------------------------------------------------------------------

		eastl::array<uint8_t, 4> rgbaColorValues = { 255, 255, 255, 255 };
		auto colorData = jsonUIElementData.FindMember("Color");

		if (colorData != jsonUIElementData.MemberEnd())
		{
			EXE_ASSERT(colorData->value.IsArray());

			size_t index = 0;
			for (auto& colorVal : colorData->value.GetArray())
			{
				EXE_ASSERT(colorVal.IsNumber());
				rgbaColorValues[index] = (uint8_t)colorVal.GetUint();
				++index;
			}
		}

		m_color.r = rgbaColorValues[0];
		m_color.g = rgbaColorValues[1];
		m_color.b = rgbaColorValues[2];
		m_color.a = rgbaColorValues[3];

        // -----------------------------------------------------------------------------------------
        // Children
        // -----------------------------------------------------------------------------------------

        auto rootChildren = jsonUIElementData.FindMember("Children");

        if (rootChildren == jsonUIElementData.MemberEnd())
        {
            return true; // We have an empty UI component... not sure if this is valid yet.
        }

        EXE_ASSERT(rootChildren->value.IsObject());

        for (auto child = rootChildren->value.MemberBegin(); child != rootChildren->value.MemberEnd(); ++child)
        {
            EXE_ASSERT(child->name.IsString());
			EXE_ASSERT(child->value.IsObject());
			UIElement* pChild = UIElementFactory::CreateUIElement(child->name.GetString(), this);

			if (!pChild->Initialize(child->value))
				return false; // Child failed to init.

			m_children.emplace_back(pChild);
        }

		return true;
	}

	void UIElement::OnUpdate(const FRectangle& parentRegion)
	{
		float originX = m_relativeOriginX.Resolve(m_actualRegion.w);
		float originY = m_relativeOriginY.Resolve(m_actualRegion.h);

		m_actualRegion.x = parentRegion.x - originX + m_relativeRegionX.Resolve(parentRegion.w);
		m_actualRegion.y = parentRegion.y - originY + m_relativeRegionY.Resolve(parentRegion.h);
		m_actualRegion.w = m_relativeRegionW.Resolve(parentRegion.w);
		m_actualRegion.h = m_relativeRegionH.Resolve(parentRegion.h);

		HandleLayoutUpdates();
	}

	void UIElement::OnRender()
	{
		// TODO:
		//if (!isVisible)
		//	return;

		for (auto* pChild : m_children)
		{
			pChild->OnRender();
		}
	}

	UIElement* UIElement::HitTest(Vector2f mousePosition)
	{
		if (!m_actualRegion.IsPointInRect(mousePosition))
			return nullptr;

		for (auto* pChild : m_children)
		{
			UIElement* pHitElement = pChild->HitTest(mousePosition);
			if (pHitElement)
				return pHitElement;
		}

		return this;
	}

    RelativeCoordinate UIElement::ParseCoordinateData(const rapidjson::Value& jsonUIElementData, const char* pCoordinateName)
    {
        EXE_ASSERT(pCoordinateName);
        RelativeCoordinate resultCoord;
        auto coordData = jsonUIElementData.FindMember(pCoordinateName);

        if (coordData != jsonUIElementData.MemberEnd())
        {
            EXE_ASSERT(coordData->value.IsArray());
            for (auto& value : coordData->value.GetArray())
            {
                if (value.IsString())
                {
                    char coordType = *value.GetString();
                    if (coordType == '%')
                        resultCoord.m_type = RelativeCoordinate::UnitType::Percentage;
                }
                else if (value.IsNumber())
                {
                    float coord = value.GetFloat();
                    resultCoord.m_value = coord;
                }
                else
                {
                    EXE_ASSERT(false); // Unexpected type detected.
                }
            }
        }

        return resultCoord;
    }

	void UIElement::HandleLayoutUpdates()
	{
		switch (m_childLayoutType)
		{
		case LayoutType::None:
			NoLayoutUpdate();
			break;
		case LayoutType::StackHorizontal:
			StackLayoutUpdate();
			break;
		case LayoutType::StackVertical:
			StackLayoutUpdate(false);
			break;
		case LayoutType::WrapHorizontal:
			WrapLayoutUpdate();
			break;
		case LayoutType::WrapVertical:
			WrapLayoutUpdate(false);
			break;
		default:
			EXE_ASSERT(false);
			break;
		}
	}

	void UIElement::NoLayoutUpdate()
	{
		for (auto* pChild : m_children)
		{
			pChild->OnUpdate(m_actualRegion);
		}
	}
	
	void UIElement::StackLayoutUpdate(bool isHorizontal /* = true */)
	{
		float shiftOffset = 0.0f;

		for (auto* pChild : m_children)
		{
			FRectangle newPosition = m_actualRegion;
			if (isHorizontal)
			{
				newPosition.x += shiftOffset;
				pChild->OnUpdate(newPosition);
				shiftOffset += pChild->m_actualRegion.w;
			}
			else
			{
				newPosition.y += shiftOffset;
				pChild->OnUpdate(newPosition);
				shiftOffset += pChild->m_actualRegion.h;
			}
		}
	}

	void UIElement::WrapLayoutUpdate(bool isHorizontal /* = true */)
	{
		float nextX = 0.0f;
		float nextY = 0.0f;
		for (auto* pChild : m_children)
		{
			pChild->OnUpdate({m_actualRegion.x + nextX, m_actualRegion.y + nextY, m_actualRegion.w, m_actualRegion.h});
			if (isHorizontal)
			{
				if ((pChild->m_actualRegion.x + pChild->m_actualRegion.w) > m_actualRegion.w)
				{
					nextX = 0;
					nextY += pChild->m_actualRegion.h;
					pChild->OnUpdate({ m_actualRegion.x + nextX, m_actualRegion.y + nextY, m_actualRegion.w, m_actualRegion.h });
				}

				nextX += pChild->m_actualRegion.w;
			}
			else
			{
				if ((pChild->m_actualRegion.y + pChild->m_actualRegion.h) > m_actualRegion.h)
				{
					nextY = 0;
					nextX += pChild->m_actualRegion.w;
					pChild->OnUpdate({ m_actualRegion.x + nextX, m_actualRegion.y + nextY, m_actualRegion.w, m_actualRegion.h });
				}

				nextY += pChild->m_actualRegion.h;
			}
		}
	}

	void UIElement::OnDestroy()
	{
		for (auto* pChild : m_children)
		{
			pChild->OnDestroy();

			EXELIUS_DELETE(pChild);
			pChild = nullptr;
		}

		m_children.clear();
	}
}