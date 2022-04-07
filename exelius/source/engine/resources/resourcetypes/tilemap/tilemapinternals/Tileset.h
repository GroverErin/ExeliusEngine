#pragma once

#include "Property.h"
#include "ObjectGroup.h"

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/array.h>

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
    \brief Represents a Tileset node as loaded
    from a *.tmx format tile map via the tmx::Map
    class.
    */
    class Tileset final
    {
    public:
        explicit Tileset(const eastl::string& workingDir);

        /*!
        \brief Any tiles within a tile set which have special
        data associated with them such as animation or terrain
        information will have one of these stored in the tile set.
        */
        struct Tile final
        {
            std::uint32_t ID = 0;
            eastl::array<std::int32_t, 4u> terrainIndices{};
            std::uint32_t probability = 100;

            /*!
            \brief a group of frames which make up an animation
            */
            struct Animation final
            {
                /*!
                \brief A frame within an animation
                */
                struct Frame final
                {
                    std::uint32_t tileID = 0;
                    std::uint32_t duration = 0;

                    bool operator == (const Frame& other) const
                    {
                        return (this == &other) ||
                            (tileID == other.tileID && duration == other.duration);
                    }

                    bool operator != (const Frame& other) const
                    {
                        return !(*this == other);
                    }
                };
                eastl::vector<Frame> frames;
            }animation;
            eastl::vector<Property> properties;
            ObjectGroup objectGroup;
            eastl::string imagePath;
            Vector2u imageSize;
            /*!
            \brief The position of the tile within the image.
            */
            Vector2u imagePosition;
            eastl::string type;
        };

        /*!
        \brief Terrain information with which one
        or more tiles may be associated.
        */
        struct Terrain final
        {
            eastl::string name;
            std::uint32_t tileID = (uint32_t)-1;
            eastl::vector<Property> properties;
        };

        /*!
        \brief Declares the alignment of tile Objects
        */
        enum class ObjectAlignment
        {
            Unspecified,
            TopLeft,
            Top,
            TopRight,
            Left,
            Center,
            Right,
            BottomLeft,
            Bottom,
            BottomRight
        };

        /*!
        \brief Attempts to parse the given xml node.
        If node parsing fails an error is printed in the console
        and the Tileset remains in an uninitialised state.
        */
        void Parse(pugi::xml_node, TilemapResource*);

        /*!
        \brief Returns the first GID of this tile set.
        This the ID of the first tile in the tile set, so that
        each tile set guarantees a unique set of IDs
        */
        std::uint32_t GetFirstGID() const { return m_firstGID; }

        /*!
        \brief Returns the last GID of this tile set.
        This is the ID of the last tile in the tile set.
        */
        std::uint32_t GetLastGID() const;

        /*!
        \brief Returns the name of this tile set.
        */
        const eastl::string& GetName() const { return m_name; }

        /*!
        \brief Returns the width and height of a tile in the
        tile set, in pixels.
        */
        const Vector2u& GetTileSize() const { return m_tileSize; }

        /*!
        \brief Returns the spacing, in pixels, between each tile in the set
        */
        std::uint32_t GetSpacing() const { return m_spacing; }

        /*!
        \brief Returns the margin, in pixels, around each tile in the set
        */
        std::uint32_t GetMargin() const { return m_margin; }

        /*!
        \brief Returns the number of tiles in the tile set
        */
        std::uint32_t GetTileCount() const { return m_tileCount; }

        /*!
        \brief Returns the number of columns which make up the tile set.
        This is used when rendering collection of images sets
        */
        std::uint32_t GetColumnCount() const { return m_columnCount; }

        /*!
        \brief Returns the alignment of tile objects.
        The default value is ObjectAlignment::Unspecified for compatibility.
        When the alignment is Unspecified tile objects use BottomLeft in
        orthogonal mode and Bottom in isometric mode.
        \see ObjectAlignment
        */
        ObjectAlignment GetObjectAlignment() const { return m_objectAlignment; }

        /*!
        \brief Returns the tile offset in pixels.
        Tile will draw tiles offset from the top left using this value.
        */
        const Vector2u& GetTileOffset() const { return m_tileOffset; }

        /*!
        \brief Returns a reference to the list of Property objects for this
        tile set
        */
        const eastl::vector<Property>& GetProperties() const { return m_properties; }

        /*!
        \brief Returns the file path to the tile set image, relative to the
        working directory. Use this to load the texture required by whichever
        method you choose to render the map.
        */
        const eastl::string& GetImagePath() const { return m_imagePath; }

        /*!
        \brief Returns the size of the tile set image in pixels.
         */
        const Vector2u& GetImageSize() const { return m_imageSize; }

        /*!
        \brief Returns the colour used by the tile map image to represent transparency.
        By default this is a transparent colour (0, 0, 0, 0)
        */
        const Color& GetTransparencyColor() const { return m_transparencyColor; }

        /*!
        \brief Returns true if the image used by this tileset specifically requests
        a colour to use as transparency.
        */
        bool HasTransparency() const { return m_hasTransparency; }

        /*!
        \brief Returns a vector of Terrain types associated with one
        or more tiles within this tile set
        */
        const eastl::vector<Terrain>& GetTerrainTypes() const { return m_terrainTypes; }

        /*!
        \brief Returns a reference to the vector of tile data used by
        tiles which make up this tile set.
        */
        const eastl::vector<Tile>& GetTiles() const { return m_tiles; }

        /*!
         \brief Checks if a tiled ID is in the range of the first ID and the last ID
         \param id Tile ID
         \return
         */
        bool HasTile(std::uint32_t id) const { return id >= m_firstGID && id <= GetLastGID(); };

        /*!
         \brief queries tiles and returns a tile with the given ID. Checks if the TileID is part of the Tileset with `hasTile(id)`
         \param id Tile ID. The Tile ID will be corrected internally.
         \return In case of a success it returns the correct tile. In terms of failure it will return a nullptr.
         */
        const Tile* GetTile(std::uint32_t id) const;

    private:

        eastl::string m_workingDir;

        std::uint32_t m_firstGID;
        eastl::string m_source;
        eastl::string m_name;
        Vector2u m_tileSize;
        std::uint32_t m_spacing;
        std::uint32_t m_margin;
        std::uint32_t m_tileCount;
        std::uint32_t m_columnCount;
        ObjectAlignment m_objectAlignment;
        Vector2u m_tileOffset;

        eastl::vector<Property> m_properties;
        eastl::string m_imagePath;
        Vector2u m_imageSize;
        Color m_transparencyColor;
        bool m_hasTransparency;

        eastl::vector<Terrain> m_terrainTypes;
        eastl::vector<Tile> m_tiles;

        void Reset();

        void ParseOffsetNode(const pugi::xml_node&);
        void ParsePropertyNode(const pugi::xml_node&);
        void ParseTerrainNode(const pugi::xml_node&);
        void ParseTileNode(const pugi::xml_node&, TilemapResource*);
        void CreateMissingTile(std::uint32_t ID);
    };
}
