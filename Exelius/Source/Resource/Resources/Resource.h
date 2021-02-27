#pragma once
#include "Source/Utility/Math/Math.h"
#include <functional>

namespace Exelius
{
	class Resource
	{
		unsigned int m_id;
		std::string m_filePath;
	public:
		enum class Status
		{
			kInvalid,
			kLoading,
			kLoaded,
			kUnloading
		};

		unsigned int GetID() const { return m_id; }
		
		Status GetStatus() const { return m_status; }
		void SetStatus(const Status status) { m_status = status; }

		const std::string& GetFilePath() const { return m_filePath; }
		void SetFilePath(const std::string& filePath) { m_filePath = filePath; }

		virtual void Load() = 0;
		virtual void Unload() = 0;

	protected:
		Resource(unsigned int resourceID)
			: m_id(resourceID)
			, m_status(Status::kInvalid)
			, m_filePath("Not Set.")
		{
			//
		}
		Resource(const Resource&) = delete;
		Resource(Resource&&) = delete;
		Resource& operator=(const Resource&) = delete;
		Resource& operator=(Resource&&) = delete;
		~Resource() = default;

	private:
		Status m_status;
	};

	using ResourcePtr = std::shared_ptr<Resource>;
}