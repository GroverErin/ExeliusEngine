#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Mouse code used by Exelius. Default type is unsigned char
	/// </summary>
	enum class MouseCode : unsigned char
	{
		Invalid = 0,
		Left,
		Middle,
		Right,
		kCount
	};

	/// <summary>
	/// Mouse scroll wheel directions used by Exelius.
	/// </summary>
	enum class ScrollWheelDirection : unsigned char
	{
		WheelUp,
		WheelDown,

		WheelNone
	};
}