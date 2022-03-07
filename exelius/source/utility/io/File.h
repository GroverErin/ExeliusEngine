#pragma once

#include <EASTL/vector.h>
#include <EASTL/string.h>
#include <fstream>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class StringIntern;

	class File
	{
		std::fstream m_file;
	public:
		enum class AccessPermission
		{
			kReadOnly	= 0, // Flag that enables reading from a file.
			kWriteOnly	= 1, // Flag that enables writing to a file.
			kReadWrite	= 2	 // Flag that enables reading and writing to a file.
		};

		enum class CreationType
		{
			kCreateFile			= 0, // Flag that enables a new file to be created, it will fail if the file already exists.
			kOverwriteFile		= 1, // Flag that enables a file to be created, if it already exists then it will be overwritten.
			kOpenFile			= 2, // Flag that enables a file to be opened, it will fail if it doesn't exist.
			kOpenOrCreateFile	= 3  // Flag that enables a file to be created or opened, depending on if it currently exists or not. If it does exist, then it will be appended to and not overwritten.
		};

		enum class FilePosition
		{
			kBeginning	= 0,
			kEnd		= 1
		};

	public:
		File() = default;
		File(const File&) = delete;
		File(File&&) = delete;
		File& operator=(const File&) = delete;
		File& operator=(File&&) = delete;
		~File();

		bool Open(const char* pFilePath, AccessPermission access, CreationType create);
		bool Open(StringIntern filePath, AccessPermission access, CreationType create);
		void Close();
		size_t Read(eastl::vector<std::byte>& data);
		size_t Write(const eastl::vector<std::byte>& data);

		bool IsValid() const;
		size_t GetSize();

		static const eastl::string GetFileExtension(const eastl::string& filepath);
		static const eastl::string GetFileDirectory(const eastl::string& filepath);

	private:
		bool VerifyAccessPermissions(AccessPermission access, CreationType create) const;
		bool InternalOpen(const char* pFileName, AccessPermission access, CreationType create);
		bool FileExists(const eastl::string& filePath) const;
	};
}