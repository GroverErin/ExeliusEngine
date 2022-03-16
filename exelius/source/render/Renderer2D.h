#pragma once
#include "source/utility/generic/Singleton.h"
#include "source/utility/generic/SmartPointers.h"
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/render/camera/OrthographicCamera.h"
#include "source/render/camera/Camera.h"
#include "source/render/camera/EditorCamera.h"

#include <glm/glm.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(RendererAPI);
	FORWARD_DECLARE(Texture);
	class SubTexture;
	struct SpriteRendererComponent;

	class Renderer2D
		: public Singleton<Renderer2D>
	{
		RendererAPI* m_pRendererAPI;
	public:
		// Stats
		struct Statistics
		{
			uint32_t m_drawCalls = 0;
			uint32_t m_quadCount = 0;

			uint32_t GetTotalVertexCount() const { return m_quadCount * 4; }
			uint32_t GetTotalIndexCount() const { return m_quadCount * 6; }
		};

		void Initialize();
		void Shutdown();

		void BeginScene(const Camera& camera, const glm::mat4& transform);
		void BeginScene(const EditorCamera& camera);
		void BeginScene(const OrthographicCamera& camera); // TODO: Remove
		void EndScene();
		void Flush();

		// Primitives
		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const SharedPtr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		void DrawQuad(const glm::vec3& position, const glm::vec2& size, const SharedPtr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const SharedPtr<SubTexture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		void DrawQuad(const glm::vec3& position, const glm::vec2& size, const SharedPtr<SubTexture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		void DrawQuad(const glm::mat4& transform, const SharedPtr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);
		void DrawQuad(const glm::mat4& transform, const SharedPtr<SubTexture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), int entityID = -1);

		void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const SharedPtr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const SharedPtr<Texture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const SharedPtr<SubTexture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const SharedPtr<SubTexture>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

		void ResetStats();
		Statistics GetStats();

	private:
		void StartBatch();
		void NextBatch();
	};
}
