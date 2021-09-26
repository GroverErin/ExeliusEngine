#pragma once
#include <EASTL/numeric_limits.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Unique identifier for GameObject Entities.
	/// </summary>
	using GameObjectID = uint32_t;

	/// <summary>
	/// Value of an invalid identifier. This is used for error state setting/evaluation.
	/// </summary>
	static constexpr GameObjectID kInvalidGameObjectID = eastl::numeric_limits<uint32_t>::max();

	/// <summary>
	/// Enum used to determine how a gameobject is created.
	/// This will effect how a gameobject will load resources
	/// that it requires in order to initialize.
	/// </summary>
	enum class CreationMode
	{
		kDoNotLoad		= 0, /// If not loaded, then fail the Object Creation.
		kLoadImmediate	= 1, /// If not isn't loaded, load on the main thread. This will also apply to resources that the GameObject depends on.
		kQueueAndSignal	= 2, /// If not loaded, queue and signal the resource thread. This will also apply to resources that the GameObject depends on.
		kQueueNoSignal	= 3  /// If not loaded, queue and DO NOT signal the resource thread. This will also apply to resources that the GameObject depends on.
	};
}