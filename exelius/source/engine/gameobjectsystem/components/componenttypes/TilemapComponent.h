#pragma once
#include "source/engine/gameobjectsystem/components/Component.h"
#include "source/resource/ResourceHelpers.h"

#include "source/render/RenderCommand.h"

#include <EASTL/vector.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct Tile
	{
		ResourceID tilesetTextureResource;
		FRectangle sourceImageRect;
		FRectangle relativePositionRect;
		uint32_t tileID;
	};

	class TilemapComponent
		: public Component
	{
		eastl::vector<Tile> m_tiles;
		ResourceID m_tilemapSourceID;
		bool m_loadingComplete;
	public:
		DEFINE_COMPONENT(TilemapComponent);

		TilemapComponent(GameObject* pOwner)
			: Component(pOwner)
			, m_loadingComplete(false)
		{
			//
		}

		virtual bool Initialize() final override;
		virtual bool Initialize(const rapidjson::Value& jsonComponentData) final override;

		virtual void Update() final override;

		virtual void Render() const final override;

		virtual void Destroy() final override;

	private:

		void PopulateTileMap();
	};
}