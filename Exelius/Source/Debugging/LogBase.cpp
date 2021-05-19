#include "EXEPCH.h"

#include "Source/Debugging/LogBase.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Intialize the engine and client macros.
	/// </summary>
	void _Log::Initialize()
	{
		eastl::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("Logs/Exelius.log", 1048576 * 5, 3, true));

		logSinks[0]->set_pattern("%^[%T] [%n] [%t]: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] [%n] [%t]: %v");

		s_pEngineLogger = std::make_shared<spdlog::logger>("EXELIUS", std::begin(logSinks), std::end(logSinks));
		spdlog::register_logger(s_pEngineLogger);
		s_pEngineLogger->set_level(spdlog::level::trace);
		s_pEngineLogger->flush_on(spdlog::level::trace);

		s_pClientLogger = std::make_shared<spdlog::logger>("CLIENT", std::begin(logSinks), std::end(logSinks));
		spdlog::register_logger(s_pClientLogger);
		s_pClientLogger->set_level(spdlog::level::trace);
		s_pClientLogger->flush_on(spdlog::level::trace);
	}
}

