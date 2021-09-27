#pragma once
#include "source/utility/containers/Vector2.h"
#include <algorithm>

// PLEASE SEE 'IMPORTANT NOTE' below.

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    /// <summary>
    /// Utility class for manipulating 2D axis aligned rectangles.
    /// 
    /// IMPORTNANT NOTE:
    ///     This class (Exelius::Rentangle) is a DIRECT COPY
    ///     of the SFML::Rect class. I had one, but SFMLs was better.
    ///     I made the copy in order to abstract the SFML code into my
    ///     engine a little easier.
    /// 
    ///     I made some alterations to the names of things.
    /// </summary>
    template <typename NumberType>
    class Rectangle
    {
        static_assert(eastl::is_integral_v<NumberType> || eastl::is_floating_point_v<NumberType>, "Type must be of integral number type.");

    public:
        NumberType m_left;   // Left coordinate of the rectangle
        NumberType m_top;    // Top coordinate of the rectangle
        NumberType m_width;  // Width of the rectangle
        NumberType m_height; // Height of the rectangle

        /// <summary>
        /// Creates an empty rectangle (it is equivalent to calling
        /// Rect(0, 0, 0, 0)).
        /// </summary>
        Rectangle()
            : m_left(0)
            , m_top(0)
            , m_width(0)
            , m_height(0)
        {
            //
        }
        
        /// <summary>
        /// Construct the rectangle from its coordinates
        ///
        /// NOTE:
        ///     Be careful, the last two parameters are the width
        ///     and height, not the right and bottom coordinates!
        /// </summary>
        /// <param name="rectLeft">Left coordinate of the rectangle</param>
        /// <param name="rectTop">Top coordinate of the rectangle</param>
        /// <param name="rectWidth">Width of the rectangle</param>
        /// <param name="rectHeight">Height of the rectangle</param>
        Rectangle(NumberType rectLeft, NumberType rectTop, NumberType rectWidth, NumberType rectHeight)
            : m_left(rectLeft)
            , m_top(rectTop)
            , m_width(rectWidth)
            , m_height(rectHeight)
        {
            //
        }
        
        /// <summary>
        /// Construct the rectangle from position and size
        ///
        /// NOTE:
        ///     Be careful, the last parameter is the size,
        ///     not the bottom-right corner!
        /// </summary>
        /// <param name="position">Position of the top-left corner of the rectangle</param>
        /// <param name="size">Size of the rectangle</param>
        Rectangle(const Vector2<NumberType>& position, const Vector2<NumberType>& size)
            : m_left(position.x)
            , m_top(position.y)
            , m_width(size.w)
            , m_height(size.h)
        {
            //
        }
        
        /// <summary>
        /// Construct the rectangle from another type of rectangle
        ///
        /// NOTE:
        ///     This constructor doesn't replace the copy constructor,
        ///     it's called only when U != T.
        ///     A call to this constructor will fail to compile if U
        ///     is not convertible to T.
        /// </summary>
        /// <param name="rectangle">Rectangle to convert</param>
        template <typename RectangleType>
        explicit Rectangle(const Rectangle<RectangleType>& rectangle)
            : m_left(static_cast<NumberType>(rectangle.m_left))
            , m_top(static_cast<NumberType>(rectangle.m_top))
            , m_width(static_cast<NumberType>(rectangle.m_width))
            , m_height(static_cast<NumberType>(rectangle.m_height))
        {

        }
        
        /// <summary>
        /// Check if a point is inside the rectangle's area
        ///
        /// NOTE:
        ///     This check is non-inclusive. If the point lies on the
        ///     edge of the rectangle, this function will return false.
        /// </summary>
        /// <param name="x">X coordinate of the point to test</param>
        /// <param name="y">Y coordinate of the point to test</param>
        /// <returns>True if the point is inside, false otherwise</returns>
        bool Contains(NumberType x, NumberType y) const
        {
            // Rectangles with negative dimensions are allowed, so we must handle them correctly

            // Compute the real min and max of the rectangle on both axes
            NumberType minX = std::min(m_left, static_cast<NumberType>(m_left + m_width));
            NumberType maxX = std::max(m_left, static_cast<NumberType>(m_left + m_width));
            NumberType minY = std::min(m_top, static_cast<NumberType>(m_top + m_height));
            NumberType maxY = std::max(m_top, static_cast<NumberType>(m_top + m_height));

            return (x >= minX) && (x < maxX) && (y >= minY) && (y < maxY);
        }
        
        /// <summary>
        /// Check if a point is inside the rectangle's area
        ///
        /// NOTE:
        ///     This check is non-inclusive. If the point lies on the
        ///     edge of the rectangle, this function will return false.
        /// </summary>
        /// <param name="point">Point to test</param>
        /// <returns>True if the point is inside, false otherwise</returns>
        bool Contains(const Vector2<NumberType>& point) const
        {
            return Contains(point.x, point.y);
        }
        
        /// <summary>
        /// Check the intersection between two rectangles
        /// </summary>
        /// <param name="rectangle">Rectangle to test</param>
        /// <returns>True if rectangles overlap, false otherwise</returns>
        bool Intersects(const Rectangle<NumberType>& rectangle) const
        {
            Rectangle<NumberType> intersection;
            return Intersects(rectangle, intersection);
        }
        
        /// <summary>
        /// Check the intersection between two rectangles
        ///
        /// NOTE:
        ///     This overload returns the overlapped rectangle in the
        ///     intersection parameter.
        /// </summary>
        /// <param name="rectangle">Rectangle to test</param>
        /// <param name="intersection">Rectangle to be filled with the intersection</param>
        /// <returns>True if rectangles overlap, false otherwise</returns>
        bool Intersects(const Rectangle<NumberType>& rectangle, Rectangle<NumberType>& intersection) const
        {
            // Rectangles with negative dimensions are allowed, so we must handle them correctly

            // Compute the min and max of the first rectangle on both axes
            NumberType r1MinX = std::min(m_left, static_cast<NumberType>(m_left + m_width));
            NumberType r1MaxX = std::max(m_left, static_cast<NumberType>(m_left + m_width));
            NumberType r1MinY = std::min(m_top, static_cast<NumberType>(m_top + m_height));
            NumberType r1MaxY = std::max(m_top, static_cast<NumberType>(m_top + m_height));

            // Compute the min and max of the second rectangle on both axes
            NumberType r2MinX = std::min(rectangle.m_left, static_cast<NumberType>(rectangle.m_left + rectangle.m_width));
            NumberType r2MaxX = std::max(rectangle.m_left, static_cast<NumberType>(rectangle.m_left + rectangle.m_width));
            NumberType r2MinY = std::min(rectangle.m_top, static_cast<NumberType>(rectangle.m_top + rectangle.m_height));
            NumberType r2MaxY = std::max(rectangle.m_top, static_cast<NumberType>(rectangle.m_top + rectangle.m_height));

            // Compute the intersection boundaries
            NumberType interLeft = std::max(r1MinX, r2MinX);
            NumberType interTop = std::max(r1MinY, r2MinY);
            NumberType interRight = std::min(r1MaxX, r2MaxX);
            NumberType interBottom = std::min(r1MaxY, r2MaxY);

            // If the intersection is valid (positive non zero area), then there is an intersection
            if ((interLeft < interRight) && (interTop < interBottom))
            {
                intersection = Rectangle<NumberType>(interLeft, interTop, interRight - interLeft, interBottom - interTop);
                return true;
            }
            else
            {
                intersection = Rectangle<NumberType>(0, 0, 0, 0);
                return false;
            }
        }
        
        /// <summary>
        /// Get the position of the rectangle's top-left corner
        /// </summary>
        /// <returns>Position of rectangle</returns>
        Vector2<NumberType> GetPosition() const
        {
            return Vector2<NumberType>(m_left, m_top);
        }
        
        /// <summary>
        /// Get the size of the rectangle
        /// </summary>
        /// <returns>Size of rectangle</returns>
        Vector2<NumberType> GetSize() const
        {
            return Vector2<NumberType>(m_width, m_height);
        }
    };
    
    /// <summary>
    /// Overload of binary operator ==
    ///
    /// This operator compares strict equality between two rectangles.
    /// </summary>
    /// <param name="left">Left operand (a rectangle)</param>
    /// <param name="right">Right operand (a rectangle)</param>
    /// <returns>True if left is equal to right</returns>
    template <typename NumberType>
    inline bool operator ==(const Rectangle<NumberType>& left, const Rectangle<NumberType>& right)
    {
        return (left.m_left == right.m_left) && (left.m_width == right.m_width) &&
            (left.m_top == right.m_top) && (left.m_height == right.m_height);
    }
    
    /// <summary>
    /// Overload of binary operator !=
    ///
    /// This operator compares strict difference between two rectangles.
    /// </summary>
    /// <param name="left">Left operand (a rectangle)</param>
    /// <param name="right">Right operand (a rectangle)</param>
    /// <returns>True if left is not equal to right</returns>
    template <typename NumberType>
    inline bool operator !=(const Rectangle<NumberType>& left, const Rectangle<NumberType>& right)
    {
        return !(left == right);
    }

    /// <summary>
    /// Rectangle of float values.
    /// </summary>
    using FRectangle = Rectangle<float>;

    /// <summary>
    /// Rectangle of int values.
    /// </summary>
    using IRectangle = Rectangle<int>;

    /// <summary>
    /// Rectangle of unsigned int values.
    /// </summary>
    using URectangle = Rectangle<unsigned int>;

}