#pragma once

#include "Layer.h"
#include "source/utility/generic/Macros.h"
#include "source/utility/containers/Vector2.h"

#include <EASTL/string.h>
#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    /*!
    \brief A layer made up from a series of tile sets
    */
    class TileLayer final : public Layer
    {
    public:
        /*!
        \brief Tile information for a layer
        */
        struct Tile final
        {
            std::uint32_t ID = 0; //!< Global ID of the tile
            std::uint8_t flipFlags = 0; //!< Flags marking if the tile should be flipped when drawn
        };

        /*!
        \brief Represents a chunk of tile data, if this is an infinite map
        */
        struct Chunk final
        {
            Vector2i position; //<! coordinate in tiles, not pixels
            Vector2i size; //!< size in tiles, not pixels
            eastl::vector<Tile> tiles;
        };

        /*!
        \brief Flags used to tell if a tile is flipped when drawn
        */
        enum FlipFlag
        {
            Horizontal = 0x8,
            Vertical = 0x4,
            Diagonal = 0x2
        };

        explicit TileLayer(std::size_t);

        Type GetType() const override { return Layer::Type::Tile; }
        void Parse(const pugi::xml_node&, TilemapResource*) override;

        /*!
        \brief Returns the list of tiles used to make up the layer
        If this is empty then the map is most likely infinite, in
        which case the tile data is stored in chunks.
        \see getChunks()
        */
        const eastl::vector<Tile>& GetTiles() const { return m_tiles; }

        /*!
        \brief Returns a vector of chunks which make up this layer
        if the map is set to infinite. This will be empty if the map
        is not infinite.
        \see getTiles()
        */
        const eastl::vector<Chunk>& GetChunks() const { return m_chunks; }

    private:
        eastl::vector<Tile> m_tiles;
        eastl::vector<Chunk> m_chunks;
        std::size_t m_tileCount;

        void ParseBase64(const pugi::xml_node&);
        void ParseCSV(const pugi::xml_node&);
        void ParseUnencoded(const pugi::xml_node&);

        void CreateTiles(const eastl::vector<std::uint32_t>&, eastl::vector<Tile>& destination);
    };

    template <>
    inline TileLayer& Layer::GetLayerAs<TileLayer>()
    {
        EXE_ASSERT(GetType() == Type::Tile);
        return *static_cast<TileLayer*>(this);
    }
}