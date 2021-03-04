#pragma once
#include "Source/Resource/ResourceHelpers.h"

namespace Exelius
{
	class Resource
	{
		// Raw resource data.
		const std::byte* m_pData;

	public:
		virtual ~Resource() = default;

		/// <summary>
		/// Load the asset. This will call the assets specific load funcion based on it's type.
		/// </summary>
		/// <returns>Return true if the raw data needs to be kept for this resource to live.</returns>
		virtual bool Load(const std::byte* pData, size_t dataSize) = 0;

		void SetRawData(const std::byte* const pRawData)
		{
			m_pData = pRawData;
		}

	protected:
		Resource() = default;
		Resource(const Resource&) = delete;
		Resource(Resource&&) = delete;
		Resource& operator=(const Resource&) = delete;
		Resource& operator=(Resource&&) = delete;
	};
}