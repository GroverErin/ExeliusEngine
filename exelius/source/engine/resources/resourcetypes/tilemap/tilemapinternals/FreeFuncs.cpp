#include "EXEPCH.h"
#include "FreeFuncs.h"

#include <zlib.h>
#include <cstring>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    bool Decompress(const char* source, eastl::vector<unsigned char>& dest, std::size_t inSize, std::size_t expectedSize)
    {
        if (!source)
        {
            // TODO: LOG("Input string is empty, decompression failed.", Logger::Type::Error);
            return false;
        }

        int currentSize = static_cast<int>(expectedSize);
        eastl::vector<unsigned char> byteArray(expectedSize / sizeof(unsigned char));
        z_stream stream;
        stream.zalloc = Z_NULL;
        stream.zfree = Z_NULL;
        stream.opaque = Z_NULL;
        stream.next_in = (Bytef*)source;
        stream.avail_in = static_cast<unsigned int>(inSize);
        stream.next_out = (Bytef*)byteArray.data();
        stream.avail_out = static_cast<unsigned int>(expectedSize);

        //we'd prefer to use inflateInit2 but it appears 
        //to be incorrect in miniz. This is fine for zlib
        //compressed data, but gzip compressed streams
        //will fail to inflate. IMO still preferable to
        //trying to build/link zlib
        if (inflateInit(&stream/*, 15 + 32*/) != Z_OK)
        {
            // TODO: LOG("inflate init failed", Logger::Type::Error);
            return false;
        }

        int result = 0;
        do
        {
            result = inflate(&stream, Z_SYNC_FLUSH);

            switch (result)
            {
            default: break;
            case Z_NEED_DICT:
            case Z_STREAM_ERROR:
                result = Z_DATA_ERROR;
            case Z_DATA_ERROR:
                // TODO: Logger::log("If using gzip or zstd compression try using zlib instead", Logger::Type::Info);
            case Z_MEM_ERROR:
                inflateEnd(&stream);
                // TODO: Logger::log("inflate() returned " + std::to_string(result), Logger::Type::Error);
                return false;
            }

            if (result != Z_STREAM_END)
            {
                int oldSize = currentSize;
                currentSize *= 2;
                eastl::vector<unsigned char> newArray(currentSize / sizeof(unsigned char));
                std::memcpy(newArray.data(), byteArray.data(), currentSize / 2);
                byteArray = std::move(newArray);

                stream.next_out = (Bytef*)(byteArray.data() + oldSize);
                stream.avail_out = oldSize;

            }
        } while (result != Z_STREAM_END);

        if (stream.avail_in != 0)
        {
            // TODO: LOG("stream.avail_in is 0", Logger::Type::Error);
            // TODO: LOG("zlib decompression failed.", Logger::Type::Error);
            return false;
        }

        const int outSize = currentSize - stream.avail_out;
        inflateEnd(&stream);

        eastl::vector<unsigned char> newArray(outSize / sizeof(unsigned char));
        std::memcpy(newArray.data(), byteArray.data(), outSize);
        byteArray = std::move(newArray);

        //copy bytes to vector
        dest.insert(dest.begin(), byteArray.begin(), byteArray.end());

        return true;
    }
}