#pragma once

#include "Property.h"
#include "source/utility/generic/SmartPointers.h"

#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace pugi
{
    class xml_node;
}

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    class TilemapResource;
    class TileLayer;
    class ObjectGroup;
    class ImageLayer;
    class LayerGroup;
    /*!
    \brief Represents a layer of a tmx format tile map.
    This is an abstract base class from which all layer
    types are derived.
    */
    class Layer
    {
    public:
        using Ptr = UniquePtr<Layer>;

        Layer() : m_opacity(1.f), m_visible(true) {};
        virtual ~Layer() = default;

        /*!
        \brief Layer type as returned by getType()
        Tile: this layer is a TileLayer type
        Object: This layer is an ObjectGroup type
        Image: This layer is an ImageLayer type
        Group: This layer is a LayerGroup type
        */
        enum class Type
        {
            Tile,
            Object,
            Image,
            Group
        };

        /*!
        \brief Returns a Type value representing the concrete type.
        Use this when deciding which conrete layer type to use when
        calling the templated function getLayerAs<T>()
        */
        virtual Type GetType() const = 0;

        /*!
        \brief Use this to get a reference to the concrete layer type
        which this layer points to.
        Use getType() to return the type value of this layer and determine
        if the concrete type is TileLayer, ObjectGroup, ImageLayer, or LayerGroup
        */
        template <typename T>
        T& GetLayerAs();
        /*{
            throw("Not a valid layer type");
            return *dynamic_cast<T*>(this);
        }*/

        template <typename T>
        const T& GetLayerAs() const { return GetLayerAs<T>(); }

        /*!
        \brief Attempts to parse the specific node layer type
        */
        virtual void Parse(const pugi::xml_node&, TilemapResource* = nullptr) = 0;

        /*!
        \brief Returns the name of the layer
        */
        const eastl::string& GetName() const { return m_name; }

        /*!
        \brief Returns the opacity value for the layer
        */
        float GetOpacity() const { return m_opacity; }

        /*!
        \brief Returns whether this layer is visible or not
        */
        bool GetVisible() const { return m_visible; }

        /*!
        \brief Returns the offset from the top left corner
        of the layer, in pixels
        */
        const Vector2i& GetOffset() const { return m_offset; }

        /*!
        \brief Returns the size of the layer, in pixels.
        This will be the same as the map size for fixed size maps.
        */
        const Vector2u& GetSize() const { return m_size; }

        /*!
        \brief Returns the list of properties of this layer
        */
        const eastl::vector<Property>& GetProperties() const { return m_properties; }

    protected:

        void SetName(const eastl::string& name) { m_name = name; }
        void SetOpacity(float opacity) { m_opacity = opacity; }
        void SetVisible(bool visible) { m_visible = visible; }
        void SetOffset(std::int32_t x, std::int32_t y) { m_offset = Vector2i(x, y); }
        void SetSize(std::uint32_t width, std::uint32_t height) { m_size = Vector2u(width, height); }
        void AddProperty(const pugi::xml_node& node) { m_properties.emplace_back(); m_properties.back().Parse(node); }

    private:
        eastl::string m_name;
        float m_opacity;
        bool m_visible;
        Vector2i m_offset;
        Vector2u m_size;

        eastl::vector<Property> m_properties;
    };
}
