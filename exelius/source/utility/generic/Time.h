#pragma once
#include <cstdint>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct Time
	{
        constexpr Time()
            : m_microseconds(0)
        {
            //
        }

        constexpr Time(float seconds)
            : m_microseconds(static_cast<int64_t>(seconds * 1000000)) // TODO: Fix warning
        {
            //
        }

        constexpr Time(int32_t milliseconds)
            : m_microseconds(static_cast<int64_t>(milliseconds * 1000)) // TODO: Fix warning
        {
            //
        }

        constexpr Time(int64_t microseconds)
            : m_microseconds(microseconds)
        {
            //
        }

        constexpr float GetAsSeconds() const
        {
            return m_microseconds / 1000000.0f;
        }

        constexpr int32_t GetAsMilliseconds() const
        {
            return static_cast<int32_t>(m_microseconds / 1000);
        }

        constexpr int64_t GetAsMicroseconds() const
        {
            return m_microseconds;
        }

        // TODO: This should not be accessible for DeltaTime publicly... maybe friend Application will work here?
        constexpr void SetFromSeconds(float seconds)
        {
            m_microseconds = static_cast<int64_t>(seconds * 1000000); // TODO: Fix warning
        }

        static Time DeltaTime;

    private:

        int64_t m_microseconds; // Time value stored as microseconds
	};

    static constexpr Time Zero; // Predefined "zero" time value

    inline Time Time::DeltaTime;
}