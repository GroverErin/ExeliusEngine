#include "EXEPCH.h"
#include "Time.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct GameTimeData
	{
		// TODO: This should realisticly only ever be necessary when debugging.
		// Consider stripping this out in release?
		static constexpr bool m_useFrameTimeLimiting = true;
		static constexpr float s_kMaxDeltaTime = 1.0f;
		static constexpr float s_kDeltaTimeResetValue = .016666667f;

		float m_deltaTime = 0.0f;
		float m_elapsedTime = 0.0f;
		float m_unscaledDeltaTime = 0.0f;
		float m_timeScale = 1.0f;

		Timer m_deltaTimer;
		Timer m_elapsedTimer;
	};

	static GameTimeData s_timeData;


	GameTime::TimeScaler::TimeScaler(bool useElapsedTime, bool isTimescaled)
		: m_usesElapsedTime(useElapsedTime)
		, m_isTimescaled(isTimescaled)
	{

	}

	GameTime::TimeScaler::operator float() const
	{
		if (m_usesElapsedTime)
			return s_timeData.m_elapsedTime;

		if (m_isTimescaled)
			return s_timeData.m_deltaTime;
		
		return s_timeData.m_unscaledDeltaTime;
	}

	GameTime::GameTime()
		: DeltaTime(false, true)
		, DeltaTimeUnscaled(false, false)
		, ElapsedGameTime(true, false)
	{

	}

	void GameTime::SetTimeScale(float newTimeScale /*= 1.0f*/)
	{
		s_timeData.m_timeScale = newTimeScale;
	}

	void GameTime::RestartDeltaTime()
	{
		float time = s_timeData.m_deltaTimer.GetElapsedTimeAsSeconds();

		if (s_timeData.m_useFrameTimeLimiting)
		{
			if (time >= s_timeData.s_kMaxDeltaTime || time <= 0.0f)
			{
				s_timeData.m_deltaTime = s_timeData.s_kDeltaTimeResetValue * s_timeData.m_timeScale;
				s_timeData.m_unscaledDeltaTime = s_timeData.s_kDeltaTimeResetValue;
			}
		}

		s_timeData.m_deltaTime = time * s_timeData.m_timeScale;
		s_timeData.m_unscaledDeltaTime = time;

		s_timeData.m_deltaTimer.Start();
	}
}