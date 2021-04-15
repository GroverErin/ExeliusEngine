#pragma once
#include <stdint.h>
#include <EASTL/functional.h>

namespace Exelius
{
	/// <summary>
	/// This Handle struct was provided by Dylan Wijnen while he was assisting me to plan out my
	/// GameObject/Component system architecture. I knew that the system would require templates,
	/// an area I was previously not strong in, so he greatly helped me in that respect.
	/// 
	/// I had the system nearly complete, but I did not know how to handle the volatile nature of
	/// the pooled components. Destruction was slow, and so was comparisons. This Handle object 
	/// is the solution to that issue, and might be handy in other places, like for resources and GameObjects.
	/// 
	/// See: https://github.com/dylanwijnen1/DragonEngine/blob/master/DragonCore/src/Dragon/Generic/Handle.h
	/// </summary>
	struct Handle
	{
		/// <summary>
		/// The internal handle value that contains both the
		/// handle 'version' and 'ID'
		/// </summary>
		uint64_t m_handle;

		/// <summary>
		/// Mask used to extract the ID from the handle.
		/// </summary>
		inline constexpr static uint64_t kIDMask = 0x000000FFFFFFFF;

		/// <summary>
		/// 'Mask' used to extract the version from the handle.
		/// </summary>
		inline constexpr static unsigned int kVersionShiftInt = 32;

	public:
		/// <summary>
		/// Default Constructor - Creates an Invalid Handle
		/// </summary>
		Handle()
			: m_handle(0) // Invalid.
		{
			//
		}

		/// <summary>
		/// Constructor - Creates handle with given ID.
		/// </summary>
		/// <param name="id">ID to make handle.</param>
		Handle(uint64_t id)
			: m_handle(id)
		{
			//
		}

		/// <summary>
		/// Extracts the ID from the handle.
		/// </summary>
		/// <returns>The handle ID.</returns>
		size_t GetId() const
		{
			return (size_t)(m_handle & kIDMask);
		}

		/// <summary>
		/// Extracts the version from the handle.
		/// </summary>
		/// <returns>The handle version.</returns>
		uint32_t GetVersion() const
		{
			return (uint32_t)(m_handle >> kVersionShiftInt);
		}

		/// <summary>
		/// 'Stores' a version value into the handle.
		/// Dylans Note: Technically against GAP Standards. But don't wanna make a .cpp just for this line.
		/// </summary>
		/// <param name="version">Value to set the version as.</param>
		void SetVersion(uint32_t version)
		{
			m_handle &= kIDMask; // Clear Version
			m_handle |= ((uint64_t)version << kVersionShiftInt); // Set Version 
		}

		/// <summary>
		/// Increments the current value of the version.
		/// Dylans Note: See SetVersion.
		/// </summary>
		void IncrementVersion()
		{
			// Get current version.
			uint32_t version = GetVersion();

			// Increment then set.
			SetVersion(++version);
		}

		/// <summary>
		/// Checks if the version in this handle is valid.
		/// Versions are invalid if they are 0.
		/// In order for a new Handle to be valid,
		/// a version must be intentionally set.
		/// </summary>
		/// <returns>True if valid, false if not.</returns>
		bool IsValid() const
		{
			return GetVersion() != 0;
		}

		/// <summary>
		/// Sets the version to 0.
		/// A Handle is considered Invalid if the version is 0.
		/// </summary>
		void Invalidate()
		{
			SetVersion(0);
		}

		/// <summary>
		/// Overload for operator bool() const
		/// Checks if the version in this handle is valid.
		/// Versions are invalid if they are 0.
		/// In order for a new Handle to be valid,
		/// a version must be intentionally set.
		/// </summary>
		/// <returns>True if valid, false if not.</returns>
		operator bool() const
		{
			return IsValid();
		}

		/// <summary>
		/// Overload for the operator==()
		/// Allows for fast Handle Comparisons.
		/// </summary>
		/// <param name="other">The Handle to compare with.</param>
		/// <returns>True if handles match, false if not matching.</returns>
		bool operator==(const Handle& other) const
		{
			return m_handle == other.m_handle;
		}

		/// <summary>
		/// Overload for the operator!=()
		/// Allows for fast Handle Comparisons.
		/// </summary>
		/// <param name="other">The Handle to compare with.</param>
		/// <returns>False if handles match, true if not matching.</returns>
		bool operator!=(const Handle& other) const
		{
			return !(m_handle == other.m_handle);
		}
	};
}

namespace eastl
{
	template<>
	struct hash<Exelius::Handle>
	{
	public:
		// Used for storing a Handle as a key in EASTL hash maps.
		size_t operator()(Exelius::Handle const& self) const noexcept
		{
			return self.GetId();
		}
	};
}