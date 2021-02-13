#pragma once

#include "Source/Debug/LogBase.h"

//Engine Log Macros
#ifdef EXE_DEBUG
	/// <summary>
	/// The engines trace log.
	/// </summary>
	#define EXE_ENGINE_TRACE(...)	::Exelius::_Log::GetEngineLogger()->trace(__VA_ARGS__)

	/// <summary>
	/// The engines info log.
	/// </summary>
	#define EXE_ENGINE_INFO(...)	::Exelius::_Log::GetEngineLogger()->info(__VA_ARGS__)

	/// <summary>
	/// The engines warn log.
	/// </summary>
	#define EXE_ENGINE_WARN(...)	::Exelius::_Log::GetEngineLogger()->warn(__VA_ARGS__)
#else
	/// <summary>
	/// The engines trace log.
	/// </summary>
	#define EXE_ENGINE_TRACE(...)
	
	/// <summary>
	/// The engines info log.
	/// </summary>
	#define EXE_ENGINE_INFO(...)
	
	/// <summary>
	/// The engines warn log.
	/// </summary>
	#define EXE_ENGINE_WARN(...)
#endif // EXE_DEBUG

#ifndef EXE_RELEASE
	/// <summary>
	/// The engines error log.
	/// </summary>
	#define EXE_ENGINE_ERROR(...)	::Exelius::_Log::GetEngineLogger()->error(__VA_ARGS__)
#else
	/// <summary>
	/// The engines error log.
	/// </summary>
	#define EXE_ENGINE_ERROR(...)
#endif // !EXE_RELEASE

/// <summary>
/// The engines fatal log.
/// </summary>
#define EXE_ENGINE_FATAL(...)		::Exelius::_Log::GetEngineLogger()->critical(__VA_ARGS__)