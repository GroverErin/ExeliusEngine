#pragma once
#include "source/resource/ResourceHelpers.h"
#include "source/utility/math/Rectangle.h"
#include "source/utility/generic/Color.h"

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
		ResourceID m_texture;
		Color m_tint;

		FRectangle m_sourceFrame;
		FRectangle m_destinationFrame;
		int m_zOrder;

		bool operator<(const RenderCommand& command) const;
	};
}