#include "EXEPCH.h"
#include "Property.h"
#include "FreeFuncs.h"

#include <pugixml.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{

    Property::Property()
        : m_type(Type::Undef)
    {
    }

    Property Property::FromBoolean(bool value)
    {
        Property p;
        p.m_type = Type::Boolean;
        p.m_boolValue = value;
        return p;
    }

    Property Property::FromFloat(float value)
    {
        Property p;
        p.m_type = Type::Float;
        p.m_floatValue = value;
        return p;
    }

    Property Property::FromInt(int value)
    {
        Property p;
        p.m_type = Type::Int;
        p.m_intValue = value;
        return p;
    }

    Property Property::FromString(const eastl::string& value)
    {
        Property p;
        p.m_type = Type::String;
        p.m_stringValue = value;
        return p;
    }

    Property Property::FromColor(const Color& value)
    {
        Property p;
        p.m_type = Type::Color;
        p.m_colorValue = value;
        return p;
    }

    Property Property::FromFile(const eastl::string& value)
    {
        Property p;
        p.m_type = Type::File;
        p.m_stringValue = value;
        return p;
    }

    Property Property::FromObject(int value)
    {
        Property p;
        p.m_type = Type::Object;
        p.m_intValue = value;
        return p;
    }

    //public
    void Property::Parse(const pugi::xml_node& node, bool isObjectTypes)
    {
        // The value attribute name is different in object types
        const char* const valueAttribute = isObjectTypes ? "default" : "value";

        std::string attribData = node.name();
        if (attribData != "property")
        {
            // TODO: Logger::log("Node was not a valid property, node will be skipped", Logger::Type::Error);
            return;
        }

        m_name = node.attribute("name").as_string();

        attribData = node.attribute("type").as_string("string");
        if (attribData == "bool")
        {
            attribData = node.attribute(valueAttribute).as_string("false");
            m_boolValue = (attribData == "true");
            m_type = Type::Boolean;
            return;
        }
        else if (attribData == "int")
        {
            m_intValue = node.attribute(valueAttribute).as_int(0);
            m_type = Type::Int;
            return;
        }
        else if (attribData == "float")
        {
            m_floatValue = node.attribute(valueAttribute).as_float(0.f);
            m_type = Type::Float;
            return;
        }
        else if (attribData == "string")
        {
            m_stringValue = node.attribute(valueAttribute).as_string();

            //if value is empty, try getting the child value instead
            //as this is how multiline string properties are stored.
            if (m_stringValue.empty())
            {
                m_stringValue = node.child_value();
            }

            m_type = Type::String;
            return;
        }
        else if (attribData == "color")
        {
            m_colorValue = Color::FromHexString(node.attribute(valueAttribute).as_string("#FFFFFFFF"));
            m_type = Type::Color;
            return;
        }
        else if (attribData == "file")
        {
            m_stringValue = node.attribute(valueAttribute).as_string();
            m_type = Type::File;
            return;
        }
        else if (attribData == "object")
        {
            m_intValue = node.attribute(valueAttribute).as_int(0);
            m_type = Type::Object;
            return;
        }
    }
}