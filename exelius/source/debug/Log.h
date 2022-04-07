#pragma once
#include "source/utility/generic/Macros.h"
#include "source/utility/string/StringIntern.h"

#include <spdlog/spdlog.h> // TODO: Figure out a way to remove this as this will likely become a public facing header.

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// The Exelius Engine Logging class. This acts as a scoped log handle.
	/// This interface will obtain a log with a given name or create one if
	/// that log name does not currently exist.
	/// 
	/// Logs are not destroyed until the logging system is destroyed by the
	/// engine, so the user must take care not to instantiate too many logs.
	/// This also means that logs can be created in any scope, and reused in
	/// any other scope.
	/// @todo Maybe limit logs or allow destruction?
	/// 
	/// Logs ARE thread safe, so the same logs can be used across multiple
	/// threads with no problems.
	/// 
	/// The default log category in "Exelius" and will be used if a name
	/// is not provided via the Log constructor. This will use the settings
	/// applied to the "Exelius" log, so if the config file overwrote
	/// these settings then they will be applied globally.
	/// 
	/// When a log is *created* using this interface the output will always
	/// default to the Console only, with a logging level of Trace. This is
	/// intentional, as it is expected that the client application will make
	/// use of the configuration file to define logs that the client app
	/// will make heavy use of. This allows the client to manage logging
	/// performance costs and enable the ability to turn on or off different
	/// categories of logging or change the various levels.
	/// 
	/// It is expected that release builds will only output Error and Fatal
	/// level logs to a file on release mode. This allows logging to be free
	/// in release builds.
	/// 
	/// Logs can output messages in a Python-like API from the library 'fmt'
	/// For formatting @see: https://fmt.dev/latest/syntax.html
	/// 
	/// @see Logging to understand how to add/change logs via engine_config.
	/// </summary>
	class Log
	{
		/// <summary>
		/// The name of the log or the "category".
		/// </summary>
		StringIntern m_logName;

		/// <summary>
		/// The log retrieved on construction.
		/// </summary>
		const std::shared_ptr<spdlog::logger> m_pLog;
	public:
		/// <summary>
		/// Instantiate a log handle with an optional name.
		/// If no name is given, the default log will be retrieved.
		/// 
		/// The log will be retrieved from the LogManager here, and if the
		/// log does not exist, it will be created.
		/// </summary>
		/// <param name="logName">- The optional name of the log to instantiate. Default is "Exelius".</param>
		Log(StringIntern logName = "Exelius");
		Log(const Log&) = delete;
		Log(Log&&) = default;
		Log& operator=(const Log&) = delete;
		Log& operator=(Log&&) = default;

		/// <summary>
		/// The actual logs are not destroyed when this destructor is called,
		/// as this class only acts as a handle to the real log.
		/// </summary>
		~Log() = default;

		/// <summary>
		/// Log a given message at the Trace Level, the lowest level.
		/// The color of the console output text is White.
		/// 
		/// Does nothing if the log has a logging level higher than this
		/// category.
		/// 
		/// For formatting syntax see: https://fmt.dev/latest/syntax.html
		/// 
		/// @code{.cpp}
		/// Exelius::Log log;
		/// log.Trace("Test: {}", 12.34);
		/// @endcode
		/// 
		/// This log level might be best used in cases where you wish to follow
		/// the execution of the application, like logging on entry and exit of
		/// a function. This can be extremely detrimental to framerate, but can
		/// easily be enabled or disabled when used in conjunction with the
		/// configuration file engine_config.ini.
		/// 
		/// Trace is disabled in Release builds.
		/// @todo The log will still be created and retrieved in release builds.
		/// </summary>
		/// <param name="...args">- The message to log.</param>
		template<typename... Args>
		void Trace(Args&&...args) const
		{
			EXE_ASSERT(m_pLog);
			m_pLog->trace(std::forward<Args>(args)...);
		}

		/// <summary>
		/// Log a given message at the Info Level, the second lowest level.
		/// The color of the console output text is Green.
		/// 
		/// Does nothing if the log has a logging level higher than this
		/// category.
		/// 
		/// For formatting syntax see: https://fmt.dev/latest/syntax.html
		/// 
		/// @code{.cpp}
		/// Exelius::Log log;
		/// log.Info("Test: {}", 12.34);
		/// @endcode
		/// 
		/// A use for this log level might be to print notes or debug text
		/// such as "Pressed Fire Key!".
		/// 
		/// Info is disabled in Release builds.
		/// @todo The log will still be created and retrieved in release builds.
		/// </summary>
		/// <param name="...args">- The message to log.</param>
		template<typename... Args>
		void Info(Args&&...args) const
		{
			EXE_ASSERT(m_pLog);
			m_pLog->info(std::forward<Args>(args)...);
		}

		/// <summary>
		/// Log a given message at the Warning Level, the mid-level.
		/// The color of the console output text is Yellow.
		/// 
		/// Does nothing if the log has a logging level higher than this
		/// category.
		/// 
		/// For formatting syntax see: https://fmt.dev/latest/syntax.html
		/// 
		/// @code{.cpp}
		/// Exelius::Log log;
		/// log.Warn("Test: {}", 12.34);
		/// @endcode
		/// 
		/// This log level might be best used in cases where something should
		/// not occur but the application can still continue without further
		/// error.
		/// 
		/// Warn is disabled in Release builds.
		/// @todo The log will still be created and retrieved in release builds.
		/// </summary>
		/// <param name="...args">- The message to log.</param>
		template<typename... Args>
		void Warn(Args&&...args) const
		{
			EXE_ASSERT(m_pLog);
			m_pLog->warn(std::forward<Args>(args)...);
		}

		/// <summary>
		/// Log a given message at the Fatal Level, the second-to-highest level.
		/// The color of the console output text is Red.
		/// 
		/// Error Logs will always output and cannot be disabled.
		/// 
		/// For formatting syntax see: https://fmt.dev/latest/syntax.html
		/// 
		/// @code{.cpp}
		/// Exelius::Log log;
		/// log.Error("Test: {}", 12.34);
		/// @endcode
		/// 
		/// This log level might be best used in cases where something should
		/// not occur but the application can still fail gracefully.
		/// 
		/// Error is enabled in Release builds.
		/// </summary>
		/// <param name="...args">- The message to log.</param>
		template<typename... Args>
		void Error(Args&&...args) const
		{
			EXE_ASSERT(m_pLog);
			m_pLog->error(std::forward<Args>(args)...);
		}

		/// <summary>
		/// Log a given message at the Fatal Level, the highest level.
		/// The color of the console output text is Highlighted-Red.
		/// 
		/// Fatal Logs will always output and cannot be disabled.
		/// 
		/// For formatting syntax see: https://fmt.dev/latest/syntax.html
		/// 
		/// @code{.cpp}
		/// Exelius::Log log;
		/// log.Fatal("Test: {}", 12.34);
		/// @endcode
		/// 
		/// This log level might be best used in cases where something cannot
		/// happen, it should be used in conjuction with an Assertion.
		/// @todo Maybe Force Assertion message when Fatal triggers?
		/// 
		/// Fatal is enabled in Release builds.
		/// </summary>
		/// <param name="...args">- The message to log.</param>
		template<typename... Args>
		void Fatal(Args&&...args) const
		{
			EXE_ASSERT(m_pLog);
			m_pLog->critical(std::forward<Args>(args)...);
		}

	private:
		/// <summary>
		/// Obtains the log from the LogManager.
		/// If the log does not yet exist, it will create one.
		/// </summary>
		/// <returns>- A scoped pointer to the requested log.</returns>
		const std::shared_ptr<spdlog::logger> GetOrCreateLog() const;
	};
}

