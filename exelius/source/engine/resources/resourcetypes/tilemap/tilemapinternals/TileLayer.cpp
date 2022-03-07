#include "EXEPCH.h"
#include "FreeFuncs.h"
#include "TileLayer.h"

#include <pugixml.hpp>
#include <sstream>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{

    TileLayer::TileLayer(std::size_t tileCount)
        : m_tileCount(tileCount)
    {
        m_tiles.reserve(tileCount);
    }

    //public
    void TileLayer::Parse(const pugi::xml_node& node, TilemapResource*)
    {
        std::string attribName = node.name();
        if (attribName != "layer")
        {
            // TODO: Logger::log("node not a layer node, skipped parsing", Logger::Type::Error);
            return;
        }

        SetName(node.attribute("name").as_string());
        SetOpacity(node.attribute("opacity").as_float(1.f));
        SetVisible(node.attribute("visible").as_bool(true));
        SetOffset(node.attribute("offsetx").as_int(), node.attribute("offsety").as_int());
        SetSize(node.attribute("width").as_uint(), node.attribute("height").as_uint());

        for (const auto& child : node.children())
        {
            attribName = child.name();
            if (attribName == "data")
            {
                attribName = child.attribute("encoding").as_string();
                if (attribName == "base64")
                {
                    ParseBase64(child);
                }
                else if (attribName == "csv")
                {
                    ParseCSV(child);
                }
                else
                {
                    ParseUnencoded(child);
                }
            }
            else if (attribName == "properties")
            {
                for (const auto& p : child.children())
                {
                    AddProperty(p);
                }
            }
        }

    }

    //private
    void TileLayer::ParseBase64(const pugi::xml_node& node)
    {
        auto ProcessDataString = [](eastl::string dataString, std::size_t tileCount, bool compressed)->eastl::vector<std::uint32_t>
        {
            std::string convertedStr = dataString.c_str();
            std::stringstream ss;
            ss << convertedStr;
            ss >> convertedStr;
            dataString = convertedStr.c_str();
            dataString = Base64_Decode(dataString);

            std::size_t expectedSize = tileCount * 4; //4 bytes per tile
            eastl::vector<unsigned char> byteData;
            byteData.reserve(expectedSize);

            if (compressed)
            {
                //unzip
                std::size_t dataSize = dataString.length() * sizeof(unsigned char);
                if (!Decompress(dataString.c_str(), byteData, dataSize, expectedSize))
                {
                    // TODO: LOG("Failed to decompress layer data, node skipped.", Logger::Type::Error);
                    return {};
                }
            }
            else
            {
                byteData.insert(byteData.end(), dataString.begin(), dataString.end());
            }

            //data stream is in bytes so we need to OR into 32 bit values
            eastl::vector<std::uint32_t> IDs;
            IDs.reserve(tileCount);
            for (auto i = 0u; i < expectedSize - 3u; i += 4u)
            {
                std::uint32_t id = byteData[i] | byteData[i + 1] << 8 | byteData[i + 2] << 16 | byteData[i + 3] << 24;
                IDs.push_back(id);
            }

            return IDs;
        };


        eastl::string data = node.text().as_string();
        if (data.empty())
        {
            //check for chunk nodes
            auto dataCount = 0;
            for (const auto& childNode : node.children())
            {
                eastl::string childName = childNode.name();
                if (childName == "chunk")
                {
                    eastl::string dataString = childNode.text().as_string();
                    if (!dataString.empty())
                    {
                        Chunk chunk;
                        chunk.position.x = childNode.attribute("x").as_int();
                        chunk.position.y = childNode.attribute("y").as_int();

                        chunk.size.x = childNode.attribute("width").as_int();
                        chunk.size.y = childNode.attribute("height").as_int();

                        auto IDs = ProcessDataString(dataString, (chunk.size.x * chunk.size.y), node.attribute("compression"));

                        if (!IDs.empty())
                        {
                            CreateTiles(IDs, chunk.tiles);
                            m_chunks.push_back(chunk);
                            dataCount++;
                        }
                    }
                }
            }

            if (dataCount == 0)
            {
                // TODO: Logger::log("Layer " + etName() + " has no layer data. Layer skipped.", Logger::Type::Error);
                return;
            }
        }
        else
        {
            auto IDs = ProcessDataString(data, m_tileCount, node.attribute("compression"));
            CreateTiles(IDs, m_tiles);
        }
    }

    void TileLayer::ParseCSV(const pugi::xml_node& node)
    {
        auto ProcessDataString = [](const eastl::string dataString, std::size_t tileCount)->eastl::vector<std::uint32_t>
        {
            eastl::vector<std::uint32_t> IDs;
            IDs.reserve(tileCount);

            const char* ptr = dataString.c_str();
            while (true)
            {
                char* end;
                auto res = std::strtoul(ptr, &end, 10);
                if (end == ptr) break;
                ptr = end;
                IDs.push_back(res);
                if (*ptr == ',') ++ptr;
            }

            return IDs;
        };

        eastl::string data = node.text().as_string();
        if (data.empty())
        {
            //check for chunk nodes
            auto dataCount = 0;
            for (const auto& childNode : node.children())
            {
                eastl::string childName = childNode.name();
                if (childName == "chunk")
                {
                    eastl::string dataString = childNode.text().as_string();
                    if (!dataString.empty())
                    {
                        Chunk chunk;
                        chunk.position.x = childNode.attribute("x").as_int();
                        chunk.position.y = childNode.attribute("y").as_int();

                        chunk.size.x = childNode.attribute("width").as_int();
                        chunk.size.y = childNode.attribute("height").as_int();

                        auto IDs = ProcessDataString(dataString, chunk.size.x * chunk.size.y);

                        if (!IDs.empty())
                        {
                            CreateTiles(IDs, chunk.tiles);
                            m_chunks.push_back(chunk);
                            dataCount++;
                        }
                    }
                }
            }

            if (dataCount == 0)
            {
                // TODO: Logger::log("Layer " + getName() + " has no layer data. Layer skipped.", Logger::Type::Error);
                return;
            }
        }
        else
        {
            CreateTiles(ProcessDataString(data, m_tileCount), m_tiles);
        }
    }

    void TileLayer::ParseUnencoded(const pugi::xml_node& node)
    {
        eastl::string attribName;
        eastl::vector<std::uint32_t> IDs;
        IDs.reserve(m_tileCount);

        for (const auto& child : node.children())
        {
            attribName = child.name();
            if (attribName == "tile")
            {
                IDs.push_back(child.attribute("gid").as_uint());
            }
        }

        CreateTiles(IDs, m_tiles);
    }

    void TileLayer::CreateTiles(const eastl::vector<std::uint32_t>& IDs, eastl::vector<Tile>& destination)
    {
        //LOG(IDs.size() != m_tileCount, "Layer tile count does not match expected size. Found: "
        //    + std::to_string(IDs.size()) + ", expected: " + std::to_string(m_tileCount));

        static const std::uint32_t mask = 0xf0000000;
        for (const auto& id : IDs)
        {
            destination.emplace_back();
            destination.back().flipFlags = ((id & mask) >> 28);
            destination.back().ID = id & ~mask;
        }
    }
}
