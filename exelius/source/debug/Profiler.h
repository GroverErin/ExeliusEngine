#pragma once
// From https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Debug/Instrumentor.h

#include <EASTL/string.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <sstream>
#include <fstream>
#include <iomanip>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		eastl::string m_name;

		FloatingPointMicroseconds m_start;
		std::chrono::microseconds m_elapsedTime;
		std::thread::id m_threadID;
	};

	struct InstrumentationSession
	{
		eastl::string m_name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const eastl::string& name, const eastl::string& filepath = "results.json")
		{
			std::lock_guard lock(m_mutex);
			if (m_pCurrentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				//if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				//{
				//	HZ_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_pCurrentSession->m_name);
				//}
				InternalEndSession();
}
			m_outputStream.open(filepath.c_str());

			if (m_outputStream.is_open())
			{
				m_pCurrentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else
			{
				//if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				//{
				//	HZ_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				//}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.m_elapsedTime.count()) << ',';
			json << "\"name\":\"" << result.m_name.c_str() << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.m_threadID << ",";
			json << "\"ts\":" << result.m_start.count();
			json << "}";

			std::lock_guard lock(m_mutex);
			if (m_pCurrentSession)
			{
				m_outputStream << json.str();
				m_outputStream.flush();
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		Instrumentor()
			: m_pCurrentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			EndSession();
		}

		void WriteHeader()
		{
			m_outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_outputStream.flush();
		}

		void WriteFooter()
		{
			m_outputStream << "]}";
			m_outputStream.flush();
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (m_pCurrentSession)
			{
				WriteFooter();
				m_outputStream.close();
				delete m_pCurrentSession;
				m_pCurrentSession = nullptr;
			}
		}
	private:
		std::mutex m_mutex;
		InstrumentationSession* m_pCurrentSession;
		std::ofstream m_outputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_name(name), m_stopped(false)
		{
			m_startTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_startTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_stopped = true;
		}
	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::steady_clock> m_startTimepoint;
		bool m_stopped;
	};

	namespace InstrumentorUtils {

		template <size_t N>
		struct ChangeResult
		{
			char m_data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#define EXELIUS_ENABLE_PROFILING 0
#if EXELIUS_ENABLE_PROFILING
	// Resolve which function signature macro will be used. Note that this only
	// is resolved when the (pre)compiler starts, so the syntax highlighting
	// could mark the wrong one in your editor!
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define EXE_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define EXE_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define EXE_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define EXE_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define EXE_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define EXE_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define EXE_FUNC_SIG __func__
	#else
		#define EXE_FUNC_SIG "EXE_FUNC_SIG unknown!"
	#endif

	#define EXELIUS_PROFILE_BEGIN_SESSION(name, filepath) ::Exelius::Instrumentor::Get().BeginSession(name, filepath)
	#define EXELIUS_PROFILE_END_SESSION() ::Exelius::Instrumentor::Get().EndSession()
	#define EXELIUS_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Exelius::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
												   ::Exelius::InstrumentationTimer timer##line(fixedName##line.Data)
	#define EXELIUS_PROFILE_SCOPE_LINE(name, line) EXELIUS_PROFILE_SCOPE_LINE2(name, line)
	#define EXELIUS_PROFILE_SCOPE(name) EXELIUS_PROFILE_SCOPE_LINE(name, __LINE__)
	#define EXELIUS_PROFILE_FUNCTION() EXELIUS_PROFILE_SCOPE(EXE_FUNC_SIG)
#else
	#define EXELIUS_PROFILE_BEGIN_SESSION(name, filepath)
	#define EXELIUS_PROFILE_END_SESSION()
	#define EXELIUS_PROFILE_SCOPE(name)
	#define EXELIUS_PROFILE_FUNCTION()
#endif