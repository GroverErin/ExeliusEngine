#pragma once

#include "Source/Debugging/LogBase.h"

// Client Log Macros
#ifdef EXE_DEBUG
	/// <summary>
	/// Exelius' clientside trace log. This is white by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_TRACE(...)		::Exelius::_Log::GetClientLogger()->trace(__VA_ARGS__)

	/// <summary>
	/// Exelius' clientside trace log. This is green by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_INFO(...)		::Exelius::_Log::GetClientLogger()->info(__VA_ARGS__)

	/// <summary>
	/// Exelius' clientside trace log. This is yellow by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_WARN(...)		::Exelius::_Log::GetClientLogger()->warn(__VA_ARGS__)
#else
	/// <summary>
	/// Exelius' clientside trace log. This is white by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_TRACE(...)
	
	/// <summary>
	/// Exelius' clientside trace log. This is green by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_INFO(...)
	
	/// <summary>
	/// Exelius' clientside trace log. This is yellow by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_WARN(...)
#endif // EXE_DEBUG

#ifndef EXE_RELEASE
	/// <summary>
	/// Exelius' clientside trace log. This is red by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_ERROR(...)		::Exelius::_Log::GetClientLogger()->error(__VA_ARGS__)
#else
	/// <summary>
	/// Exelius' clientside trace log. This is red by default.
	/// Outputs to a file in the ProjectDir/Logs/ directory.
	/// </summary>
	#define EXELOG_ERROR(...)
#endif // !EXE_RELEASE

/// <summary>
/// Exelius' clientside trace log. This is white with a red highlight by default.
/// Outputs to a file in the ProjectDir/Logs/ directory.
/// </summary>
#define EXELOG_FATAL(...)			::Exelius::_Log::GetClientLogger()->critical(__VA_ARGS__)