#pragma once

#include "Property.h"
#include "source/utility/math/Rectangle.h"

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

    /*!
    \brief Contains the text information stored in a Text object.
    */
    struct Text final
    {
        eastl::string fontFamily;
        std::uint32_t pixelSize = 16; //!< pixels, not points
        bool wrap = false;
        Color color;
        bool bold = false;
        bool italic = false;
        bool underline = false;
        bool strikethough = false;
        bool kerning = true;

        enum class HAlign
        {
            Left, Centre, Right
        }hAlign = HAlign::Left;

        enum class VAlign
        {
            Top, Centre, Bottom
        }vAlign = VAlign::Top;

        eastl::string content; //!< actual string content
    };

    /*!
    \brief Objects are stored in ObjectGroup layers.
    Objects may be rectangular, elliptical, polygonal or
    a polyline. Rectangular and elliptical Objects have their
    size determined via the AABB, whereas polygon and polyline
    shapes are defined by a list of points. Objects are
    rectangular by default. Since version 1.0 Objects also
    support Text nodes.
    */
    class Object final
    {
    public:
        enum class Shape
        {
            Rectangle,
            Ellipse,
            Point,
            Polygon,
            Polyline,
            Text
        };

        Object();

        /*!
        \brief Attempts to parse the given xml node and
        read the Object properties if it is valid.
        */
        void Parse(const pugi::xml_node&, TilemapResource*);

        /*!
        \brief Returns the unique ID of the Object
        */
        std::uint32_t GetUID() const { return m_UID; }

        /*!
        \brief Returns the name of the Object
        */
        const eastl::string& GetName() const { return m_name; }

        /*!
        \brief Returns the type of the Object, as defined in the editor
        */
        const eastl::string& GetType() const { return m_type; }

        /*!
        \brief Returns the position of the Object in pixels
        */
        const Vector2f& GetPosition() const { return m_position; }

        /*!
        \brief Returns the global Axis Aligned Bounding Box.
        The AABB is positioned via the left and top properties, and
        define the Object's width and height. This can be used to derive
        the shape of the Object if it is rectangular or elliptical.
        */
        const FRectangle& GetAABB() const { return m_AABB; }

        /*!
        \brief Returns the rotation of the Object in degrees clockwise
        */
        float GetRotation() const { return m_rotation; }

        /*!
        \brief Returns the global tile ID associated with the Object
        if there is one. This is used to draw the Object (and therefore
        the Object must be rectangular)
        */
        std::uint32_t GetTileID() const { return m_tileID; }

        /*!
        \brief Returns the flip flags if the objects uses a TileID to
        draw it.
        Returns 0 otherwise.
        */
        std::uint8_t GetFlipFlags() const { return m_flipFlags; }

        /*!
        \brief Returns whether or not the Object is visible
        */
        bool Visible() const { return m_visible; }

        /*!
        \brief Returns the Shape type of the Object
        */
        Shape GetShape() const { return m_shape; }

        /*!
        \brief Returns a reference to the vector of points which
        make up the Object. If the Object is rectangular or elliptical
        then the vector will be empty. Point coordinates are in pixels,
        relative to the object position.
        */
        const eastl::vector<Vector2f>& GetPoints() const { return m_points; }

        /*!
        \brief Returns a reference to the vector of properties belonging to
        the Object.
        */
        const eastl::vector<Property>& GetProperties() const { return m_properties; }

        /*!
        \brief Returns a Text struct containing information about any text
        this object may have, such as font data and formatting.
        If an object does not contain any text information this struct will
        be populated with default values. Use getShape() to determine
        if this object is in fact a text object.
        */
        const Text& GetText() const { return m_textData; }
        Text& GetText() { return m_textData; }

        /*!
        \brief Returns the tileset name used by this object if it is derived
        from a template, else returns an empty string.
        If the string is not empty use it to index the unordered_map returned
        by Map::getTemplateTilesets()
        */
        const eastl::string& GetTilesetName() const { return m_tilesetName; }

    private:
        std::uint32_t m_UID;
        eastl::string m_name;
        eastl::string m_type;
        Vector2f m_position;
        FRectangle m_AABB;
        float m_rotation;
        std::uint32_t m_tileID;
        std::uint8_t m_flipFlags;
        bool m_visible;

        Shape m_shape;
        eastl::vector<Vector2f> m_points;
        eastl::vector<Property> m_properties;

        Text m_textData;

        eastl::string m_tilesetName;

        void ParsePoints(const pugi::xml_node&);
        void ParseText(const pugi::xml_node&);
        void ParseTemplate(const eastl::string&, TilemapResource*);
    };
}