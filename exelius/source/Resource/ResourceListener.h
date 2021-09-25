#pragma once
#include "ResourceHelpers.h"

#include <EASTL/shared_ptr.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Subclassing ResourceListener allows an object to be notified
	/// when an object has completed loading. This is the best option
	/// for initializing an object with the data loaded.
	/// 
	/// This class is purely virtual and the subclass must define
	/// OnResourceLoaded() in order to subclass.
	/// </summary>
	class ResourceListener
	{
	public:
		/// <summary>
		/// Destroy the resource listener.
		/// </summary>
		virtual ~ResourceListener() = default;

		/// <summary>
		/// This function is called when a resource has successfully loaded.
		/// Subclass can then use this function as a means of performing
		/// operations on/with the loaded resource.
		/// 
		/// @note
		/// This function is typically called from the resource thread
		/// and thus should be made thread safe.
        /// </summary>
        /// <param name="resourceID">- The ID of the loaded resource.</param>
        /// <returns>True if the resource was flushed here, false if not.</returns>
		virtual bool OnResourceLoaded(const ResourceID& resourceID) = 0;
	};

	/// <summary>
	/// A weak pointer to a ResourceListener object.
	/// This is weak because the ResourceListenerPtr
	/// is held by the resource thread and called upon
	/// load completion. A resource listener may expire
	/// before that load reaches completion.
	/// 
	/// @note
	/// The caviat to this is that the ResourceListenerPtr
	/// MUST be constructed using a shared_ptr.
	/// https://stackoverflow.com/questions/19140172/creating-weak-ptr-from-raw-pointer
	/// It is also worth noting that the use of
	/// enable_shared_from_this is very specific and in
	/// general should be avoided.
	/// </summary>
	using ResourceListenerPtr = eastl::weak_ptr<ResourceListener>;
}