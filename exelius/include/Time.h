#pragma once
#include "source/utility/generic/Timing.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	// This is a bit of a hacky class that allows the engine
	// and clients to use the following syntax to get delta time:
	// float dt = Exelius::Time.DeltaTime;
	class GameTime
	{
		friend class Application;
		friend class ScriptingSystem;

		class TimeScaler
		{
			friend class GameTime;

			bool m_usesElapsedTime;
			bool m_isTimescaled;

		public:
			operator float() const;
			float operator *(float left);
			float operator +(float left);
			float operator -(float left);

		private:
			TimeScaler(bool useElapsedTime, bool isTimescaled);
		};

	public:
		GameTime();

		// Last frame time in seconds.
		TimeScaler DeltaTime;

		// Last frame time unaffected by the timescale.
		TimeScaler DeltaTimeUnscaled;

		// Total application running time.
		TimeScaler ElapsedGameTime;

		static void SetTimeScale(float newTimeScale = 1.0f);

	private:
		static void RestartDeltaTime();
	};

	static GameTime Time;
}