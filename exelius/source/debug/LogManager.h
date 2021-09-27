#pragma once
#include "source/utility/string/StringIntern.h"
#include "source/utility/generic/Singleton.h"

#include <spdlog/spdlog.h>

#include <EASTL/array.h>
#include <EASTL/unordered_map.h>
#include <EASTL/string.h>

#include <rapidjson/document.h>

#include <mutex>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Enum that is used to determine where a log message will output to.
	/// </summary>
	enum class LogLocation
	{
		kConsole		= 0,	/// Enables a log to output to the console.
		kFile			= 1,	/// Enables a log to output to the file.
		kConsoleAndFile	= 2,	/// Enables a log to output to the console and the file.
		kMax					/// Used for bounds checking. Not a valid location.
	};

	/// <summary>
	/// Enum that is used to determine what the lowest priority message that will be logged.
	/// Excluded log messages will simply be ignored and in release builds they will not be
	/// a part of the compiled code and will be "free".
	/// </summary>
	enum class LogLevel
	{
		kFatal	= 0,	/// Sets the log level to Fatal. At this level, all other log levels will be ignored.
		kTrace	= 1,	/// Sets the log level to Trace. At this level all other log levels will be logged as well.
		kInfo	= 2,	/// Sets the log level to Info. At this level Trace logs will be ignored.
		kWarn	= 3,	/// Sets the log level to Warn. At this level Trace and Info logs will be ignored.
		kError	= 4,	/// Sets the log level to Error. At this level Trace, Info and Warn logs will be ignored.
		kMax			/// Used for bounds checking. Not a valid level.
	};

	/// <summary>
	/// The structure containing the data necessary to define a log
	/// that will output to a file.
	/// 
	/// @todo It may be wise to make this polymorphic in case we wish to add further options in the future.
	/// </summary>
	struct FileLogDefinition
	{
		/// <summary>
		/// The pattern or format that will be output to the file.
		/// </summary>
		eastl::string m_pattern;

		/// <summary>
		/// The output directory and filename for the file log.
		/// </summary>
		eastl::string m_outputDirectory;

		/// <summary>
		/// The maximum file size of a log.
		/// </summary>
		unsigned int m_maxFileSize;

		/// <summary>
		/// The maximum number of rotating log files.
		/// </summary>
		unsigned int m_numFiles;

		/// <summary>
		/// Should the log rotate on program startup.
		/// </summary>
		bool m_rotateOnOpen;

		/// <summary>
		/// Construct the definition with reasonable default values.
		/// </summary>
		FileLogDefinition()
			: m_pattern("[%T] [%l] [%n] [%t]: %v")
			, m_outputDirectory("Logs/Exelius.log")
			, m_maxFileSize(1024 * 1024 * 5)
			, m_numFiles(3)
			, m_rotateOnOpen(true)
		{
			//
		}
	};

	/// <summary>
	/// The structure containing the data necessary to define a log
	/// that will output to the console.
	/// 
	/// @todo It may be wise to make this polymorphic in case we wish to add further options in the future.
	/// </summary>
	struct ConsoleLogDefinition
	{
		/// <summary>
		/// The pattern or format that will be output to the console.
		/// </summary>
		eastl::string m_pattern;

		/// <summary>
		/// Construct the definition with reasonable default values.
		/// </summary>
		ConsoleLogDefinition()
			: m_pattern("%^[%T] [%n] [%t]: %v%$")
		{
			//
		}
	};

	/// <summary>
	/// The structure containing the data necessary to create a log.
	/// </summary>
	struct LogData
	{
		/// <summary>
		/// The name of the log, included in the output.
		/// </summary>
		StringIntern m_logName;

		/// <summary>
		/// The location that the log will output; File and/or Console.
		/// </summary>
		LogLocation m_logLocation;

		/// <summary>
		/// The level or priority of the minimum logged message.
		/// </summary>
		LogLevel m_logLevel;

		/// <summary>
		/// Construct the data with reasonable default values.
		/// </summary>
		LogData()
			: m_logName("Exelius")
			, m_logLocation(LogLocation::kConsole)
			, m_logLevel(LogLevel::kTrace)
		{
			//
		}
	};

	/// <summary>
	/// Log Managment class. This is an internal singleton manager that is managed by the engine.
	/// Actual log management should be deferred to the Log class, which acts as a Scoped Handle to a managed log.
	/// This class is responsible for creating log categories from the engine_config.ini file.
	/// Creation of new logs from code (Instantiated purely via code rather than the .ini file) is
	/// also managed here, although the settings for those logs are not changable. It should *always*
	/// be preferable to create a log via the .ini file.
	/// 
	/// @note Not intended for use by client applications.
	/// 
	/// @todo It may be possible that the GetLog and CreateLog functions need to be wrapped in a mutex.
	/// </summary>
	class LogManager
		: public Singleton<LogManager>
	{
		/// <summary>
		/// Contains the File log and the Console Log definition.
		/// </summary>
		eastl::array<spdlog::sink_ptr, 2> m_logDefinitions;

		/// <summary>
		/// Map of log names to logs.
		/// </summary>
		eastl::unordered_map<StringIntern, std::shared_ptr<spdlog::logger>> m_logs;

		/// <summary>
		/// Lock for data the logs map.
		/// </summary>
		std::mutex m_logLock;
	public:
		LogManager() = default;
		LogManager(const LogManager&) = delete;
		LogManager(LogManager&&) = delete;
		LogManager& operator=(const LogManager&) = delete;
		LogManager& operator=(LogManager&&) = delete;

		/// <summary>
		/// Unregister all the loggers and clear the map of logs and log definitions.
		/// </summary>
		~LogManager();

		/// <summary>
		/// Initialize default engine log definitions and default log.
		/// Engine default log is always created but can be overridden by the config file.
		/// </summary>
		/// <returns>True if initialization was successful, false on failure.</returns>
		bool PreInitialize();

		/// <summary>
		/// Initialize logs defined in the config file.
		/// 
		/// This function will NOT fail if *some* data is undefined in the config file, as logs will
		/// be given reasonable default values when undefined.
		/// </summary>
		/// <param name="fileDefinition">- The file definition data retrieved from the config file.</param>
		/// <param name="consoleDefinition">- The console definition data retrieved from the config file.</param>
		/// <param name="logData">- The log data retrieved from the config file.</param>
		/// <returns>True if initialization was successful, false on failure.</returns>
		bool Initialize(const FileLogDefinition& fileDefinition, const ConsoleLogDefinition& consoleDefinition, const eastl::vector<LogData>& logData);

		/// <summary>
		/// Create a log catagory with the given name, log location, and log level.
		/// 
		/// @note
		/// Creating a log with a name that already exists will destroy the previous log,
		/// thus overwriting it.
		/// </summary>
		/// <param name="pLogName">- The name of the log. For example: "Exelius" is the name of the engines default log.</param>
		/// <param name="location">- The location the log will output to. The default is to output only to the console.</param>
		/// <param name="logLevel">- The lowest priority message that will be logged. The default will output all log messages.</param>
		/// <returns>True if the creation succeeds, False if the creation fails.</returns>
		bool CreateLog(const char* pLogName, LogLocation location = LogLocation::kConsole, LogLevel logLevel = LogLevel::kTrace);

		/// <summary>
		/// Retrieve the log with the given name if it exists.
		/// </summary>
		/// <param name="logName">- The name of the log to retrieve. Example: "Exelius" will retrieve the default log.</param>
		/// <returns>The log with the given name if found, nullptr if not found.</returns>
		std::shared_ptr<spdlog::logger> GetLog(StringIntern logName);

	private:
		/// <summary>
		/// Create the default log definitions; the Console and File logs.
		/// Will define the attributes that are applied to each logged message.
		/// These definitions can be changed via the engine_config.ini file
		/// but the default definitions are created to assist with the
		/// LogManager initialization, while providing reasonable defaults
		/// in the case that they are not defined in the .ini file.
		/// </summary>
		void CreateDefaultLogDefinitions();

		/// <summary>
		/// Register a log. This is purely for spdlog, and is likely an unnecessary step.
		/// </summary>
		/// <param name="pLogToRegister">- The log to register with spdlog.</param>
		void RegisterLog(std::shared_ptr<spdlog::logger> pLogToRegister);

		/// <summary>
		/// Unregister a log. This is purely for spdlog, and is likely an unnecessary step.
		/// </summary>
		/// <param name="pLogToRegister">- The log to unregister with spdlog.</param>
		void UnregisterLog(StringIntern logName);

		/// <summary>
		/// Unregister all logs. This is purely for spdlog, and is likely an unnecessary step.
		/// </summary>
		void UnregisterAllLogs();

		/// <summary>
		/// Converts the given Exelius LogLevel to the spdlog::level and sets it onto the
		/// given log.
		/// </summary>
		/// <param name="pLogToSet">- The log which to set the level of.</param>
		/// <param name="level">- The log level to set.</param>
		/// <returns>True if successfully set, false otherwise.</returns>
		bool SetLogLevel(std::shared_ptr<spdlog::logger> pLogToSet, LogLevel level);

		/// <summary>
		/// Overwrites the default definition for the console log.
		/// </summary>
		/// <param name="consoleDefinition">- The data to overwrite.</param>
		/// <returns>True if successful, false otherwise.</returns>
		bool OverwriteConsoleDefinition(const ConsoleLogDefinition& consoleDefinition);

		/// <summary>
		/// Overwrites the default definition for the file log.
		/// </summary>
		/// <param name="consoleDefinition">- The data to overwrite.</param>
		/// <returns>True if successful, false otherwise.</returns>
		bool OverwriteFileDefinition(const FileLogDefinition& fileDefinition);
	};
}