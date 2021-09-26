#pragma once
#include "Source/Utility/String/StringIntern.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// The unique identifier of a resource.
	///	This identifier is the Filepath and File name
	/// of the asset to be loaded.
	/// </summary>
	using ResourceID = StringIntern;

	/// <summary>
	/// The type of a resource. This is 'initially
	/// defined' in the Resource layer. It works as an
	/// exansive list of values.
	/// The engine must inheret the kInvalid type and
	/// increment it to define a new type. The engine
	/// must then increment that value again to further
	/// define a new type.
	/// 
	/// The first client resource type must inheret and
	/// increment the kLastEngineResourceType in a
	/// similar fashion in order to define client specific
	/// resource types.
	/// 
	/// This coincides with how the resource factories
	/// work.
	/// 
	/// @see ResourceFactory
	/// 
	/// @todo Defining these resource types is a bit odd,
	/// maybe consider redesigning this.
	/// </summary>
	namespace ResourceType
	{
		/// <summary>
		/// The numarical interpretation of the resource type.
		/// Used for comparison operations within the resource
		/// factories.
		/// </summary>
		using Type = uint32_t;

		/// <summary>
		/// Invalid resource type.
		/// </summary>
		static constexpr Type kInvalid = 0;
	}

	/// <summary>
	/// The current status resource signifying where
	/// this resource is in the loading or unloading
	/// process of the resource manager.
	/// </summary>
	enum class ResourceLoadStatus
	{
		kInvalid,	/// Initial state, this is set by the constructor.
		kLoading,	/// Set when the file begins the loading process.
		kLoaded,	/// Set when the loader thread has completed loading this resource.
		kUnloading,	/// Set when the loader thread begins unloading this resource.
		kUnloaded,	/// Set when the loader thread finished unloading this resource.
	};
}