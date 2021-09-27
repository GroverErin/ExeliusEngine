#pragma once

#include "source/utility/containers/Vector2.h"
#include "EASTL/string.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	static constexpr float PI = 3.1415927f;

	/// <summary>
	/// Linear Interpolation.
	/// </summary>
	/// <param name="min">Minimum value (Inclusive).</param>
	/// <param name="max">Maximum value (Inclusive).</param>
	/// <param name="weight">Weight of interpolation.</param>
	/// <returns>Interpolated value.</returns>
	static constexpr float Lerp(float min, float max, float weight) {
		return (1.0f - weight) * min + weight * max;
	}

	/// <summary>
	/// Smoothing function.
	/// </summary>
	/// <param name="x">Value to be smoothed.</param>
	/// <returns>Smoothed value.</returns>
	static constexpr float SmootherStep(float x)
	{
		float newX = x * x * x * (x * (x * 6 - 15) + 10);
		if (newX >= 1.0f)
			return 1.0f;
		else if (newX <= 0.0f)
			return 0.0f;
		return newX;
	}

	/// <summary>
	/// Force a value to fall between the given range.
	/// </summary>
	/// <param name="x">Value to normalize.</param>
	/// <param name="min">Minimum range.</param>
	/// <param name="max">Maximum range.</param>
	/// <returns>Normalized value.</returns>
	static constexpr float Normalize(float x, float min, float max)
	{
		return (x - min) / (max - min);
	}

	/// <summary>
	/// TODO: Verify this code and comment it.
	/// </summary>
	/// <param name="pos1"></param>
	/// <param name="pos2"></param>
	/// <returns></returns>
	static inline float SquareDistance(const Vector2f& pos1, const Vector2f& pos2)
	{
		return (pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y);
	}

	/// <summary>
	/// TODO: Verify this code and comment it.
	/// </summary>
	/// <param name="pos1"></param>
	/// <param name="pos2"></param>
	/// <returns></returns>
	static inline float Distance(const Vector2f& pos1, const Vector2f& pos2)
	{
		return std::sqrtf(SquareDistance(pos1, pos2));
	}

	// TODO:
	//	These hashing functions need work:
	//		Static error checking for invalid types.
	//		Write FNV-1a Hash instead of eastl::hash. <- Important due to how std and eastl handles the prime number sizes.
	//		Properly handle <32 bit sizes.

	template <class ReturnType>
	static inline ReturnType HashString(const eastl::string& pName)
	{
		eastl::hash<eastl::string> hasher;
		return static_cast<ReturnType>(hasher(pName));
	}

	template <class ReturnType>
	static inline ReturnType HashString(const char* pName)
	{
		return HashString<ReturnType>(eastl::string(pName));
	}
}