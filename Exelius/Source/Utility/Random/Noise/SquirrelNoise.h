#pragma once

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// This algorithm (called SquirrelNoise3) was originally written by Squirrel Eiserloh and was presented in 
	/// his 2017 GDC talk titled Math for Game Programmers: Noise-Based RNG, which can be found here:
	/// https://www.gdcvault.com/play/1024365/Math-for-Game-Programmers-Noise
	/// 
	/// The use of the static and noexcept were borrowed from an explanation given by David "Rez" Graham.
	/// 
	/// Note: All hard-coded numbers are taken from Squirrels GDC talk or from Rez.
	/// </summary>
	class SquirrelNoise
	{
		unsigned int m_seed;

		static constexpr unsigned int kBitNoise = 0x68e31da4;
		static constexpr unsigned int kBitNoise1 = 0xb5297a4d;
		static constexpr unsigned int kBitNoise2 = 0x1b56c4e9;

		static constexpr unsigned int kPrime = 198491317;
		static constexpr unsigned int kPrime1 = 6543989;

	public:
		/// <summary>
		/// Creates a SquirrelNoise generator.
		/// </summary>
		/// <param name="seed">(unsigned long long) The seed used by the generator. Default is 0.</param>
		constexpr SquirrelNoise(unsigned int seed = 0) noexcept
			: m_seed(seed)
		{
			//
		}

		//----------------------------------------------------------------------------------------------------
		// 1 Dimensional Noise Functions
		//----------------------------------------------------------------------------------------------------
#pragma region 1 Dimensional Noise
		/// <summary>
		/// Retrieves a random value from the coordinate parameters.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="seedOverride">Seed to override defined seed.</param>
		/// <returns>Random value.</returns>
		static constexpr unsigned int Get1DNoise(int x, unsigned int seedOverride) noexcept
		{
			unsigned int mangledBits = static_cast<unsigned int>(x);
			mangledBits *= kBitNoise;
			mangledBits += seedOverride;
			mangledBits ^= (mangledBits >> 8);
			mangledBits *= kBitNoise1;
			mangledBits ^= (mangledBits << 8);
			mangledBits *= kBitNoise2;
			mangledBits ^= (mangledBits >> 8);

			return mangledBits;
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <returns>Random value.</returns>
		constexpr unsigned int Get1DNoise(int x) const noexcept
		{
			return Get1DNoise(x, m_seed);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// 0.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="seedOverride">Seed to override defined seed.</param>
		/// <returns>Random float between 0.0f and 1.0f.</returns>
		static constexpr float GetUniform1DNoise(int x, unsigned int seedOverride) noexcept
		{
			float noise = static_cast<float>(Get1DNoise(x, seedOverride));
			return noise / static_cast<float>(0xffffffff);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// 0.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <returns>Random float between 0.0f and 1.0f.</returns>
		constexpr float GetUniform1DNoise(int x) const noexcept
		{
			return GetUniform1DNoise(x, m_seed);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// -1.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="seedOverride">Seed to override defined seed.</param>
		/// <returns>Random float between -1.0f and 1.0f.</returns>
		static constexpr float GetSignedUniform1DNoise(int x, unsigned int seedOverride) noexcept
		{
			int noise = static_cast<int>(Get1DNoise(x, seedOverride));
			return static_cast<float>(noise) / static_cast<float>(0x7fffffff);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// -1.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <returns>Random float between -1.0f and 1.0f.</returns>
		constexpr float GetSignedUniform1DNoise(int x) const noexcept
		{
			return GetSignedUniform1DNoise(x, m_seed);
		}
#pragma endregion
		//----------------------------------------------------------------------------------------------------
		// 2 Dimensional Noise Functions
		//----------------------------------------------------------------------------------------------------
#pragma region 2 Dimensional Noise
		/// <summary>
		/// Retrieves a random value from the coordinate parameters.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="seedOverride">Seed to override defined seed.</param>
		/// <returns>Random value.</returns>
		static constexpr unsigned int Get2DNoise(int x, int y, unsigned int seedOverride) noexcept
		{
			return Get1DNoise(x + (kPrime * y), seedOverride);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <returns>Random value.</returns>
		constexpr unsigned int Get2DNoise(int x, int y) const noexcept
		{
			return Get2DNoise(x, y, m_seed);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// 0.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="seedOverride">Seed to override defined seed.</param>
		/// <returns>Random float between 0.0f and 1.0f.</returns>
		static constexpr float GetUniform2DNoise(int x, int y, unsigned int seedOverride) noexcept
		{
			float noise = static_cast<float>(Get2DNoise(x, y, seedOverride));
			return noise / static_cast<float>(0xffffffff);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// 0.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <returns>Random float between 0.0f and 1.0f.</returns>
		constexpr float GetUniform2DNoise(int x, int y) const noexcept
		{
			return GetUniform2DNoise(x, y, m_seed);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// -1.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="seedOverride">Seed to override defined seed.</param>
		/// <returns>Random float between -1.0f and 1.0f.</returns>
		static constexpr float GetSignedUniform2DNoise(int x, int y, unsigned int seedOverride) noexcept
		{
			int noise = static_cast<int>(Get2DNoise(x, y, seedOverride));
			return static_cast<float>(noise) / static_cast<float>(0x7fffffff);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// -1.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <returns>Random float between -1.0f and 1.0f.</returns>
		constexpr float GetSignedUniform2DNoise(int x, int y) const noexcept
		{
			return GetSignedUniform2DNoise(x, y, m_seed);
		}
#pragma endregion
		//----------------------------------------------------------------------------------------------------
		// 3 Dimensional Noise Functions
		//----------------------------------------------------------------------------------------------------
#pragma region 3 Dimensional Noise
		/// <summary>
		/// Retrieves a random value from the coordinate parameters.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="z">Z coordinate.</param>
		/// <param name="seedOverride">Seed to override defined seed.</param>
		/// <returns>Random value.</returns>
		static constexpr unsigned int Get3DNoise(int x, int y, int z, unsigned int seedOverride) noexcept
		{
			return Get1DNoise(x + (kPrime * y) + (kPrime1 * z), seedOverride);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters.
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="z">Z coordinate.</param>
		/// <returns>Random value.</returns>
		constexpr unsigned int Get3DNoise(int x, int y, int z) const noexcept
		{
			return Get3DNoise(x, y, z, m_seed);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// 0.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="z">Z coordinate.</param>
		/// <param name="seedOverride">Seed to override defined seed.</param>
		/// <returns>Random float between 0.0f and 1.0f.</returns>
		static constexpr float GetUniform3DNoise(int x, int y, int z, unsigned int seedOverride) noexcept
		{
			float noise = static_cast<float>(Get3DNoise(x, y, z, seedOverride));
			return 2.f * (noise / static_cast<float>(0xffffffff)) - 1.0f;
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// 0.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="z">Z coordinate.</param>
		/// <returns>Random float between 0.0f and 1.0f.</returns>
		constexpr float GetUniform3DNoise(int x, int y, int z) const noexcept
		{
			return GetUniform3DNoise(x, y, z, m_seed);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// -1.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="z">Z coordinate.</param>
		/// <param name="seedOverride">Seed to override defined seed.</param>
		/// <returns>Random float between -1.0f and 1.0f.</returns>
		static constexpr float GetSignedUniform3DNoise(int x, int y, int z, unsigned int seedOverride) noexcept
		{
			int noise = static_cast<int>(Get3DNoise(x, y, z, seedOverride));
			return static_cast<float>(noise) / static_cast<float>(0x7fffffff);
		}

		/// <summary>
		/// Retrieves a random value from the coordinate parameters between
		/// -1.0f and 1.0f (Inclusive).
		/// </summary>
		/// <param name="x">X coordinate.</param>
		/// <param name="y">Y coordinate.</param>
		/// <param name="z">Z coordinate.</param>
		/// <returns>Random float between -1.0f and 1.0f.</returns>
		constexpr float GetSignedUniform3DNoise(int x, int y, int z) const noexcept
		{
			return GetSignedUniform3DNoise(x, y, z, m_seed);
		}
#pragma endregion
		//----------------------------------------------------------------------------------------------------
		// Accessors
		//----------------------------------------------------------------------------------------------------

		/// <summary>
		/// Retrieve the defined seed.
		/// </summary>
		/// <returns>The defined seed.</returns>
		unsigned int GetSeed() { return m_seed; }

		/// <summary>
		/// Set the seed value.
		/// </summary>
		/// <param name="seed">Value to set.</param>
		void SetSeed(unsigned int seed) { m_seed = seed; }
	};
}