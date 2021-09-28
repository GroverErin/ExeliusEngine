#pragma once
#include "source/resource/ResourceHelpers.h"
#include "source/utility/math/Rectangle.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct RenderCommand
	{
		enum class RenderLayer
		{
			UIDebug,
			UI,
			WorldEffectDebug,
			WorldEffect,
			WorldDebug,
			World
		};

		RenderLayer m_renderLayer;
		ResourceID m_shader;
		ResourceID m_texture;
		IRectangle m_spriteFrame;

		FRectangle m_sourceFrame;
		FRectangle m_destinationFrame;

		Vector2f m_position;
		Vector2f m_scaleFactor;
		int m_zOrder;

		bool operator<(const RenderCommand& command) const;
	};
}