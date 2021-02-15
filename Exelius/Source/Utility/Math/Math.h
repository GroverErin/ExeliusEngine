#pragma once

#include "Source/Utility/Containers/Vector2.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
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
	static constexpr inline float Lerp(float min, float max, float weight) {
		return (1.0f - weight) * min + weight * max;
	}

	/// <summary>
	/// Smoothing function.
	/// </summary>
	/// <param name="x">Value to be smoothed.</param>
	/// <returns>Smoothed value.</returns>
	static constexpr inline float SmootherStep(float x)
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
	static constexpr inline float Normalize(float x, float min, float max)
	{
		return (x - min) / (max - min);
	}

	/// <summary>
	/// TODO: Verify this code and comment it.
	/// </summary>
	/// <param name="pos1"></param>
	/// <param name="pos2"></param>
	/// <returns></returns>
	static constexpr inline float SquareDistance(const Vector2f& pos1, const Vector2f& pos2)
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
}