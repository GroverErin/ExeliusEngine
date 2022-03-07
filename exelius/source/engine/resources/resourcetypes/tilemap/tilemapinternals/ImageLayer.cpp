#include "EXEPCH.h"
#include "ImageLayer.h"
#include "FreeFuncs.h"

#include <pugixml.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    ImageLayer::ImageLayer(const eastl::string& workingDir)
        : m_workingDir(workingDir),
        m_hasTransparency(false)
    {

    }

    //public
    void ImageLayer::Parse(const pugi::xml_node& node, TilemapResource*)
    {
        eastl::string attribName = node.name();
        if (attribName != "imagelayer")
        {
            // TODO: Logger::log("Node not an image layer, node skipped", Logger::Type::Error);
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
            if (attribName == "image")
            {
                attribName = child.attribute("source").as_string();
                if (attribName.empty())
                {
                    // TODO: Logger::log("Image Layer has missing source property", Logger::Type::Warning);
                    return;
                }

                if (child.attribute("width") && child.attribute("height"))
                {
                    m_imageSize.x = child.attribute("width").as_uint();
                    m_imageSize.y = child.attribute("height").as_uint();
                }

                m_filePath = ResolveFilePath(attribName, m_workingDir);
                if (child.attribute("trans"))
                {
                    attribName = child.attribute("trans").as_string();
                    m_transparencyColor = Color::FromHexString(attribName);
                    m_hasTransparency = true;
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
}
