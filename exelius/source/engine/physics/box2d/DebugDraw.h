#pragma once

#include <box2d/b2_draw.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	class DebugDraw
		: public b2Draw
	{
	public:
		virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) final override;
					 
		virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) final override;
					 
		virtual void DrawCircle(const b2Vec2 & center, float radius, const b2Color & color) final override;
					 
		virtual void DrawSolidCircle(const b2Vec2 & center, float radius, const b2Vec2 & axis, const b2Color & color) final override;
					 
		virtual void DrawSegment(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color) final override;
					 
		virtual void DrawTransform(const b2Transform & xf) final override;
					 
		virtual void DrawPoint(const b2Vec2 & p, float size, const b2Color & color) final override;
	};
}