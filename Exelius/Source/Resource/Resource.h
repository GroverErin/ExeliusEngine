#pragma once
#include "Source/Resource/ResourceHelpers.h"

#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class Resource
	{
		ResourceID m_id;
	public:
		Resource() = delete;
		Resource(const Resource&) = delete;
		Resource(Resource&&) = delete;
		Resource& operator=(const Resource&) = delete;
		Resource& operator=(Resource&&) = delete;
		virtual ~Resource() = default;
		
		enum class LoadResult
		{
			kFailed,
			kKeptRawData,
			kDiscardRawData
		};

		/// <summary>
		/// Load the asset. This will call the assets specific load funcion based on it's type.
		/// </summary>
		/// <returns>Return true if the raw data needs to be kept for this resource to live.</returns>
		virtual LoadResult Load(eastl::vector<std::byte>&& data) = 0;

		/// <summary>
		/// Calls the resources specific unloading function.
		/// </summary>
		virtual void Unload() = 0;

		const ResourceID& GetResourceID() const { return m_id; }

	protected:
		Resource(const ResourceID& id)
			: m_id(id)
		{
			//
		}
	};
}