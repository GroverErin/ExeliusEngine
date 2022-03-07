#include "EXEPCH.h"
#include "FreeFuncs.h"
#include "ObjectTypes.h"

#include <pugixml.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    bool ObjectTypes::Load(const eastl::string& path)
    {
        Reset();

        //open the doc
        pugi::xml_document doc;
        auto result = doc.load_file(path.c_str());
        if (!result)
        {
            // TODO: Logger::log("Failed opening " + path, Logger::Type::Error);
            // TODO: Logger::log("Reason: " + std::string(result.description()), Logger::Type::Error);
            return false;
        }

        //make sure we have consistent path separators
        m_workingDirectory = path;
        eastl::replace(m_workingDirectory.begin(), m_workingDirectory.end(), '\\', '/');
        m_workingDirectory = GetFilePath(m_workingDirectory);

        if (!m_workingDirectory.empty() &&
            m_workingDirectory.back() == '/')
        {
            m_workingDirectory.pop_back();
        }


        //find the node and bail if it doesn't exist
        auto node = doc.child("objecttypes");
        if (!node)
        {
            // TODO: Logger::log("Failed opening object types: " + path + ", no objecttype node found", Logger::Type::Error);
            return Reset();
        }

        return ParseObjectTypesNode(node);
    }

    bool ObjectTypes::LoadFromString(const eastl::string& data, const eastl::string& workingDir)
    {
        Reset();

        //open the doc
        pugi::xml_document doc;
        auto result = doc.load_string(data.c_str());
        if (!result)
        {
            // TODO: Logger::log("Failed opening object types", Logger::Type::Error);
            // TODO: Logger::log("Reason: " + std::string(result.description()), Logger::Type::Error);
            return false;
        }

        //make sure we have consistent path separators
        m_workingDirectory = workingDir;
        eastl::replace(m_workingDirectory.begin(), m_workingDirectory.end(), '\\', '/');
        m_workingDirectory = GetFilePath(m_workingDirectory);

        if (!m_workingDirectory.empty() &&
            m_workingDirectory.back() == '/')
        {
            m_workingDirectory.pop_back();
        }


        //find the node and bail if it doesn't exist
        auto node = doc.child("objecttypes");
        if (!node)
        {
            // TODO: Logger::log("Failed object types: no objecttypes node found", Logger::Type::Error);
            return Reset();
        }

        return ParseObjectTypesNode(node);
    }

    bool ObjectTypes::ParseObjectTypesNode(const pugi::xml_node& node)
    {
        //<objecttypes> <-- node
        //  <objecttype name="Character" color="#1e47ff">
        //    <property>...

        //parse types
        for (const auto& child : node.children())
        {
            eastl::string attribString = child.name();
            if (attribString == "objecttype")
            {
                Type type;

                //parse the metadata of the type
                type.name = child.attribute("name").as_string();
                type.color = Color::FromHexString(child.attribute("color").as_string("#FFFFFFFF"));;

                //parse the default properties of the type
                for (const auto& p : child.children())
                {
                    Property prop;
                    prop.Parse(p, true);
                    type.properties.push_back(prop);
                }

                m_types.push_back(type);
            }
            else
            {
                // TODO: LOG("Unidentified name " + attribString + ": node skipped", Logger::Type::Warning);
            }
        }

        return true;
    }

    bool ObjectTypes::Reset()
    {
        m_workingDirectory.clear();
        m_types.clear();
        return false;
    }

}