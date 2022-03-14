#include "EXEPCH.h"
#include "TilemapResource.h"

#include "tilemapinternals/FreeFuncs.h"
#include "tilemapinternals/ObjectGroup.h"
#include "tilemapinternals/ImageLayer.h"
#include "tilemapinternals/TileLayer.h"
#include "tilemapinternals/LayerGroup.h"

#include "source/utility/string/StringTransformation.h"

#include <EASTL/string.h>
#include <EASTL/queue.h>
#include <pugixml.hpp>
#include <cstring>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    TilemapResource::TilemapResource(const ResourceID& id)
        : Resource(id)
        , m_orientation(Orientation::None)
        , m_renderOrder(RenderOrder::None)
        , m_isInfinite(false)
        , m_hexSideLength(0.f)
        , m_staggerAxis(StaggerAxis::None)
        , m_staggerIndex(StaggerIndex::None)
    {
        //
    }

    Resource::LoadResult TilemapResource::Load(eastl::vector<std::byte>&& data)
    {
        eastl::string text = eastl::string((const char*)data.begin(), (const char*)data.end());
        if (text.empty())
        {
            m_resourceManagerLog.Warn("Failed to write data to Tilemap Resource.");
            return LoadResult::kFailed;
        }

        LoadMapFromStringData(text, "assets");

        return LoadResult::kKeptRawData;
    }

    void TilemapResource::Unload()
    {
    }

    bool TilemapResource::LoadMapFromStringData(const eastl::string& data, const eastl::string& workingDir)
    {
        ResetTilemapData();

        //open the doc
        pugi::xml_document doc;
        auto result = doc.load_string(data.c_str());
        if (!result)
        {
            //TODO: Logger::log("Failed opening map", Logger::Type::Error);
            //TODO: Logger::log("Reason: " + eastl::string(result.description()), Logger::Type::Error);
            return false;
        }

        //make sure we have consistent path separators
        m_workingDirectory = workingDir;
        String::ToFilepath(m_workingDirectory);

        if (!m_workingDirectory.empty() &&
            m_workingDirectory.back() == '/')
        {
            m_workingDirectory.pop_back();
        }

        //find the map node and bail if it doesn't exist
        auto mapNode = doc.child("map");
        if (!mapNode)
        {
            //TODO: Logger::log("Failed opening map: no map node found", Logger::Type::Error);
            return ResetTilemapData();
        }

        return ParseRootMapNode(mapNode);
    }

    //private
    bool TilemapResource::ParseRootMapNode(const pugi::xml_node& mapNode)
    {
        //parse map attributes
        std::size_t pointPos = 0;
        eastl::string attribString = mapNode.attribute("version").as_string();
        if (attribString.empty() || (pointPos = attribString.find('.')) == eastl::string::npos)
        {
            //TODO: Logger::log("Invalid map version value, map not loaded.", Logger::Type::Error);
            return ResetTilemapData();
        }
        
        m_version.majorVersion = std::stoi(attribString.substr(0, pointPos).c_str()); // TODO: Can I avoid .c_str() here?
        m_version.minorVersion = std::stoi(attribString.substr(pointPos + 1).c_str());

        attribString = mapNode.attribute("orientation").as_string();
        if (attribString.empty())
        {
            //TODO: Logger::log("Missing map orientation attribute, map not loaded.", Logger::Type::Error);
            return ResetTilemapData();
        }

        if (attribString == "orthogonal")
        {
            m_orientation = Orientation::Orthogonal;
        }
        else if (attribString == "isometric")
        {
            m_orientation = Orientation::Isometric;
        }
        else if (attribString == "staggered")
        {
            m_orientation = Orientation::Staggered;
        }
        else if (attribString == "hexagonal")
        {
            m_orientation = Orientation::Hexagonal;
        }
        else
        {
            //TODO: Logger::log(attribString + " format maps aren't supported yet, sorry! Map not loaded", Logger::Type::Error);
            return ResetTilemapData();
        }

        attribString = mapNode.attribute("renderorder").as_string();
        //this property is optional for older version of map files
        if (!attribString.empty())
        {
            if (attribString == "right-down")
            {
                m_renderOrder = RenderOrder::RightDown;
            }
            else if (attribString == "right-up")
            {
                m_renderOrder = RenderOrder::RightUp;
            }
            else if (attribString == "left-down")
            {
                m_renderOrder = RenderOrder::LeftDown;
            }
            else if (attribString == "left-up")
            {
                m_renderOrder = RenderOrder::LeftUp;
            }
            else
            {
                //TODO: Logger::log(attribString + ": invalid render order. Map not loaded.", Logger::Type::Error);
                return ResetTilemapData();
            }
        }

        if (mapNode.attribute("infinite"))
        {
            m_isInfinite = mapNode.attribute("infinite").as_int() != 0;
        }

        unsigned width = mapNode.attribute("width").as_int();
        unsigned height = mapNode.attribute("height").as_int();
        if (width && height)
        {
            m_tileCount = { width, height };
        }
        else
        {
            //TODO: Logger::log("Invalid map tile count, map not loaded", Logger::Type::Error);
            return ResetTilemapData();
        }

        width = mapNode.attribute("tilewidth").as_int();
        height = mapNode.attribute("tileheight").as_int();
        if (width && height)
        {
            m_tileSize = { width, height };
        }
        else
        {
            //TODO: Logger::log("Invalid tile size, map not loaded", Logger::Type::Error);
            return ResetTilemapData();
        }

        m_hexSideLength = mapNode.attribute("hexsidelength").as_float();
        if (m_orientation == Orientation::Hexagonal && m_hexSideLength <= 0)
        {
            //TODO: Logger::log("Invalid he side length found, map not loaded", Logger::Type::Error);
            return ResetTilemapData();
        }

        attribString = mapNode.attribute("staggeraxis").as_string();
        if (attribString == "x")
        {
            m_staggerAxis = StaggerAxis::X;
        }
        else if (attribString == "y")
        {
            m_staggerAxis = StaggerAxis::Y;
        }
        if ((m_orientation == Orientation::Staggered || m_orientation == Orientation::Hexagonal)
            && m_staggerAxis == StaggerAxis::None)
        {
            //TODO: Logger::log("Map missing stagger axis property. Map not loaded.", Logger::Type::Error);
            return ResetTilemapData();
        }

        attribString = mapNode.attribute("staggerindex").as_string();
        if (attribString == "odd")
        {
            m_staggerIndex = StaggerIndex::Odd;
        }
        else if (attribString == "even")
        {
            m_staggerIndex = StaggerIndex::Even;
        }
        if ((m_orientation == Orientation::Staggered || m_orientation == Orientation::Hexagonal)
            && m_staggerIndex == StaggerIndex::None)
        {
            //TODO: Logger::log("Map missing stagger index property. Map not loaded.", Logger::Type::Error);
            return ResetTilemapData();
        }

        //colour property is optional
        attribString = mapNode.attribute("backgroundcolor").as_string();
        if (!attribString.empty())
        {
            m_backgroundColor = Color::FromHexString(attribString);
        }

        //TODO do we need next object ID? technically we won't be creating
        //new objects outside of the scene in xygine.

        //parse all child nodes
        for (const auto& node : mapNode.children())
        {
            eastl::string name = node.name();
            if (name == "tileset")
            {
                m_tilesets.emplace_back(m_workingDirectory);
                m_tilesets.back().Parse(node, this);
            }
            else if (name == "layer")
            {
                m_layers.emplace_back(MakeUnique<TileLayer>(m_tileCount.x * m_tileCount.y));
                m_layers.back()->Parse(node);
            }
            else if (name == "objectgroup")
            {
                m_layers.emplace_back(MakeUnique<ObjectGroup>());
                m_layers.back()->Parse(node, this);
            }
            else if (name == "imagelayer")
            {
                m_layers.emplace_back(MakeUnique<ImageLayer>(m_workingDirectory));
                m_layers.back()->Parse(node, this);
            }
            else if (name == "properties")
            {
                const auto& children = node.children();
                for (const auto& child : children)
                {
                    m_properties.emplace_back();
                    m_properties.back().Parse(child);
                }
            }
            else if (name == "group")
            {
                m_layers.emplace_back(MakeUnique<LayerGroup>(m_workingDirectory, m_tileCount));
                m_layers.back()->Parse(node, this);
            }
            else
            {
                //TODO: LOG("Unidentified name " + name + ": node skipped", Logger::Type::Warning);
            }
        }

        // fill animated tiles for easier lookup into map
        for (const auto& ts : m_tilesets)
        {
            for (const auto& tile : ts.GetTiles())
            {
                if (!tile.animation.frames.empty())
                {
                    m_animTiles[tile.ID + ts.GetFirstGID()] = tile;
                }
            }
        }

        return true;
    }

    bool TilemapResource::ResetTilemapData()
    {
        m_orientation = Orientation::None;
        m_renderOrder = RenderOrder::None;
        m_tileCount = { 0u, 0u };
        m_tileSize = { 0u, 0u };
        m_hexSideLength = 0.f;
        m_staggerAxis = StaggerAxis::None;
        m_staggerIndex = StaggerIndex::None;
        m_backgroundColor = {};
        m_workingDirectory = "";

        m_tilesets.clear();
        m_layers.clear();
        m_properties.clear();

        m_templateObjects.clear();
        m_templateTilesets.clear();

        return false;
    }
}
