#include "EXEPCH.h"
#include "OpenGLInputConversions.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	Exelius::KeyCode ConvertKeyCode(int key)
	{
		switch (key)
		{
			case 65:	return KeyCode::A;
			case 66:	return KeyCode::B;
			case 67:	return KeyCode::C;
			case 68:	return KeyCode::D;
			case 69:	return KeyCode::E;
			case 70:	return KeyCode::F;
			case 71:	return KeyCode::G;
			case 72:	return KeyCode::H;
			case 73:	return KeyCode::I;
			case 74:	return KeyCode::J;
			case 75:	return KeyCode::K;
			case 76:	return KeyCode::L;
			case 77:	return KeyCode::M;
			case 78:	return KeyCode::N;
			case 79:	return KeyCode::O;
			case 80:	return KeyCode::P;
			case 81:	return KeyCode::Q;
			case 82:	return KeyCode::R;
			case 83:	return KeyCode::S;
			case 84:	return KeyCode::T;
			case 85:	return KeyCode::U;
			case 86:	return KeyCode::V;
			case 87:	return KeyCode::W;
			case 88:	return KeyCode::X;
			case 89:	return KeyCode::Y;
			case 90:	return KeyCode::Z;
			case 48:	return KeyCode::Num0;
			case 49:	return KeyCode::Num1;
			case 50:	return KeyCode::Num2;
			case 51:	return KeyCode::Num3;
			case 52:	return KeyCode::Num4;
			case 53:	return KeyCode::Num5;
			case 54:	return KeyCode::Num6;
			case 55:	return KeyCode::Num7;
			case 56:	return KeyCode::Num8;
			case 57:	return KeyCode::Num9;
			case 256:	return KeyCode::Escape;
			case 341:	return KeyCode::LControl;
			case 340:	return KeyCode::LShift;
			case 342:	return KeyCode::LAlt;
			case 343:	return KeyCode::LSystem;
			case 345:	return KeyCode::RControl;
			case 344:	return KeyCode::RShift;
			case 346:	return KeyCode::RAlt;
			case 347:	return KeyCode::RSystem;
			case 348:	return KeyCode::Menu;
			case 91:	return KeyCode::LBracket;
			case 93:	return KeyCode::RBracket;
			case 59:	return KeyCode::Semicolon;
			case 44:	return KeyCode::Comma;
			case 46:	return KeyCode::Period;
			case 39:	return KeyCode::Quote;
			case 47:	return KeyCode::Slash;
			case 92:	return KeyCode::Backslash;
			case 96:	return KeyCode::Tilde;
			case 61:	return KeyCode::Equal;
			case 45:	return KeyCode::Hyphen;
			case 32:	return KeyCode::Space;
			case 257:	return KeyCode::Enter;
			case 259:	return KeyCode::Backspace;
			case 258:	return KeyCode::Tab;
			case 266:	return KeyCode::PageUp;
			case 267:	return KeyCode::PageDown;
			case 269:	return KeyCode::End;
			case 268:	return KeyCode::Home;
			case 260:	return KeyCode::Insert;
			case 261:	return KeyCode::Delete;
			case 334:	return KeyCode::Add;
			case 333:	return KeyCode::Subtract;
			case 332:	return KeyCode::Multiply;
			case 331:	return KeyCode::Divide;
			case 263:	return KeyCode::Left;
			case 262:	return KeyCode::Right;
			case 265:	return KeyCode::Up;
			case 264:	return KeyCode::Down;
			case 320:	return KeyCode::Numpad0;
			case 321:	return KeyCode::Numpad0;
			case 322:	return KeyCode::Numpad0;
			case 323:	return KeyCode::Numpad0;
			case 324:	return KeyCode::Numpad0;
			case 325:	return KeyCode::Numpad0;
			case 326:	return KeyCode::Numpad0;
			case 327:	return KeyCode::Numpad0;
			case 328:	return KeyCode::Numpad0;
			case 329:	return KeyCode::Numpad0;
			case 290:	return KeyCode::F1;
			case 291:	return KeyCode::F2;
			case 292:	return KeyCode::F3;
			case 293:	return KeyCode::F3;
			case 294:	return KeyCode::F3;
			case 295:	return KeyCode::F3;
			case 296:	return KeyCode::F3;
			case 297:	return KeyCode::F3;
			case 298:	return KeyCode::F3;
			case 299:	return KeyCode::F3;
			case 300:	return KeyCode::F3;
			case 301:	return KeyCode::F3;
			case 302:	return KeyCode::F3;
			case 303:	return KeyCode::F3;
			case 304:	return KeyCode::F3;
			case 284:	return KeyCode::Pause;
		}

		return KeyCode::Unknown;
	}

	Exelius::MouseCode ConvertMouseCode(int button)
	{
		switch (button)
		{
			case 0:		return MouseCode::Left;
			case 2:		return MouseCode::Middle;
			case 1:		return MouseCode::Right;
		}

		return MouseCode::Invalid;
	}
}
