#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Key code used by Exelius. Default type is unsigned char.
	/// </summary>
	enum class KeyCode : char
	{
		Unknown = -1,		// Unhandled key
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,

		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		Escape,
		Space,
		Quote,		// The " key
		LBracket,		// The [ key
		RBracket,		// The ] key

		LControl,
		LShift,
		LAlt,
		LSystem,			// The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
		RControl,
		RShift,
		RAlt,
		RSystem,			// The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
		Menu,

		Tab,
		Backspace,
		Enter,		// The Enter/Return keys
		Comma,		// The , key
		Period,		// The . key
		Slash,		// The / key
		Backslash,	// The \ key
		Tilde,		// The ~ key
		Equal,		// The = key
		Hyphen,		// The - key
		Semicolon,	// The ; key

		PageUp,				// The Page up key
		PageDown,			// The Page down key

		End,				// The End key
		Home,				// The Home key
		Insert,				// The Insert key
		Delete,       // The Delete key

		Divide,		// The numpad / key
		Multiply,		// The numpad * key
		Add,          // The numpad + key
		Subtract,     // The numpad - key

		Left,				// Left arrow
		Right,				// Right arrow
		Up,					// Up arrow
		Down,				// Down arrow

		Numpad0,			// The numpad 0 key
		Numpad1,			// The numpad 1 key
		Numpad2,			// The numpad 2 key
		Numpad3,			// The numpad 3 key
		Numpad4,			// The numpad 4 key
		Numpad5,			// The numpad 5 key
		Numpad6,			// The numpad 6 key
		Numpad7,			// The numpad 7 key
		Numpad8,			// The numpad 8 key
		Numpad9,			// The numpad 9 key

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		Pause,

		kCount
	};
}