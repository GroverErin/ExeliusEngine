#pragma once

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/functional.h>
#include <EASTL/algorithm.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    //using inline here just to supress unused warnings on gcc
    bool Decompress(const char* source, eastl::vector<unsigned char>& dest, std::size_t inSize, std::size_t expectedSize);

    static inline eastl::string Base64_Decode(eastl::string const& encoded_string)
    {
        const eastl::string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

        eastl::function<bool(unsigned char)> is_base64 =
            [](unsigned char c)->bool
        {
            return (isalnum(c) || (c == '+') || (c == '/'));
        };

        auto in_len = encoded_string.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        eastl::string ret;

        while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
        {
            char_array_4[i++] = encoded_string[in_]; in_++;
            if (i == 4)
            {
                for (i = 0; i < 4; i++)
                {
                    char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));
                }
                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                {
                    ret += char_array_3[i];
                }
                i = 0;
            }
        }

        if (i)
        {
            for (j = i; j < 4; j++)
            {
                char_array_4[j] = 0;
            }

            for (j = 0; j < 4; j++)
            {
                char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++)
            {
                ret += char_array_3[j];
            }
        }

        return ret;
    }

    static inline eastl::string ResolveFilePath(eastl::string path, const eastl::string& workingDir)
    {
        static const eastl::string match("../");
        std::size_t result = path.find(match);
        std::size_t count = 0;
        while (result != eastl::string::npos)
        {
            count++;
            path = path.substr(result + match.size());
            result = path.find(match);
        }

        if (workingDir.empty()) return path;

        eastl::string outPath = workingDir;
        for (auto i = 0u; i < count; ++i)
        {
            result = outPath.find_last_of('/');
            if (result != eastl::string::npos)
            {
                outPath = outPath.substr(0, result);
            }
        }
        // this does only work on windows
        return outPath + '/' + path;
    }

    static inline eastl::string GetFilePath(const eastl::string& path)
    {
        //TODO this doesn't actually check that there is a file at the
        //end of the path, or that it's even a valid path...

        static auto searchFunc = [](const char separator, const eastl::string& path)->eastl::string
        {
            std::size_t i = path.rfind(separator, path.length());
            if (i != eastl::string::npos)
            {
                return(path.substr(0, i + 1));
            }

            return "";
        };

        return searchFunc('/', path);
    }
}