#pragma once
#include <cstdint>
#include <chrono>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Timer
	{
		std::chrono::time_point<std::chrono::steady_clock> m_startTime;
	public:
		Timer(bool startNow = false)
			: m_startTime()
		{
            if (startNow)
                Start();
		}

        void Start()
        {
            m_startTime = std::chrono::steady_clock::now();
        }

        inline int64_t GetElapsedTime() const
		{
            auto currentTime = std::chrono::steady_clock::now();
            auto highResolutionStart = std::chrono::duration<double, std::micro>{ m_startTime.time_since_epoch() };
            auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(currentTime).time_since_epoch()
                - std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch();

            return (int64_t)elapsedTime.count();
		}

        inline float GetElapsedTimeAsMilliseconds() const
        {
            return (float)GetElapsedTime() * .001f;
        }

        inline float GetElapsedTimeAsSeconds() const
        {
            return (float)GetElapsedTime() * .000001f;
        }
	};
}