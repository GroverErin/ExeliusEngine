#pragma once
#include <cmath>
#include <EASTL/numeric_limits.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Container used primarily for 2D position data.
	/// </summary>
	template <class NumberType>
	class Vector2
	{
		static_assert(eastl::is_integral_v<NumberType> || eastl::is_floating_point_v<NumberType>, "Type must be of integral number type.");

	public:
		NumberType x;
		NumberType y;

		/// <summary>
		/// Multiplies vector by a one-dimensional scalar.
		/// </summary>
		template<class ScalarType>
		Vector2 operator*(ScalarType right)
		{
			static_assert(eastl::is_integral_v<ScalarType> || eastl::is_floating_point_v<ScalarType>, "Type must be of integral number type.");
			return Vector2{ x * right, y * right };
		}

		/// <summary>
		/// Calculates a vector passing from A to B.
		/// </summary>
		/// <param name="b">The other Vector2.</param>
		/// <returns>Result.</returns>
		Vector2 operator-(Vector2 b)
		{
			return Vector2{ this->x - b.x, this->y - b.y };
		}

		/// <summary>
		/// Calculate the product.
		/// </summary>
		/// <param name="right">The other Vector2.</param>
		/// <returns>Product.</returns>
		Vector2<NumberType>& operator*=(const Vector2<NumberType>& right)
		{
			x *= right.x;
			y *= right.y;
			return *this;
		}

		/// <summary>
		/// Calculates the resultant of A and B.
		/// </summary>
		/// <param name="b">The other Vector2.</param>
		/// <returns>Result.</returns>
		Vector2 operator+(const Vector2& b)
		{
			return Vector2{ this->x + b.x, this->y + b.y };
		}

		/// <summary>
		/// Calculates the resultant of A and B.
		/// </summary>
		/// <param name="b">The other Vector2.</param>
		/// <returns>Result.</returns>
		Vector2 operator/(float right)
		{
			return Vector2{ this->x / right, this->y / right };
		}

		/// <summary>
		/// Check if this vector matches another.
		/// </summary>
		/// <param name="b">The other Vector2.</param>
		/// <returns>Result.</returns>
		bool operator==(const Vector2& right)
		{
			return (this->x == right.x && this->y == right.y);
		}

		/// <summary>
		/// Check if this vector does not match another.
		/// </summary>
		/// <param name="b">The other Vector2.</param>
		/// <returns>Result.</returns>
		bool operator!=(const Vector2& right)
		{
			return !(this->x == right.x && this->y == right.y);
		}

		/// <summary>
		/// Calculate the dot product between this vector and the passed in vector.
		/// </summary>
		/// <param name="b">The other Vector2.</param>
		/// <returns>Result.</returns>
		NumberType Dot(const Vector2& b)
		{
			return this->x * b.x + this->y * b.y;
		}

		/// <summary>
		/// Calculate the determinent between this vector and the passed in vector.
		/// </summary>
		/// <param name="b">The other Vector2.</param>
		/// <returns>Result.</returns>
		NumberType Determinent(const Vector2& b)
		{
			return this->x * b.y - this->y * b.x;
		}

		/// <summary>
		/// Returns the squared length of this vector.
		/// </summary>
		/// <returns>Square length</returns>
		NumberType GetSquareLength()
		{
			return (x * x) + (y * y);
		}
		
		/// <summary>
		/// Returns the length of this vector.
		/// </summary>
		/// <returns>Float length.</returns>
		float Length()
		{
			return eastl::sqrt(GetSquareLength());
		}

		/// <summary>
		/// Return a vector with the same component proportions but length of one.
		/// A Unit vector.
		/// </summary>
		template <class _NumberType = NumberType>
		typename eastl::enable_if_t<eastl::is_floating_point_v<_NumberType>, Vector2<_NumberType>> Normalize()
		{
			if (x == 0 && y == 0)
				return { 0,0 };

			float length = eastl::sqrt(GetSquareLength());
			return { x / length, y / length };
		}
	};

	/// <summary>
	/// Vector2 of float values.
	/// </summary>
	using Vector2f = Vector2<float>;

	/// <summary>
	/// Vector2 of int values.
	/// </summary>
	using Vector2i = Vector2<int>;

	/// <summary>
	/// Vector2 of unsigned int values.
	/// </summary>
	using Vector2u = Vector2<unsigned int>;
}