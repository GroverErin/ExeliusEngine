#pragma once

#include "source/resource/Resource.h"

#include "source/utility/containers/Vector2.h"
#include "source/utility/math/Rectangle.h"
#include "source/utility/generic/Color.h"

#include "tilemapinternals/Tileset.h"
#include "tilemapinternals/Layer.h"
#include "tilemapinternals/Property.h"
#include "tilemapinternals/Object.h"

#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>
#include <EASTL/hash_map.h>
#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    /// <summary>
    /// Holds the XML version of the tilemap.
    /// </summary>
    struct Version final
    {
        std::uint16_t majorVersion;
        std::uint16_t minorVersion;

        Version(std::uint16_t majorVer = 0, std::uint16_t minorVer = 0)
            : majorVersion(majorVer), minorVersion(minorVer)
        {
            //
        }
    };

    /// <summary>
    /// The possible orientations of a tilemap.
    /// </summary>
    enum class Orientation
    {
        Orthogonal,
        Isometric,
        Staggered,
        Hexagonal,
        None
    };

    /// <summary>
    /// The possible render order of the tiles in a tilemap.
    /// </summary>
    enum class RenderOrder
    {
        RightDown,
        RightUp,
        LeftDown,
        LeftUp,
        None
    };

    /// <summary>
    /// The possible axis of stagger of a tilemap.
    /// </summary>
    enum class StaggerAxis
    {
        X,
        Y,
        None
    };

    /// <summary>
    /// The possible stagger index of a tilemap.
    /// </summary>
    enum class StaggerIndex
    {
        Even,
        Odd,
        None
    };

	class TilemapResource
		: public Resource
	{
        Version m_version;
        Orientation m_orientation;
        RenderOrder m_renderOrder;
        bool m_isInfinite;

        Vector2u m_tileCount;
        Vector2u m_tileSize;

        float m_hexSideLength;
        StaggerAxis m_staggerAxis;
        StaggerIndex m_staggerIndex;

        Color m_backgroundColor;

        eastl::string m_workingDirectory;

        eastl::vector<Tileset> m_tilesets;
        eastl::vector<Layer::Ptr> m_layers;
        eastl::vector<Property> m_properties;
        eastl::hash_map<std::uint32_t, Tileset::Tile> m_animTiles;

        eastl::unordered_map<eastl::string, Object> m_templateObjects;
        eastl::unordered_map<eastl::string, Tileset> m_templateTilesets;

	public:
		TilemapResource(const ResourceID& id);
		TilemapResource(const TilemapResource&) = delete;
		TilemapResource(TilemapResource&&) = delete;
		TilemapResource& operator=(const TilemapResource&) = delete;
		virtual ~TilemapResource() = default;

		virtual LoadResult Load(eastl::vector<std::byte>&& data) final override;
        virtual void Unload() final override;

        /// <summary>
        /// Loads a map from a document stored in a string
        /// </summary>
        /// <param name="data">A eastl::string containing the map data to load.</param>
        /// <param name="workingDir">A eastl::string containing the working directory in which to find assets such as tile sets or images.</param>
        /// <returns>true if successful, else false.</returns>
        bool LoadMapFromStringData(const eastl::string& data, const eastl::string& workingDir);

        /// <summary>
        /// Returns the version of the tile map last parsed.
        /// If no tile map has yet been parsed the version will read 0, 0.
        /// </summary>
        /// <returns>Returns the version of the tile map last parsed.</returns>
        const Version& GetMapVersion() const { return m_version; }

        /// <summary>
        /// Returns the orientation of the map if one is loaded,
        /// else returns None
        /// </summary>
        /// <returns>Returns the orientation of the map.</returns>
        Orientation GetMapOrientation() const { return m_orientation; }
        
        /// <summary>
        /// Returns the RenderOrder of the map if one is loaded,
        /// else returns None.
        /// </summary>
        /// <returns>Returns the RenderOrder of the map.</returns>
        RenderOrder GetTileRenderOrder() const { return m_renderOrder; }

        /// <summary>
        /// Returns the tile count of the map in the X and Y directions.
        /// </summary>
        /// <returns>Returns the tile count of the map in the X and Y directions.</returns>
        const Vector2u& GetTileCount() const { return m_tileCount; }

        /// <summary>
        /// Returns the size of the tile grid in this map.
        /// Actual tile sizes may vary and will be extended / shrunk about
        /// the bottom left corner of the tile.
        /// </summary>
        /// <returns>Returns the size of the tile grid in this map.</returns>
        const Vector2u& GetTileSize() const { return m_tileSize; }

        /// <summary>
        /// Returns the bounds of the map.
        /// </summary>
        /// <returns>Returns the bounds of the map.</returns>
        FRectangle GetMapBounds() const { return FRectangle(0.f, 0.f, static_cast<float>(m_tileCount.x * m_tileSize.x), static_cast<float>(m_tileCount.y * m_tileSize.y)); }

        /// <summary>
        /// Returns the length of an edge of a tile if a Hexagonal
        /// map is loaded.
        /// The length returned is in pixels of the straight edge running
        /// along the axis returned by getStaggerAxis().If no map is loaded
        /// or the loaded map is not of Hexagonal orientation this function
        /// returns 0.f
        /// </summary>
        /// <returns>Returns the length of an edge of a Hexagonal tile</returns>
        float GetHexSideLength() const { return m_hexSideLength; }

        /// <summary>
        /// Stagger axis of the map.
        /// If either a Staggered or Hexagonal tile map is loaded this returns
        /// which axis the map is staggered along, else returns None.
        /// </summary>
        /// <returns>Returns the stagger axis of the map.</returns>
        StaggerAxis GetMapStaggerAxis() const { return m_staggerAxis; }

        /// <summary>
        /// Stagger Index of the loaded map.
        /// If a Staggered or Hexagonal map is loaded this returns whether
        /// the even or odd rows of tiles are staggered, otherwise it returns None.
        /// </summary>
        /// <returns>Returns the stagger Index of the loaded map.</returns>
        StaggerIndex GetMapStaggerIndex() const { return m_staggerIndex; }

        /// <summary>
        /// Returns the background colour of the map.
        /// </summary>
        /// <returns>Returns the background colour of the map.</returns>
        const Color& GetBackgroundColor() const { return m_backgroundColor; }

        /// <summary>
        /// Returns a reference to the vector of tile sets used by the map
        /// </summary>
        /// <returns>Returns the maps tilesets.</returns>
        const eastl::vector<Tileset>& GetMapTilesets() const { return m_tilesets; }

        /// <summary>
        /// Returns a reference to the vector containing the layer data.
        /// Layers are pointer-to-baseclass, the concrete type of which can be
        /// found via Layer::getType()
        /// 
        /// @see Layer
        /// </summary>
        /// <returns>Returns the vector containing the layer data.</returns>
        const eastl::vector<Layer::Ptr>& GetMapLayers() const { return m_layers; }

        /// <summary>
        /// Returns a vector of Property objects loaded by the map
        /// </summary>
        /// <returns>Returns a vector of Property objects of the map.</returns>
        const eastl::vector<Property>& GetMapPropertyObjects() const { return m_properties; }

        /// <summary>
        /// Returns a Hashmap of all animated tiles accessible by TileID
        /// </summary>
        /// <returns>Returns a Hashmap of all animated tiles accessible by TileID.</returns>
        const eastl::hash_map<std::uint32_t, Tileset::Tile>& GetAnimatedTiles() const { return m_animTiles; }

        /// <summary>
        /// Returns the current working directory of the map. Images and
        /// other resources are loaded relative to this.
        /// </summary>
        /// <returns>Returns the current working directory of the map.</returns>
        const eastl::string& GetWorkingDirectory() const { return m_workingDirectory; }

        /// <summary>
        /// Returns an unordered_map of template objects indexed by file name.
        /// </summary>
        /// <returns>Returns an unordered_map of template objects indexed by file name.</returns>
        eastl::unordered_map<eastl::string, Object>& GetTemplateObjects() { return m_templateObjects; }

        /// <summary>
        /// Returns an unordered_map of template objects indexed by file name.
        /// const.
        /// </summary>
        /// <returns>Returns an unordered_map of template objects indexed by file name.</returns>
        const eastl::unordered_map<eastl::string, Object>& GetTemplateObjects() const { return m_templateObjects; }

        /// <summary>
        /// Returns an unordered_map of tilesets used by templated objects.
        /// If Object::getTilesetName() is not empty it can be used to retreive a tileset
        /// from this map. Otherwise the object's tileset can be found from in the map's
        /// global tilesets returned by getTilesets().
        /// </summary>
        /// <returns>Returns an unordered_map of tilesets used by templated objects.</returns>
        eastl::unordered_map<eastl::string, Tileset>& GetTemplateTilesets() { return m_templateTilesets; }

        /// <summary>
        /// Returns an unordered_map of tilesets used by templated objects.
        /// If Object::getTilesetName() is not empty it can be used to retreive a tileset
        /// from this map. Otherwise the object's tileset can be found from in the map's
        /// global tilesets returned by getTilesets().
        /// </summary>
        /// <returns>Returns an unordered_map of tilesets used by templated objects.</returns>
        const eastl::unordered_map<eastl::string, Tileset>& GetTemplateTilesets() const { return m_templateTilesets; }

        /// <summary>
        /// Returns true if this is in infinite tile map.
        /// Infinite maps store their tile data in for tile layers in chunks. If
        /// this is an infinite map use TileLayer::getChunks() to get tile IDs
        /// rather than TileLayer::getTiles().
        /// 
        /// @see TileLayer
        /// </summary>
        /// <returns>Returns true if this is in infinite tile map.</returns>
        bool IsInfinite() const { return m_isInfinite; }

    private:

        /// <summary>
        /// Parses the root "map" xml node to populate the TilemapResource.
        /// </summary>
        /// <param name="">The root "map" node.</param>
        /// <returns>True if sucessful, false otherwise.</returns>
        bool ParseRootMapNode(const pugi::xml_node&);

        //always returns false so we can return this
        //on load failure

        /// <summary>
        /// Resets all the TilemapResource members.
        /// Always returns false so we can return this
        /// on load failure.
        /// </summary>
        /// <returns>Returns false.</returns>
        bool ResetTilemapData();
	};
}