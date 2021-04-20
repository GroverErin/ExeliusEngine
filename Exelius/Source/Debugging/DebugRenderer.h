#pragma once
#include "Source/Utility/Generic/Singleton.h"
#include "Source/Utility/Containers/Vector2.h"
#include "Source/Utility/Generic/Color.h"

namespace Exelius
{
	class DebugRenderer
		: public Singleton<DebugRenderer>
	{
	public:
		void AddLine(const Vector2f& fromPosition, const Vector2f& toPosition, Color color, float lineWidth = 1.0f, float duration = 0.0f, bool depthEnabled = true);
		void AddCross(const Vector2f& position, Color color, float size, float duration = 0.0f, bool depthEnabled = true);
		void AddCircle(const Vector2f& centerPosition, float radius, Color color, float duration = 0.0f, bool depthEnabled = true);
		void AddTriangle(const Vector2f& vertex1, const Vector2f& vertex2, const Vector2f& vertex3, Color color, float lineWidth = 1.0f, float duration = 0.0f, bool depthEnabled = true);
		void AddAABB(const Vector2f& position, const Vector2f& size, Color color, float lineWidth = 1.0f, float duration = 0.0f, bool depthEnabled = true);
		void AddString(const Vector2f& position, const char* text, Color color, float duration = 0.0f, bool depthEnabled = true);
	};
}