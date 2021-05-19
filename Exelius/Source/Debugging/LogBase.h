#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"



/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Engine and Client logging class. This class is considered private because
	/// clients should prefer to use the Macros that are defined in Include/Log.h
	/// </summary>
	class _Log
	{
		inline static std::shared_ptr<spdlog::logger> s_pEngineLogger;
		inline static std::shared_ptr<spdlog::logger> s_pClientLogger;

	public:
		_Log() = default;
		_Log(const _Log&) = delete;
		_Log(_Log&&) = delete;
		_Log& operator=(const _Log&) = delete;
		_Log& operator=(_Log&&) = delete;
		~_Log() = default;

		/// <summary>
		/// Intialize the engine and client macros.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// Retrieve the engine specific logger.
		/// Prefer the EXELIUS_ENGINE_ macro.
		/// </summary>
		/// <returns>The engine logger.</returns>
		static constexpr std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_pEngineLogger; }

		/// <summary>
		/// Retrieve the client specific logger.
		/// Prefer the EXELIUS_ macro.
		/// </summary>
		/// <returns>The clent logger.</returns>
		static constexpr std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_pClientLogger; }
	};
}