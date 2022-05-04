#include "EXEPCH.h"

#include "source/utility/io/File.h"
#include "source/utility/string/StringIntern.h"
#include "source/utility/string/StringTransformation.h"

#include<filesystem>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	File::~File()
	{
		Close();
	}

	bool File::Open(const char* pFileName, AccessPermission access, CreationType create)
	{
		return InternalOpen(pFileName, access, create);
	}

	bool File::Open(StringIntern filePath, AccessPermission access, CreationType create)
	{
		EXE_ASSERT(filePath.IsValid());
		return InternalOpen(filePath.Get().c_str(), access, create);
	}

	void File::Close()
	{
		m_file.close();
	}

	size_t File::Read(eastl::vector<std::byte>& data)
	{
		// Check validity prior to reading.
		if (!IsValid())
			return 0;

		m_file.read(reinterpret_cast<char*>(data.data()), GetSize());

		// Check validity after reading.
		if (!IsValid())
			return 0;

		return static_cast<size_t>(m_file.gcount());
	}

	size_t File::Write(const eastl::vector<std::byte>& data)
	{
		// Check validity prior to writing.
		if (!IsValid())
			return 0;

		m_file.write(reinterpret_cast<const char*>(data.data()), data.size());

		// Check validity after writing.
		if (!IsValid())
			return 0;

		return data.size();
	}

	bool File::IsValid() const
	{
		// Returns true if a file has no errors and is open currently.
		if (m_file.is_open() && m_file.good())
			return true;

		// Returns false if the file is good, but is not open.
		// This is the default state of newly created files.
		// This returns false due to a need to prevent
		// attempts to open files multiple times.
		// This should be the only case which this triggers.
		if (m_file.good())
			return false;

		EXE_LOG_INFO("File is not valid.");

		if (m_file.bad())
		{
			EXE_LOG_WARN("I/O error while reading");
		}
		else if (m_file.eof())
		{
			EXE_LOG_WARN("End of file reached successfully");
		}
		else if (m_file.fail())
		{
			EXE_LOG_WARN("Non-integer data encountered. Possible incorrect file path.");
		}
		else
		{
			EXE_LOG_WARN("An unknown error has occurred. Bit Flag States:\nGoodbit '{}'\nBadbit '{}'\nEOFbit '{}'\nFailbit '{}'", m_file.goodbit, m_file.badbit, m_file.eofbit, m_file.failbit);
		}

		return false;
	}

	size_t File::GetSize()
	{
		// Check if the file is opened.
		if (!IsValid())
			return 0;

		// TODO: This might fail with the OpenOrCreateFile flag set.

		// Find the size of the file in bytes
		m_file.seekg(0, std::ios::end);
		std::streampos size = m_file.tellg();
		m_file.seekg(0, std::ios::beg);

		return static_cast<size_t>(size);
	}

	const eastl::string File::GetFileExtension(const eastl::string& filepath)
	{
		return filepath.substr(filepath.find_last_of('.') + 1);
	}

	const eastl::string File::GetFileDirectory(const eastl::string& filepath)
	{
		const size_t lastSlashIndex = filepath.find_last_of('/');
		if (eastl::string::npos != lastSlashIndex)
		{
			return filepath.substr(0, lastSlashIndex);
		}

		return eastl::string();
	}

	const eastl::string File::GetFileName(const eastl::string& filepath, bool includeExtension)
	{
		const size_t lastSlashIndex = filepath.find_last_of('\\');
		if (eastl::string::npos != lastSlashIndex)
		{
			eastl::string fileName = filepath.substr(lastSlashIndex + 1);

			if (includeExtension)
				return fileName;
			return fileName.substr(0, fileName.find_last_of('.'));
		}
		return eastl::string();
	}

	bool File::VerifyAccessPermissions(AccessPermission access, CreationType create) const
	{
		if (access == AccessPermission::kReadOnly && (create == CreationType::kCreateFile || create == CreationType::kOverwriteFile))
		{
			EXE_LOG_ERROR("Invalid access and creation combination: Trying to create or overwrite a file with read only access.");
			return false;
		}

		return true;
	}

	bool File::InternalOpen(const char* pFileName, AccessPermission access, CreationType create)
	{
		if (!VerifyAccessPermissions(access, create))
			return false;

		if (IsValid())
		{
			EXE_LOG_WARN("Attempting to open a file that is already valid.");
			Close();
		}

		eastl::string filePath = pFileName;
		String::ToFilepath(filePath);
		

		std::ios_base::openmode mode = std::ios::binary;

		switch (access)
		{
			case AccessPermission::kReadOnly:
			{
				mode = std::ios::binary | std::ios::in;
				break;
			}
			case AccessPermission::kWriteOnly:
			{
				mode = mode | std::ios::out;
				break;
			}
			case AccessPermission::kReadWrite:
			{
				mode = mode | std::ios::in | std::ios::out;
				break;
			}
			default:
			{
				EXE_LOG_ERROR("Invalid access flag.");
				break;
			}
		}

		switch (create)
		{
			case CreationType::kCreateFile:
			{
				if (FileExists(filePath))
					return false;
				break;
			}
			case CreationType::kOverwriteFile:
			{
				mode = mode | std::ios::trunc;
				break;
			}
			case CreationType::kOpenFile:
			{
				if (!FileExists(filePath))
				{
					EXE_LOG_ERROR("File Not Found: '{}'", filePath.c_str());
					return false;
				}
				break;
			}
			case CreationType::kOpenOrCreateFile:
			{
				// TODO:
				// We use app here so that it will always append.
				// we should probably change this to 'ate' and add
				// the ability to move the file pointer.
				mode = mode | std::ios::app;
				break;
			}
			default:
			{
				EXE_LOG_ERROR("Invalid creation flag.");
				return false;
			}
		}

		m_file.open(filePath.c_str(), mode);

		if (!IsValid())
		{
			EXE_LOG_WARN("Failed File: {}", filePath.c_str());
			return false;
		}

		return true;
	}

	bool File::FileExists(const eastl::string& filePath) const
	{
		if (std::filesystem::exists(filePath.c_str()))
			return true;
		return false;
	}
}
