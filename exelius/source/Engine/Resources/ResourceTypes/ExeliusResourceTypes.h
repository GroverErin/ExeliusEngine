#pragma once
#include "Source/Resource/ResourceHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// The type of a resource. This is 'initially
	/// defined' in the Resource layer. It works as an
	/// exansive list of values.
	/// The engine must inheret the kInvalid type and
	/// increment it to define a new type. The engine
	/// must then increment that value again to further
	/// define a new type.
	/// 
	/// The client resource factory must inheret and
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
		/// A text file resource type is a resource
		/// that is converted to a string format.
		/// 
		/// @see TextFileResource
		/// </summary>
		static constexpr Type kTextFile = kInvalid + 1;

		/// <summary>
		/// A texture resource type is a resource
		/// that is an image format.
		/// 
		/// @see TextureResource
		/// </summary>
		static constexpr Type kTexture = kTextFile + 1;

		/// <summary>
		/// A sprite sheet resource type is a resource
		/// that contains a reference to a Texture
		/// resource and maintains a list of Sprites.
		/// 
		/// @see SpritesheetResource
		/// </summary>
		static constexpr Type kSpritesheet = kTexture + 1;

		/// <summary>
		/// The last resource type to be defined by the engine.
		/// The client application should increment this type
		/// in order to define a new type.
		/// 
		/// Example:
		/// @code{.cpp}
		///		static constexpr Exelius::ResourceType::Type kClientResource = kLastEngineResourceType + 1;
		/// @endcode
		/// </summary>
		static constexpr Type kLastEngineResourceType = kSpritesheet;
	}
}