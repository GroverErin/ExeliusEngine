#include "EXEPCH.h"

#include "Source/Debugging/LogManager.h"
#include "Source/Utility/String/StringTransformation.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Unregister all the loggers and clear the map of logs and log definitions.
	/// </summary>
	LogManager::~LogManager()
	{
		UnregisterAllLogs();

		for (auto pNameLogPair : m_logs)
		{
			pNameLogPair.second = nullptr;
		}
		m_logs.clear();

		for (auto pDefinition : m_logDefinitions)
		{
			pDefinition = nullptr;
		}
	}

	/// <summary>
	/// Initialize default engine log definitions and default log.
	/// Engine default log is always created but can be overridden by the config file.
	/// </summary>
	/// <returns>True if initialization was successful, false on failure.</returns>
	bool LogManager::PreInitialize()
	{
		CreateDefaultLogDefinitions();

		// Create Default Log. This is intentionally not created using the preinit file. This is
		// accessible as some of the tools that are used to initialize the log system rely on logging.
		// However, the config file can overwrite these default settings.
		if (!CreateLog("Exelius", LogLocation::kConsoleAndFile, LogLevel::kTrace))
			return false;

		return true;
	}

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
	bool LogManager::Initialize(const FileLogDefinition& fileDefinition, const ConsoleLogDefinition& consoleDefinition, const eastl::vector<LogData>& logData)
	{
		Log defaultLog;
		bool result = true;

		if (!OverwriteConsoleDefinition(consoleDefinition))
		{
			defaultLog.Error("Failed to create console definition.");
			result = false;
		}

		if (!OverwriteFileDefinition(fileDefinition))
		{
			defaultLog.Error("Failed to create file definition.");
			result = false;
		}

		for (auto& data : logData)
		{
			if (!CreateLog(data.m_logName, data.m_logLocation, data.m_logLevel))
			{
				defaultLog.Error("Failed to create log '{}'.", data.m_logName);
				result = false;
			}
		}

		return true;
	}

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
	bool LogManager::CreateLog(const char* pLogName, LogLocation type, LogLevel logLevel)
	{
		StringIntern logName(pLogName);

		// If a log with this name exists, destroy it.
		if (GetLog(logName))
		{
			UnregisterLog(logName);

			m_logLock.lock();
			m_logs.erase(logName);
			m_logLock.unlock();
		}

		std::shared_ptr<spdlog::logger> newLog = nullptr;

		m_logLock.lock();
		switch (type)
		{
			case LogLocation::kConsole:
			{
				// Create the logger that uses the defined sink for the console.
				m_logs.emplace(pLogName, std::make_shared<spdlog::logger>(pLogName, m_logDefinitions[0]));
				break;
			}
			case LogLocation::kFile:
			{
				// Create the logger that uses the defined sink for the file.
				m_logs.emplace(pLogName, std::make_shared<spdlog::logger>(pLogName, m_logDefinitions[1]));
				break;
			}
			case LogLocation::kConsoleAndFile:
			{
				// Create the logger that uses the defined sink for the console and the file.
				m_logs.emplace(pLogName, std::make_shared<spdlog::logger>(pLogName, std::begin(m_logDefinitions), std::end(m_logDefinitions)));
				break;
			}
			default:
			{
				m_logLock.unlock();
				return false;
				break;
			}
		}
		m_logLock.unlock();

		// Get the log we just created and initialize it.
		newLog = GetLog(logName);
		EXE_ASSERT(newLog);

		// TODO: This may need to be inside of the locked creation above...
		RegisterLog(newLog);

		SetLogLevel(newLog, logLevel);

		return true;
	}

	/// <summary>
	/// Retrieve the log with the given name if it exists.
	/// </summary>
	/// <param name="logName">- The name of the log to retrieve. Example: "Exelius" will retrieve the default log.</param>
	/// <returns>The log with the given name if found, nullptr if not found.</returns>
	std::shared_ptr<spdlog::logger> LogManager::GetLog(StringIntern logName)
	{
		EXE_ASSERT(logName.IsValid());

		// TODO: This locking can severely cripple the logging (Main thread!) performance,
		// and is caused by attempting to get and create logs at the same time.
		// These locks should be stripped in release mode, and log creation
		// outside of initialization should be a hard error case (in release builds).
		
		m_logLock.lock();
		auto found = m_logs.find(logName);
		m_logLock.unlock();

		if (found != m_logs.end())
			return found->second;

		return nullptr;
	}

	/// <summary>
	/// Create the default log definitions; the Console and File logs.
	/// Will define the attributes that are applied to each logged message.
	/// These definitions can be changed via the engine_config.ini file
	/// but the default definitions are created to assist with the
	/// LogManager initialization, while providing reasonable defaults
	/// in the case that they are not defined in the .ini file.
	/// </summary>
	void LogManager::CreateDefaultLogDefinitions()
	{
		// Create the definition of a colored console log, cabable of being used by multiple threads.
		m_logDefinitions[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

		// Create the definition of a file log that rotates between 3 files with a max size of 5mb,
		// cabable of being used by multiple threads.
		m_logDefinitions[1] = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("Logs/Exelius.log", 1048576 * 5, 3, true);

		// Sets the pattern attribute of the console log.
		// The pattern symbols are defined here: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
		m_logDefinitions[0]->set_pattern("%^[%T] [%n] [%t]: %v%$");
		m_logDefinitions[1]->set_pattern("[%T] [%l] [%n] [%t]: %v");
	}

	/// <summary>
	/// Register a log. This is purely for spdlog, and is likely an unnecessary step.
	/// </summary>
	/// <param name="pLogToRegister">- The log to register with spdlog.</param>
	void LogManager::RegisterLog(std::shared_ptr<spdlog::logger> pLogToRegister)
	{
		// Register the loggers, this makes them accessible by the spdlog::get() function.
		spdlog::register_logger(pLogToRegister);
	}

	/// <summary>
	/// Unregister a log. This is purely for spdlog, and is likely an unnecessary step.
	/// </summary>
	/// <param name="pLogToRegister">- The log to unregister with spdlog.</param>
	void LogManager::UnregisterLog(StringIntern logName)
	{
		spdlog::drop(logName.Get().c_str());
	}

	/// <summary>
	/// Unregister all logs. This is purely for spdlog, and is likely an unnecessary step.
	/// </summary>
	void LogManager::UnregisterAllLogs()
	{
		spdlog::drop_all();
	}

	/// <summary>
	/// Converts the given Exelius LogLevel to the spdlog::level and sets it onto the
	/// given log.
	/// </summary>
	/// <param name="pLogToSet">- The log which to set the level of.</param>
	/// <param name="level">- The log level to set.</param>
	/// <returns>True if successfully set, false otherwise.</returns>
	bool LogManager::SetLogLevel(const std::shared_ptr<spdlog::logger> pLogToSet, LogLevel level)
	{
		EXE_ASSERT(pLogToSet);

		if (level == LogLevel::kMax)
			level = LogLevel::kFatal;

		// Set the logging level of the loggers. This is the minimum severity level to be logged.
		// Setting the level to trace will log all messages.
		spdlog::level::level_enum logLevel = spdlog::level::off;

		switch (level)
		{
			case LogLevel::kTrace:	{ logLevel = spdlog::level::trace; break; }
			case LogLevel::kInfo:	{ logLevel = spdlog::level::info; break; }
			case LogLevel::kWarn:	{ logLevel = spdlog::level::warn; break; }
			case LogLevel::kError:	{ logLevel = spdlog::level::err; break; }
			case LogLevel::kFatal:	{ logLevel = spdlog::level::critical; break; }
			default:				{ return false; break; }
		}

		pLogToSet->set_level(logLevel);
		return true;
	}

	/// <summary>
	/// Overwrites the default definition for the console log.
	/// </summary>
	/// <param name="consoleDefinition">- The data to overwrite.</param>
	/// <returns>True if successful, false otherwise.</returns>
	bool LogManager::OverwriteConsoleDefinition(const ConsoleLogDefinition& consoleDefinition)
	{
		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		m_logDefinitions[0] = std::move(consoleSink);
		m_logDefinitions[0]->set_pattern(consoleDefinition.m_pattern.c_str());

		return true;
	}

	/// <summary>
	/// Overwrites the default definition for the file log.
	/// </summary>
	/// <param name="consoleDefinition">- The data to overwrite.</param>
	/// <returns>True if successful, false otherwise.</returns>
	bool LogManager::OverwriteFileDefinition(const FileLogDefinition& fileDefinition)
	{
		auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(fileDefinition.m_outputDirectory.c_str(), fileDefinition.m_maxFileSize, fileDefinition.m_numFiles, fileDefinition.m_rotateOnOpen);
		m_logDefinitions[1] = std::move(fileSink);
		m_logDefinitions[1]->set_pattern(fileDefinition.m_pattern.c_str());

		return true;
	}
}