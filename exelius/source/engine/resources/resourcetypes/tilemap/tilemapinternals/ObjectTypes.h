#pragma once

#include "Property.h"
#include "source/utility/generic/Color.h"

#include <EASTL/string.h>
#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    /*!
    \brief Parser for Tiled object types export format.
    Link to the specification: https://doc.mapeditor.org/fr/latest/manual/custom-properties/#predefining-properties.
    */
    class ObjectTypes final
    {
    public:
        /*!
        \brief Types that stores all predefined properties for all objects of this type.
        To take less spaces, they are not exported by default into maps.
        */
        struct Type
        {
            eastl::string name;
            Color color;
            eastl::vector<Property> properties;
        };

        /*!
        \brief Attempts to parse the object types at the given location.
        \param std::string Path to object types file to try to parse
        \returns true if object types was parsed successfully else returns false.
        In debug mode this will attempt to log any errors to the console.
        */
        bool Load(const eastl::string&);

        /*!
        \brief Loads an object types from a document stored in a string
        \param data A std::string containing the object types to load
        \param workingDir A std::string containing the working directory
        in which to find files.
        \returns true if successful, else false
        */
        bool LoadFromString(const eastl::string& data, const eastl::string& workingDir);

        /*!
        \brief Returns all predefined types and their default values.
        */
        const eastl::vector<Type>& GetTypes() const { return m_types; }

    private:
        eastl::string m_workingDirectory;
        eastl::vector<Type> m_types;

        bool ParseObjectTypesNode(const pugi::xml_node&);

        //always returns false so we can return this
        //on load failure
        bool Reset();
    };
}