#pragma once

#include "Source/Debug/LogBase.h"

// Client Log Macros
#ifdef EXE_DEBUG
	/// <summary>
	/// The clients trace log.
	/// </summary>
	#define EXE_TRACE(...)		::Exelius::_Log::GetClientLogger()->trace(__VA_ARGS__)

	/// <summary>
	/// The clients info log.
	/// </summary>
	#define EXE_INFO(...)		::Exelius::_Log::GetClientLogger()->info(__VA_ARGS__)

	/// <summary>
	/// The clients warn log.
	/// </summary>
	#define EXE_WARN(...)		::Exelius::_Log::GetClientLogger()->warn(__VA_ARGS__)
#else
	/// <summary>
	/// The clients trace log.
	/// </summary>
	#define EXE_TRACE(...)
	
	/// <summary>
	/// The clients info log.
	/// </summary>
	#define EXE_INFO(...)
	
	/// <summary>
	/// The clients warn log.
	/// </summary>
	#define EXE_WARN(...)
#endif // EXE_DEBUG

#ifndef EXE_RELEASE
	/// <summary>
	/// The clients error log.
	/// </summary>
	#define EXE_ERROR(...)		::Exelius::_Log::GetClientLogger()->error(__VA_ARGS__)
#else
	/// <summary>
	/// The clients error log.
	/// </summary>
	#define EXE_ERROR(...)
#endif // !EXE_RELEASE

/// <summary>
/// The clients fatal log.
/// </summary>
#define EXE_FATAL(...)			::Exelius::_Log::GetClientLogger()->critical(__VA_ARGS__)