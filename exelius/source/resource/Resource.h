#pragma once
#include "source/resource/ResourceHelpers.h"

#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Resource is a base class intended to be inhereted from.
	/// On it's own, Resource is just an identifier for a resource.
	/// 
	/// @note "Resource" and "Asset" terms may be used interchangably.
	/// 
	/// Subclasses should overwrite the Load and Unload functions
	/// which should then handle what happens with the raw binary
	/// data that the engine obtains.
	/// 
	/// You must take care that the Load and Unload functions are
	/// thread safe, as these functions are typically called from
	/// the resource loading thread.
	/// </summary>
	class Resource
	{
		/// <summary>
		/// The ResourceID referred to by this resource.
		/// </summary>
		ResourceID m_id;
	public:
		Resource() = delete; // TODO: Does delete matter here?
		Resource(const Resource&) = delete;
		Resource(Resource&&) = delete;
		Resource& operator=(const Resource&) = delete;
		Resource& operator=(Resource&&) = delete;
		virtual ~Resource() = default;
		
		/// <summary>
		/// An enum representing the result of the loading opertion.
		/// 
		/// @todo Due to the use of move semantics with a vector of
		/// bytes, it may only be necessary to provide a boolean.
		/// </summary>
		enum class LoadResult
		{
			kFailed,		/// The loading operation has failed.
			kKeptRawData,	/// The subclass has held onto the raw data.
			kDiscardRawData /// The subclass has discarded the raw data.
		};
		 
		/// <summary>
		/// Load the asset. This will call the Subclass specific load funcion.
		/// 
		/// @note
		/// This function is typically called from the resource thread
		/// and thus should be made thread safe.
		/// </summary>
		/// <param name="data">- The raw byte data of the loaded asset.</param>
		/// <returns>The result of the load operation.</returns>
		virtual LoadResult Load(eastl::vector<std::byte>&& data) = 0;

		/// <summary>
		/// Unload the asset. This will call the Subclass specific unloading function.
		/// </summary>
		virtual void Unload() = 0;

		/// <summary>
		/// Get the ResourceID referred to by this resource.
		/// </summary>
		/// <returns>The ID of this resource.</returns>
		const ResourceID& GetResourceID() const { return m_id; }

	protected:
		/// <summary>
		/// Constructor can only be called from within a Subclass.
		/// Assigns the resource ID.
		/// </summary>
		/// <param name="id">ResourceID to assign to this resource.</param>
		Resource(const ResourceID& id)
			: m_id(id)
		{
			EXE_ASSERT(id.IsValid());
		}
	};
}