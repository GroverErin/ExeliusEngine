#include "EXEPCH.h"
#include "DebugDraw.h"

#include "source/engine/renderer/Renderer2D.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		eastl::array<glm::vec4, 4> vertData;
		vertData[0] = { vertices[0].x, vertices[0].y, 0.0f, 1.0f };
		vertData[1] = { vertices[1].x, vertices[1].y, 0.0f, 1.0f };
		vertData[2] = { vertices[2].x, vertices[2].y, 0.0f, 1.0f };
		vertData[3] = { vertices[3].x, vertices[3].y, 0.0f, 1.0f };

		Color colorData(255, color.b, color.b, 255);
		Renderer2D::GetInstance()->DrawRawVertexRect(vertData, colorData);
	}

	void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		eastl::array<glm::vec4, 4> vertData;
		vertData[0] = { vertices[0].x, vertices[0].y, 0.0f, 1.0f };
		vertData[1] = { vertices[1].x, vertices[1].y, 0.0f, 1.0f };
		vertData[2] = { vertices[2].x, vertices[2].y, 0.0f, 1.0f };
		vertData[3] = { vertices[3].x, vertices[3].y, 0.0f, 1.0f };

		Color colorData(255, 255, color.b, 255);
		Renderer2D::GetInstance()->DrawRawVertexRect(vertData, colorData);
	}

	void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
	{
		EXE_LOG_CATEGORY_INFO("PhysicsDebug", "Trying to draw a circle.");
	}

	void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
	{
		EXE_LOG_CATEGORY_INFO("PhysicsDebug", "Trying to draw a solid circle.");
	}

	void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		EXE_LOG_CATEGORY_INFO("PhysicsDebug", "Trying to draw a segment.");
	}

	void DebugDraw::DrawTransform(const b2Transform& xf)
	{
		EXE_LOG_CATEGORY_INFO("PhysicsDebug", "Trying to draw a transform.");
	}

	void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
	{
		EXE_LOG_CATEGORY_INFO("PhysicsDebug", "Trying to draw a point.");
	}

}