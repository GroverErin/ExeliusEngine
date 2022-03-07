#pragma once

#include "source/utility/generic/Macros.h"
#include "source/utility/generic/Color.h"

#include <EASTL/string.h>

namespace pugi
{
    class xml_node;
}

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    /*!
    \brief Represents a custom property.
    Tiles, objects and layers of a tmx map may have custom
    properties assigned to them. This class represents a
    single property and provides access to its value, the
    type of which can be determined with getType()
    */
    class Property final
    {
    public:

        enum class Type
        {
            Boolean,
            Float,
            Int,
            String,
            Color,
            File,
            Object,
            Undef
        };

        Property();

        static Property FromBoolean(bool value);
        static Property FromFloat(float value);
        static Property FromInt(int value);
        static Property FromString(const eastl::string& value);
        static Property FromColor(const Color& value);
        static Property FromFile(const eastl::string& value);
        static Property FromObject(int value);

        /*!
        \brief Attempts to parse the given node as a property
        \param isObjectTypes Set to true if the parsing is done from an object types files.
        */
        void Parse(const pugi::xml_node&, bool isObjectTypes = false);

        /*!
        \brief Returns the type of data stored in the property.
        This should generally be called first before trying to
        read the proprty value, as reading the incorrect type
        will lead to undefined behaviour.
        */
        Type GetType() const { return m_type; }

        /*!
        \brief Returns the name of this property
        */
        const eastl::string& GetName() const { return m_name; }

        /*!
        \brief Returns the property's value as a boolean
        */
        bool GetBoolValue() const { EXE_ASSERT(m_type == Type::Boolean); return m_boolValue; }

        /*!
        \brief Returns the property's value as a float
        */
        float GetFloatValue() const { EXE_ASSERT(m_type == Type::Float); return m_floatValue; }

        /*!
        \brief Returns the property's value as an integer
        */
        int GetIntValue() const { EXE_ASSERT(m_type == Type::Int); return m_intValue; }

        /*!
        \brief Returns the property's value as a string
        */
        const eastl::string& GetStringValue() const { EXE_ASSERT(m_type == Type::String); return m_stringValue; }

        /*!
        \brief Returns the property's value as a Colour struct
        */
        const Color& GetColourValue() const { EXE_ASSERT(m_type == Type::Color); return m_colorValue; }

        /*!
        \brief Returns the file path property as a string, relative to the map file
        */
        const eastl::string& GetFileValue() const { EXE_ASSERT(m_type == Type::File); return m_stringValue; }

        /*!
        \brief Returns the property's value as an integer object handle
        */
        int GetObjectValue() const { EXE_ASSERT(m_type == Type::Object); return m_intValue; }


    private:
        union
        {
            bool m_boolValue;
            float m_floatValue;
            int m_intValue;
        };
        eastl::string m_stringValue;
        eastl::string m_name;
        Color m_colorValue;

        Type m_type;
    };
}
