#include "EXEPCH.h"

#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Exelius
{
	std::shared_ptr<spdlog::logger> Log::s_pCoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_pClientLogger;

	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_pCoreLogger = spdlog::stdout_color_mt("EXELIUS");
		s_pCoreLogger->set_level(spdlog::level::trace);

		s_pClientLogger = spdlog::stdout_color_mt("CLIENT");
		s_pClientLogger->set_level(spdlog::level::trace);
	}
}