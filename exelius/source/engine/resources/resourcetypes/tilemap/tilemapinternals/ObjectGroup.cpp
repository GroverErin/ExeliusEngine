#include "EXEPCH.h"
#include "FreeFuncs.h"
#include "ObjectGroup.h"

#include <pugixml.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    ObjectGroup::ObjectGroup()
        : m_color(127, 127, 127, 255),
        m_drawOrder(DrawOrder::TopDown)
    {

    }

    //public
    void ObjectGroup::Parse(const pugi::xml_node& node, TilemapResource* map)
    {
        assert(map);

        eastl::string attribString = node.name();
        if (attribString != "objectgroup")
        {
            // TODO: Logger::log("Node was not an object group, node will be skipped.", Logger::Type::Error);
            return;
        }

        SetName(node.attribute("name").as_string());

        attribString = node.attribute("color").as_string();
        if (!attribString.empty())
        {
            m_color = Color::FromHexString(attribString);
        }

        SetOpacity(node.attribute("opacity").as_float(1.f));
        SetVisible(node.attribute("visible").as_bool(true));
        SetOffset(node.attribute("offsetx").as_int(), node.attribute("offsety").as_int());
        SetSize(node.attribute("width").as_uint(), node.attribute("height").as_uint());

        attribString = node.attribute("draworder").as_string();
        if (attribString == "index")
        {
            m_drawOrder = DrawOrder::Index;
        }

        for (const auto& child : node.children())
        {
            attribString = child.name();
            if (attribString == "properties")
            {
                for (const auto& p : child)
                {
                    m_properties.emplace_back();
                    m_properties.back().Parse(p);
                }
            }
            else if (attribString == "object")
            {
                m_objects.emplace_back();
                m_objects.back().Parse(child, map);
            }
        }
    }
}
