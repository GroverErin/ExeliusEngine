#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Generates a random number using XORShift 128+.
	/// </summary>
	class Random
	{
		unsigned long long m_seed[2];

	public:
		/// <summary>
		/// Generator of random numbers using XORShift 128+.
		/// </summary>
		Random(unsigned long long seedLow = 0, unsigned long long seedHigh = 0);

		/// <summary>
		/// Retrieves the next random number in the "set".
		/// </summary>
		/// <returns>Random number.</returns>
		unsigned long long Rand();

		/// <summary>
		/// Retrieves the next random number in the "set" in the form o a float.
		/// </summary>
		/// <returns>Random float.</returns>
		float FRand();

		/// <summary>
		///  Return a int value from a specified range. (Inclusive)
		/// </summary>
		/// <param name="min">Minimum int value in range.</param>
		/// <param name="max">Maximum int value in range.</param>
		/// <returns>Random int.</returns>
		int IRandomRange(int min, int max);

		/// <summary>
		///  Return a float value from a specified range. (Inclusive)
		/// </summary>
		/// <param name="min">Minimum float value in range.</param>
		/// <param name="max">Maximum float value in range.</param>
		/// <returns>Random float.</returns>
		float FRandomRange(float min, float max);
	};
}