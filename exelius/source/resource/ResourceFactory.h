#pragma once
#include "source/resource/ResourceHelpers.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Resource;

	/// <summary>
	/// The abstract factory that creates resources. This should be
	/// subclassed in order for the resource manager to create
	/// resources from loaded data.
	/// 
	/// @note When subclassing the resource factory, Exelius::ResourceType
	/// will also play a role in how the factory can identify resources.
	/// 
	/// @note This class will be primarily interacting with the resource
	/// thread, and thus should be made thread safe.
	/// 
	/// @see ResourceType
	/// 
	/// For an example of the use of ResourceFactory
	/// @see ExeliusResourceFactory
	/// </summary>
	class ResourceFactory
	{
	public:
		/// <summary>
		/// Destroy the resource factory.
		/// </summary>
		virtual ~ResourceFactory() = default;

		/// <summary>
		/// Creates a resource from the given ResourceID.
		/// 
		/// This function is typically called from the resource thread
		/// and thus should be made thread safe.
		/// 
		/// Subclasses should return a pointer to the
		/// resource created, or nullptr on failure.
		/// </summary>
		/// <param name="resourceID">- The ID of the resource to create.</param>
		/// <returns>The created resource on success, or nullptr otherwise.</returns>
		virtual Resource* CreateResource(const ResourceID& resourceID) = 0;

	protected:
		/// <summary>
		/// A function that can be called from within the CreateResource
		/// function. It is used to convert the ResourceID to a ResourceType
		/// as necessary.
		/// </summary>
		/// <param name="resourceID"></param>
		/// <returns></returns>
		virtual ResourceType::Type GetResourceType(const ResourceID& resourceID) const = 0;
	};
}