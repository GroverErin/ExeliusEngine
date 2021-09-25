#pragma once
#include <cmath>
#include <EASTL/numeric_limits.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Utility template class for manipulating
	/// 2-dimensional vectors
	/// 
	/// IMPORTANT NOTE:
	///		Operator overloads from SFML.
	/// </summary>
	template <class NumberType>
	class Vector2
	{
		static_assert(eastl::is_integral_v<NumberType> || eastl::is_floating_point_v<NumberType>, "Type must be of integral number type.");

	public:
		/// <summary>
		/// This slightly complex looking union allows vectors to use either x,y or w,h names
		/// and they will have the same values.
		/// </summary>
		#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
		union
		{
			struct
			{
				/// <summary>
				/// Value in the first position of the 'pair'.
				/// this->x == this->w
				/// </summary>
				NumberType x;

				/// <summary>
				/// Value in the second position of the 'pair'.
				/// this->y == this->h
				/// </summary>
				NumberType y;
			};

			struct
			{
				/// <summary>
				/// Value in the first position of the 'pair'.
				/// this->x == this->w
				/// </summary>
				NumberType w;

				/// <summary>
				/// Value in the second position of the 'pair'.
				/// this->y == this->h
				/// </summary>
				NumberType h;
			};
		};

		/// <summary>
		/// Creates a Vector2(0, 0).
		/// </summary>
		Vector2()
			: x(0)
			, y(0)
		{
			//
		}

		/// <summary>
		/// Construct the vector from its coordinates
		/// </summary>
		/// <param name="X">X coordinate</param>
		/// <param name="Y">Y coordinate</param>
		Vector2(NumberType X, NumberType Y)
			: x(X)
			, y(Y)
		{

		}

		/// <summary>
		/// Construct the vector from another type of vector
		///
		/// NOTE:
		///		This constructor doesn't replace the copy constructor,
		///		it's called only when VectorType != NumberType.
		///		A call to this constructor will fail to compile if VectorType
		///		is not convertible to NumberType.
		/// </summary>
		/// <param name="vector">Vector to convert</param>
		template <typename VectorType>
		explicit Vector2(const Vector2<VectorType>& vector)
			: x(static_cast<NumberType>(vector.x))
			, y(static_cast<NumberType>(vector.y))
		{
			//
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
			return std::sqrt(GetSquareLength());
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

			float length = std::sqrt(GetSquareLength());
			return { x / length, y / length };
		}
	};
	
	/// <summary>
	/// Overload of unary operator -
	/// </summary>
	/// <param name="right">Vector to negate</param>
	/// <returns>Memberwise opposite of the vector</returns>
	template <typename NumberType>
	inline Vector2<NumberType> operator -(const Vector2<NumberType>& right)
	{
		return Vector2<NumberType>(-right.x, -right.y);
	}
	
	/// <summary>
	/// Overload of binary operator +=
	/// 
	/// This operator performs a memberwise addition of both vectors,
	/// and assigns the result to left.
	/// </summary>
	/// <param name="left">Left operand (a vector)</param>
	/// <param name="right">Right operand (a vector)</param>
	/// <returns>Reference to left</returns>
	template <typename NumberType>
	inline Vector2<NumberType>& operator +=(Vector2<NumberType>& left, const Vector2<NumberType>& right)
	{
		left.x += right.x;
		left.y += right.y;
		return left;
	}
	
	/// <summary>
	/// Overload of binary operator -=
	///
	/// This operator performs a memberwise subtraction of both vectors,
	/// and assigns the result to left.
	/// </summary>
	/// <param name="left">Left operand (a vector)</param>
	/// <param name="right">Right operand (a vector)</param>
	/// <returns>Reference to left</returns>
	template <typename NumberType>
	inline Vector2<NumberType>& operator -=(Vector2<NumberType>& left, const Vector2<NumberType>& right)
	{
		left.x -= right.x;
		left.y -= right.y;
		return left;
	}
	
	/// <summary>
	/// Overload of binary operator +
	/// </summary>
	/// <param name="left">Left operand (a vector)</param>
	/// <param name="right">Right operand (a vector)</param>
	/// <returns>Memberwise addition of both vectors</returns>
	template <typename NumberType>
	inline Vector2<NumberType> operator +(const Vector2<NumberType>& left, const Vector2<NumberType>& right)
	{
		return Vector2<NumberType>(left.x + right.x, left.y + right.y);
	}
	
	/// <summary>
	/// Overload of binary operator -
	/// </summary>
	/// <param name="left">Left operand (a vector)</param>
	/// <param name="right">Right operand (a vector)</param>
	/// <returns>Memberwise subtraction of both vectors</returns>
	template <typename NumberType>
	inline Vector2<NumberType> operator -(const Vector2<NumberType>& left, const Vector2<NumberType>& right)
	{
		return Vector2<NumberType>(left.x - right.x, left.y - right.y);
	}
	
	/// <summary>
	/// Overload of binary operator *
	/// </summary>
	/// <param name="left">Left operand (a vector)</param>
	/// <param name="right">Right operand (a scalar value)</param>
	/// <returns>Memberwise multiplication by right</returns>
	template <typename NumberType>
	inline Vector2<NumberType> operator *(const Vector2<NumberType>& left, NumberType right)
	{
		return Vector2<NumberType>(left.x * right, left.y * right);
	}
	
	/// <summary>
	/// Overload of binary operator *
	/// </summary>
	/// <param name="left">Left operand (a scalar value)</param>
	/// <param name="right">Right operand (a vector)</param>
	/// <returns>Memberwise multiplication by left</returns>
	template <typename NumberType>
	inline Vector2<NumberType> operator *(NumberType left, const Vector2<NumberType>& right)
	{
		return Vector2<NumberType>(right.x * left, right.y * left);
	}
	
	/// <summary>
	/// Overload of binary operator *=
	///
	/// This operator performs a memberwise multiplication by right,
	/// and assigns the result to left.
	/// </summary>
	/// <param name="left">Left operand (a vector)</param>
	/// <param name="right">Right operand (a scalar value)</param>
	/// <returns>Reference to left</returns>
	template <typename NumberType>
	inline Vector2<NumberType>& operator *=(Vector2<NumberType>& left, NumberType right)
	{
		left.x *= right;
		left.y *= right;

		return left;
	}
	
	/// <summary>
	/// Overload of binary operator /
	/// </summary>
	/// <param name="left">Left operand (a vector)</param>
	/// <param name="right">Right operand (a scalar value)</param>
	/// <returns>Memberwise division by right</returns>
	template <typename NumberType>
	inline Vector2<NumberType> operator /(const Vector2<NumberType>& left, NumberType right)
	{
		return Vector2<NumberType>(left.x / right, left.y / right);
	}
	
	/// <summary>
	/// Overload of binary operator /=
	///
	/// This operator performs a memberwise division by right,
	/// and assigns the result to left.
	/// </summary>
	/// <param name="left">Left operand (a vector)</param>
	/// <param name="right">Right operand (a scalar value)</param>
	/// <returns>Reference to left</returns>
	template <typename NumberType>
	inline Vector2<NumberType>& operator /=(Vector2<NumberType>& left, NumberType right)
	{
		left.x /= right;
		left.y /= right;

		return left;
	}
	
	/// <summary>
	/// Overload of binary operator ==
	///
	/// This operator compares strict equality between two vectors.
	/// </summary>
	/// <param name="left">Left operand (a vector)</param>
	/// <param name="right">Right operand (a vector)</param>
	/// <returns>True if left is equal to right</returns>
	template <typename NumberType>
	inline bool operator ==(const Vector2<NumberType>& left, const Vector2<NumberType>& right)
	{
		return (left.x == right.x) && (left.y == right.y);
	}
	
	/// <summary>
	/// Overload of binary operator !=
	///
	/// This operator compares strict difference between two vectors.
	/// </summary>
	/// <param name="left">Left operand (a vector)</param>
	/// <param name="right">Right operand (a vector)</param>
	/// <returns>True if left is not equal to right</returns>
	template <typename NumberType>
	inline bool operator !=(const Vector2<NumberType>& left, const Vector2<NumberType>& right)
	{
		return (left.x != right.x) || (left.y != right.y);
	}

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