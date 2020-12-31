#pragma once

#include "spdlog/spdlog.h"

namespace Exelius
{
	class Log
	{
	public:
		static void Initialize();

		static constexpr std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_pCoreLogger; }
		static constexpr std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_pClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_pCoreLogger;
		static std::shared_ptr<spdlog::logger> s_pClientLogger;
	};
}

//Engine Log Macros
#define EXE_CORE_TRACE(...)	::Exelius::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EXE_CORE_INFO(...)	::Exelius::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EXE_CORE_WARN(...)	::Exelius::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EXE_CORE_ERROR(...)	::Exelius::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EXE_CORE_FATAL(...)	::Exelius::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client Log Macros
#define EXE_TRACE(...)		::Exelius::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EXE_INFO(...)		::Exelius::Log::GetClientLogger()->info(__VA_ARGS__)
#define EXE_WARN(...)		::Exelius::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EXE_ERROR(...)		::Exelius::Log::GetClientLogger()->error(__VA_ARGS__)
#define EXE_FATAL(...)		::Exelius::Log::GetClientLogger()->fatal(__VA_ARGS__)