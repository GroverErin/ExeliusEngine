#include "EXEPCH.h"
#include "LayerGroup.h"
#include "FreeFuncs.h"
#include "ObjectGroup.h"
#include "ImageLayer.h"
#include "TileLayer.h"

#include <pugixml.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    LayerGroup::LayerGroup(const eastl::string& workingDir, const Vector2u& tileCount)
        : m_workingDir(workingDir),
        m_tileCount(tileCount)
    {
    }

    //public
    void LayerGroup::Parse(const pugi::xml_node& node, TilemapResource* map)
    {
        assert(map);
        eastl::string attribString = node.name();
        if (attribString != "group")
        {
            //TODO: Logger::log("Node was not a group layer, node will be skipped.", Logger::Type::Error);
            return;
        }

        SetName(node.attribute("name").as_string());
        SetOpacity(node.attribute("opacity").as_float(1.f));
        SetVisible(node.attribute("visible").as_bool(true));
        SetOffset(node.attribute("offsetx").as_int(), node.attribute("offsety").as_int());
        SetSize(node.attribute("width").as_uint(), node.attribute("height").as_uint());

        // parse children
        for (const auto& child : node.children())
        {
            attribString = child.name();
            if (attribString == "properties")
            {
                for (const auto& p : child.children())
                {
                    AddProperty(p);
                }
            }
            else if (attribString == "layer")
            {
                m_layers.emplace_back(MakeUnique<TileLayer>(m_tileCount.x * m_tileCount.y));
                m_layers.back()->Parse(child, map);
            }
            else if (attribString == "objectgroup")
            {
                m_layers.emplace_back(MakeUnique<ObjectGroup>());
                m_layers.back()->Parse(child, map);
            }
            else if (attribString == "imagelayer")
            {
                m_layers.emplace_back(MakeUnique<ImageLayer>(m_workingDir));
                m_layers.back()->Parse(child, map);
            }
            else if (attribString == "group")
            {
                m_layers.emplace_back(MakeUnique<LayerGroup>(m_workingDir, m_tileCount));
                m_layers.back()->Parse(child, map);
            }
            else
            {
                //TODO: LOG("Unidentified name " + attribString + ": node skipped", Logger::Type::Warning);
            }
        }
    }
}