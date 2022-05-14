#pragma once
#include "source/precompilation/EXEPCH.h"
#include "source/engine/Application.h"

#ifdef LINUX
	#include <X11/Xlib.h>
#endif

extern Exelius::Application* Exelius::CreateApplication();

namespace Exelius
{
	int Main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
	{
#ifdef LINUX
		XInitThreads();
#endif
		Exelius::Application::SetSingleton(Exelius::CreateApplication());
		auto* pApp = Exelius::Application::GetInstance();
		EXE_ASSERT(pApp);

		if (!pApp->PreInitializeExelius())
			return 1;
		if (!pApp->InitializeExelius())
			return 1;

		pApp->Run();

		pApp->DestroySingleton();

		return 0;
	}
}

//#ifdef EXE_RELEASE
//	#ifdef EXE_WINDOWS
//		#include <Windows.h>
//
//		int APIENTRY WinMain([[maybe_unused]] HINSTANCE hInst, [[maybe_unused]] HINSTANCE hInstPrevious, [[maybe_unused]] PSTR cmdline, [[maybe_unused]] int cmdshow)
//		{
//			return Exelius::Main(__argc, __argv);
//		}
//	#endif
//#else
	int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
	{
		return Exelius::Main(argc, argv);
	}
//#endif // EXE_RELEASE


/// <summary>
/// Generates a class that acts as the hook into the engine.
/// The client application is required to define this macro
/// once in order to spin up the engine and gain access to
/// the Initialize, Update, and Shutdown functions.
/// 
/// A list of member object-name pairs can be passed into
/// the second argument, with a minimum requirement of 1.
/// This enables the client to pass in their game class.
/// 
/// @code{.cpp}
/// #include "Game.h"
/// EXELIUS_MAIN(ClientAppSubclass, Game m_game);
/// @endcode
/// 
/// or
/// 
/// @code{.cpp}
/// #include "Game.h"
/// EXELIUS_MAIN(ClientAppSubclass, Game m_game; int m_myInt);
/// @endcode
/// 
/// @note
/// In the latter example the parameters are separated by
/// a semi-colon ';' instead of a comma ','. There is also NOT
/// a final semi-colon ';' proceding m_myInt. It is important
/// to understand what this macro will expand to when using this
/// notation.
/// 
/// @todo
/// I'm not sure I am a fan of the client needing to
/// pass in a parameter object.
/// </summary>
/// <param name="USER_APP_NAME">- The name of the Application subclass.</param>
/// <param name="EXELIUS_USER_PARAMS">- A member variable object and name pair.</param>
#define EXELIUS_MAIN(USER_APP_NAME, EXELIUS_USER_PARAMS)\
class USER_APP_NAME final\
	: public Exelius::Application\
{\
	EXELIUS_USER_PARAMS;\
public:\
	virtual ~USER_APP_NAME()\
	{\
		Shutdown();\
	}\
	virtual bool Initialize() final override;\
	virtual void Update() final override;\
	virtual void SetMessageFactory() final override;\
	void Shutdown();\
};\
Exelius::Application* Exelius::CreateApplication() { return new USER_APP_NAME(); }\
void _()