#define EXE_LOG_TRACE(...) {::Exelius::Log log; log.Trace(__VA_ARGS__);}
#define EXE_LOG_CATEGORY_TRACE(CATEGORY, ...) {::Exelius::Log log(CATEGORY); log.Trace(__VA_ARGS__);}

#define EXE_LOG_INFO(...) {::Exelius::Log log; log.Info(__VA_ARGS__);}
#define EXE_LOG_CATEGORY_INFO(CATEGORY, ...) {::Exelius::Log log(CATEGORY); log.Info(__VA_ARGS__);}

#define EXE_LOG_WARN(...) {::Exelius::Log log; log.Warn(__VA_ARGS__);}
#define EXE_LOG_CATEGORY_WARN(CATEGORY, ...) {::Exelius::Log log(CATEGORY); log.Warn(__VA_ARGS__);}

#define EXE_LOG_ERROR(...) {::Exelius::Log log; log.Error(__VA_ARGS__);}
#define EXE_LOG_CATEGORY_ERROR(CATEGORY, ...) {::Exelius::Log log(CATEGORY); log.Error(__VA_ARGS__);}

#define EXE_LOG_FATAL(...) {::Exelius::Log log; log.Fatal(__VA_ARGS__);}
#define EXE_LOG_CATEGORY_FATAL(CATEGORY, ...) {::Exelius::Log log(CATEGORY); log.Fatal(__VA_ARGS__);}