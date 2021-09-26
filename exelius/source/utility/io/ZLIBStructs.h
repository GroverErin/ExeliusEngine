#pragma once
#include <cstdint>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	/// ------------------------------------------------------------------------------------------
	/// ZLIB Structs - Provided by Rez in GAP 301
	/// ------------------------------------------------------------------------------------------
	#pragma region ZLIB_STRUCTS
		#pragma pack(1)
			struct ZipLocalHeader
			{
				static constexpr unsigned int kSignature = 0x04034b50;

				uint32_t sig;
				uint16_t version;
				uint16_t flag;
				uint16_t compression;	// Z_NO_COMPRESSION or Z_DEFLATED
				uint16_t modTime;
				uint16_t modDate;
				uint32_t crc32;
				uint32_t cSize;
				uint32_t ucSize;
				uint16_t fnameLen;	// Filename string follows header.
				uint16_t xtraLen;	// Extra field follows filename.
			};

			struct ZipDirHeader
			{
				static constexpr unsigned int kSignature = 0x06054b50;

				uint32_t sig;
				uint16_t nDisk;
				uint16_t nStartDisk;
				uint16_t totalDirEntries;
				uint32_t dirSize;
				uint32_t dirOffset;
				uint16_t cmntLen;
			};

			struct ZipDirFileHeader
			{
				static constexpr unsigned int kSignature = 0x02014b50;

				uint32_t sig;
				uint16_t verMade;
				uint16_t verNeeded;
				uint16_t flag;
				uint16_t compression;	// COMP_xxxx
				uint16_t modTime;
				uint16_t modDate;
				uint32_t crc32;
				uint32_t cSize;			// Compressed size
				uint32_t ucSize;		// Uncompressed size
				uint16_t fnameLen;		// Filename string follows header.
				uint16_t xtraLen;		// Extra field follows filename.
				uint16_t cmntLen;		// Comment field follows extra field.
				uint16_t diskStart;
				uint16_t intAttr;
				uint32_t extAttr;
				uint32_t hdrOffset;

				char* GetName() const { return (char*)(this + 1); }
				char* GetExtra() const { return GetName() + fnameLen; }
				char* GetComment() const { return GetExtra() + xtraLen; }
			};
		#pragma pack()
		static constexpr ptrdiff_t kPtrDelta = -(static_cast<ptrdiff_t>(sizeof(ZipDirHeader)));
	#pragma endregion
}