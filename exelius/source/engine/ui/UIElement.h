#pragma once
#include "source/utility/math/Rectangle.h"
#include "source/utility/generic/Color.h"

#include <rapidjson/document.h>

#include <EASTL/vector.h>
/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class RelativeCoordinate
	{
	public:
		float m_value;

		enum class UnitType
		{
			Percentage,
			Pixels
		};
		UnitType m_type = UnitType::Pixels;

		RelativeCoordinate()
			: m_value(0.0f)
			, m_type(UnitType::Pixels)
		{
			//
		}

		RelativeCoordinate(UnitType type, float value)
			: m_value(value)
			, m_type(type)
		{
			//
		}

		inline float Resolve(float valueToResolve) const
		{
			if (m_type == UnitType::Pixels)
				return m_value;
			else
				return m_value * valueToResolve;
		}
	};

	class UIElement
	{
		eastl::vector<UIElement*> m_children;
	protected:
		UIElement* m_pParent;
		Color m_color;
		enum class LayoutType
		{
			None = 0,
			StackHorizontal = 1,
			StackVertical = 2,
			WrapHorizontal = 3,
			WrapVertical = 4
		};
		LayoutType m_childLayoutType;

		FRectangle m_actualRegion; // Actual position and size.
	public:
		RelativeCoordinate m_relativeRegionX; // Designed X position.
		RelativeCoordinate m_relativeRegionY; // Designed Y position.
		RelativeCoordinate m_relativeRegionW; // Designed width.
		RelativeCoordinate m_relativeRegionH; // Designed height.
		RelativeCoordinate m_relativeOriginX; // Designed Origin X position.
		RelativeCoordinate m_relativeOriginY; // Designed Origin Y position.

		UIElement(UIElement* pParentElement = nullptr);

		virtual ~UIElement() = default;

		virtual bool Initialize(const rapidjson::Value& jsonUIElementData);

		virtual void OnUpdate(const FRectangle& parentRegion);

		virtual void OnRender();

		virtual void OnDestroy();

		virtual void OnMouseEnter() {}

		virtual void OnMouseExit() {}

		UIElement* HitTest(Vector2f mousePosition);

		Color GetColor() const { return m_color; }
		void SetColor(Color colorToSet) { m_color = colorToSet; }

	private:
		RelativeCoordinate ParseCoordinateData(const rapidjson::Value& jsonUIElementData, const char* pCoordinateName);

		void HandleLayoutUpdates();

		void NoLayoutUpdate();

		void StackLayoutUpdate(bool isHorizontal = true);

		void WrapLayoutUpdate(bool isHorizontal = true);
	};
}