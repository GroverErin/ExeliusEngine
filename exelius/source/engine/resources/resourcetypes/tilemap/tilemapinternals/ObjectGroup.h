#pragma once

#include "Layer.h"
#include "Object.h"
#include "source/utility/generic/Macros.h"
#include "source/utility/generic/Color.h"

#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    /*!
    \brief ObjectGroup layers contain a series of Objects
    which may be made up of shapes or images.
    */
    class ObjectGroup final : public Layer
    {
    public:
        enum class DrawOrder
        {
            Index, //< objects should be drawn in the order in which they appear
            TopDown //< objects should be drawn sorted by their Y position
        };

        ObjectGroup();

        Type GetType() const override { return Layer::Type::Object; }
        void Parse(const pugi::xml_node&, TilemapResource*) override;

        /*!
        \brief Returns the colour associated with this layer
        */
        const Color& GetColour() const { return m_color; }

        /*!
        \brief Returns the DrawOrder for the objects in this group.
        Defaults to TopDown, where Objects are drawn sorted by Y position
        */
        DrawOrder GetDrawOrder() const { return m_drawOrder; }

        /*!
        \brief Returns a reference to the vector of properties for
        the ObjectGroup
        */
        const eastl::vector<Property>& GetProperties() const { return m_properties; }

        /*!
        \brief Returns a reference to the vector of Objects which belong to the group
        */
        const eastl::vector<Object>& GetObjects() const { return m_objects; }

    private:
        Color m_color;
        DrawOrder m_drawOrder;

        eastl::vector<Property> m_properties;
        eastl::vector<Object> m_objects;
    };

    template <>
    inline ObjectGroup& Layer::GetLayerAs<ObjectGroup>()
    {
        EXE_ASSERT(GetType() == Type::Object);
        return *static_cast<ObjectGroup*>(this);
    }
}