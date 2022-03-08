#include "EXEPCH.h"
#include "source/engine/gameobjectsystem/components/componenttypes/TilemapComponent.h"
#include "source/resource/ResourceHandle.h"
#include "source/engine/resources/resourcetypes/tilemap/TilemapResource.h"

#include "source/engine/gameobjectsystem/GameObject.h"
#include "source/render/RenderManager.h"

#include "source/engine/resources/resourcetypes/tilemap/tilemapinternals/TileLayer.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    bool TilemapComponent::Initialize()
    {
        EXE_ASSERT(m_pOwner);
        return true;
    }

    bool TilemapComponent::Initialize(const rapidjson::Value& jsonComponentData)
    {
        EXE_ASSERT(m_pOwner);

        auto tilemapFilepath = jsonComponentData.FindMember("file");

        EXE_ASSERT(tilemapFilepath != jsonComponentData.MemberEnd());
        EXE_ASSERT(tilemapFilepath->value.IsString());

        m_tilemapSourceID = tilemapFilepath->value.GetString();

        ResourceHandle tilemapResource(m_tilemapSourceID);

        tilemapResource.QueueLoad(true);
        tilemapResource.LockResource();

        return true;
    }

    void TilemapComponent::Update()
    {
        if (!m_pOwner)
        {
            m_gameObjectSystemLog.Fatal("Owner of UIComponent was nullptr. This should NEVER happen.");
            return;
        }

        if (!m_pOwner->IsEnabled())
            return;

        ResourceHandle tilemapResource(m_tilemapSourceID);

        auto* pMap = tilemapResource.GetAs<TilemapResource>();

        if (!pMap)
            return;

        if (!m_loadingComplete)
            PopulateTileMap();

    }

    void TilemapComponent::Render() const
    {
        if (!m_pOwner)
        {
            m_gameObjectSystemLog.Fatal("Owner of UIComponent was nullptr. This should NEVER happen.");
            return;
        }

        if (!m_pOwner->IsEnabled())
            return;

        auto* pRenderManager = RenderManager::GetInstance();
        if (!pRenderManager)
            return;

        for (const auto& tile : m_tiles)
        {
            if (tile.tileID == 0)
                continue;

            RenderCommand command;
            command.m_destinationFrame = tile.relativePositionRect; // * Transform component pos
            command.m_sourceFrame = tile.sourceImageRect;
            command.m_texture = tile.tilesetTextureResource;
            command.m_renderLayer = RenderCommand::RenderLayer::World;

            pRenderManager->PushRenderCommand(command);
        }
    }

    void TilemapComponent::Destroy()
    {
        ResourceHandle tilemapResource(m_tilemapSourceID);
        tilemapResource.UnlockResource();
    }

    void TilemapComponent::PopulateTileMap()
    {
        ResourceHandle tilemapResource(m_tilemapSourceID);

        auto* pMap = tilemapResource.GetAs<TilemapResource>();

        if (!pMap)
            return;

        unsigned int mapWidth = pMap->GetTileCount().x;
        unsigned int mapHeight = pMap->GetTileCount().y;
        m_tiles.resize(mapWidth * mapHeight);

        unsigned int tileWidth = pMap->GetTileSize().w;
        unsigned int tileHeight = pMap->GetTileSize().h;

        const auto& layers = pMap->GetMapLayers();
        for (const auto& layer : layers)
        {
            if (layer->GetType() == Layer::Type::Tile)
            {
                const auto& tileLayer = layer->GetLayerAs<TileLayer>();
                const auto& tiles = tileLayer.GetTiles();
                for (size_t i = 0; i < m_tiles.size(); ++i)
                {
                    m_tiles[i].tileID = tiles[i].ID;
                    m_tiles[i].relativePositionRect.m_left = ((i % mapWidth) * tileWidth);
                    m_tiles[i].relativePositionRect.m_top = ((i / mapWidth) * tileHeight);
                    m_tiles[i].relativePositionRect.m_width = tileWidth;
                    m_tiles[i].relativePositionRect.m_height = tileHeight;
                }

                break;
            }
        }

        const auto& tilesets = pMap->GetMapTilesets();
        for (const auto& tileset : tilesets)
        {
            unsigned int tilesetTileWidth = tileset.GetTileSize().w;
            unsigned int tilesetTileHeight = tileset.GetTileSize().h;
            unsigned int tilesetColumns = tileset.GetColumnCount();

            ResourceHandle tilesetHandle(tileset.GetImagePath());
            tilesetHandle.QueueLoad(true);
            tilesetHandle.LockResource();

            for (auto& tile : m_tiles)
            {
                if (tile.tileID >= tileset.GetFirstGID() && tile.tileID <= tileset.GetLastGID())
                {
                    tile.tilesetTextureResource = tileset.GetImagePath();
                    tile.sourceImageRect.m_left = ((tile.tileID - 1) % tilesetColumns) * tilesetTileWidth;
                    tile.sourceImageRect.m_top = (((tile.tileID - 1) / tilesetColumns) * tilesetTileHeight);
                    tile.sourceImageRect.m_width = tilesetTileWidth;
                    tile.sourceImageRect.m_height = tilesetTileHeight;
                }
            }
        }

        m_loadingComplete = true;
    }
}