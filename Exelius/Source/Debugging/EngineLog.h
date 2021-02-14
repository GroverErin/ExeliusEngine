#pragma once

#include "Source/Debugging/LogBase.h"

//Engine Log Macros
#ifdef EXE_DEBUG
	/// <summary>
	/// Exelius' engine trace log. This is white by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_ENGINE_TRACE(...)	::Exelius::_Log::GetEngineLogger()->trace(__VA_ARGS__)

	/// <summary>
	/// Exelius' engine trace log. This is green by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_ENGINE_INFO(...)	::Exelius::_Log::GetEngineLogger()->info(__VA_ARGS__)

	/// <summary>
	/// Exelius' engine trace log. This is yellow by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_ENGINE_WARN(...)	::Exelius::_Log::GetEngineLogger()->warn(__VA_ARGS__)
#else
	/// <summary>
	/// Exelius' engine trace log. This is white by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_ENGINE_TRACE(...)
	
	/// <summary>
	/// Exelius' engine trace log. This is green by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_ENGINE_INFO(...)
	
	/// <summary>
	/// Exelius' engine trace log. This is yellow by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_ENGINE_WARN(...)
#endif // EXE_DEBUG

#ifndef EXE_RELEASE
	/// <summary>
	/// Exelius' engine trace log. This is red by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_ENGINE_ERROR(...)	::Exelius::_Log::GetEngineLogger()->error(__VA_ARGS__)
#else
	/// <summary>
	/// Exelius' engine trace log. This is red by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_ENGINE_ERROR(...)
#endif // !EXE_RELEASE

/// <summary>
/// Exelius' engine trace log. This is white with a red highlight by default.
/// Outputs to a file in the ProjectDir/Logs/ directory.
/// </summary>
#define EXELOG_ENGINE_FATAL(...)		::Exelius::_Log::GetEngineLogger()->critical(__VA_ARGS__)