#include "EXEPCH.h"
#include "Source/Utility/Random/Random.h"

#include <stdint.h>
#include <ctime>

#include <EASTL/numeric_limits.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Generator of random numbers using XORShift 128+.
	/// </summary>
	Random::Random(unsigned long long seedLow, unsigned long long seedHigh)
		: m_seed{seedLow, seedHigh}
	{
		if (seedLow == 0 && seedHigh == 0)
		{
			m_seed[0] = time(nullptr);
			m_seed[1] = time(nullptr);
		}
	}

	/// <summary>
	/// Retrieves the next random number in the "set".
	/// </summary>
	/// <returns>Random number.</returns>
	unsigned long long Random::Rand()
	{
		unsigned long long x = m_seed[0];
		const unsigned long long y = m_seed[1];
		m_seed[0] = y;
		x ^= x << 23;
		m_seed[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
		return m_seed[1] + y;
	}

	/// <summary>
	/// Retrieves the next random number in the "set" in the form o a float.
	/// </summary>
	/// <returns>Random float.</returns>
	float Random::FRand()
	{
		float randF = static_cast<float>(Rand()) / static_cast<float>(eastl::numeric_limits<unsigned long long>::max());
		return randF;
	}

	/// <summary>
	///  Return a int value from a specified range. (Inclusive)
	/// </summary>
	/// <param name="min">Minimum int value in range.</param>
	/// <param name="max">Maximum int value in range.</param>
	/// <returns>Random int.</returns>
	int Random::IRandomRange(int min, int max)
	{
		unsigned long long mod = (((unsigned long long)max - (unsigned long long)min) + 1);
		return min + (int)(Rand() % mod);
	}

	/// <summary>
	///  Return a float value from a specified range. (Inclusive)
	/// </summary>
	/// <param name="min">Minimum float value in range.</param>
	/// <param name="max">Maximum float value in range.</param>
	/// <returns>Random float.</returns>
	float Random::FRandomRange(float min, float max)
	{
		float rand = FRand();
		float difference = max - min;
		return min + (rand * difference);
	}
}