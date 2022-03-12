#pragma once

#if EXELIUS_RENDERER == SFML_RENDERER
	/// <summary>
	/// Forward declares a class of an SFML type.
	/// </summary>
	#define FORWARD_DECLARE(CLASSNAME) class SFML##CLASSNAME; template<class Impl##CLASSNAME> class _##CLASSNAME; using CLASSNAME = _##CLASSNAME<SFML##CLASSNAME>

#elif EXELIUS_RENDERER == OPENGL_RENDERER
	/// <summary>
	/// Forward declares a class of an OpenGL type.
	/// </summary>
	#define FORWARD_DECLARE(CLASSNAME) class OpenGL##CLASSNAME; template<class Impl##CLASSNAME> class _##CLASSNAME; using CLASSNAME = _##CLASSNAME<OpenGL##CLASSNAME>
#else
	#error "Unknown Render Skin Implementation."
#endif