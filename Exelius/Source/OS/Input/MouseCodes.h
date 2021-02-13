#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
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
		Right
	};
}