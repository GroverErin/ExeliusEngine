#pragma once
#include "source/utility/generic/Singleton.h"

#include "source/render/Renderer.h"
#include "source/resource/ResourceHandle.h"
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/Color.h"
#include "source/utility/generic/SmartPointers.h"

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
	FORWARD_DECLARE(VertexArray);
	FORWARD_DECLARE(VertexBuffer);
	FORWARD_DECLARE(UniformBuffer);
	FORWARD_DECLARE(IndexBuffer);
	class SubTexture;
	struct SpriteRendererComponent;

	class Renderer2D
		: public Singleton<Renderer2D>, public Renderer
	{
		static const uint32_t s_kMaxQuads = 20000;
		static const uint32_t s_kMaxVertices = s_kMaxQuads * 4;
		static const uint32_t s_kMaxIndices = s_kMaxQuads * 6;
		static const uint32_t s_kMaxTextureSlots = 32; // TODO: Determine maximum from hardware?

		struct CircleVertex
		{
			glm::vec3 m_worldPosition;
			glm::vec3 m_localPosition;
			glm::vec4 m_color;
			float m_thickness;
			float m_fade;

			// TODO: Editor-only
			int m_gameObjectGUID;
		};

		struct QuadVertex
		{
			glm::vec3 m_position;
			glm::vec4 m_color;
			glm::vec2 m_textureCoord;
			float m_textureIndex;
			float m_tilingFactor;

			// TODO: Editor-only
			int m_gameObjectGUID;
		};

		struct LineVertex
		{
			glm::vec3 m_position;
			glm::vec4 m_color;

			// TODO: Editor-only
			int m_gameObjectGUID;
		};

		/// <summary>
		/// This should be a pointer so we can forward declare... dont want OpenGL stuff in the header.
		/// </summary>
		RendererAPI* m_pRendererAPI;

		// Vertex array containing quad vertex buffers.
		SharedPtr<VertexArray> m_pQuadVertexArray;
		SharedPtr<VertexBuffer> m_pQuadVertexBuffer;

		ResourceHandle m_quadShaderResource;
		ResourceHandle m_whiteTextureResource;

		SharedPtr<VertexArray> m_pCircleVertexArray;
		SharedPtr<VertexBuffer> m_pCircleVertexBuffer;
		ResourceHandle m_circleShaderResource;

		SharedPtr<VertexArray> m_pLineVertexArray;
		SharedPtr<VertexBuffer> m_pLineVertexBuffer;
		ResourceHandle m_lineShaderResource;

		uint32_t m_quadIndexCount;
		QuadVertex* m_pQuadVertexBufferBase;
		QuadVertex* m_pQuadVertexBufferPtr;

		uint32_t m_circleIndexCount;
		CircleVertex* m_pCircleVertexBufferBase;
		CircleVertex* m_pCircleVertexBufferPtr;

		uint32_t m_lineVertexCount;
		LineVertex* m_pLineVertexBufferBase;
		LineVertex* m_pLineVertexBufferPtr;

		float m_lineWidth;

		eastl::array<ResourceID, s_kMaxTextureSlots> m_textureSlots;
		uint32_t m_textureSlotIndex;

		struct CameraData
		{
			glm::mat4 m_viewProjection;
		};
		CameraData m_cameraBuffer;

		// Pointer so we can forward declare.
		UniformBuffer* m_pCameraUniformBuffer;
	public:

		Renderer2D(eastl::string& windowTitle, const Vector2u& windowSize, bool isVSyncEnabled = true);
		~Renderer2D();

		void Initialize();
		void Shutdown();

		void Begin2DScene(const Camera& camera, const glm::mat4& transform);
		void Begin2DScene(const EditorCamera& camera);
		void End2DScene();
		void Flush();

		// Primitives
		void DrawQuad(const glm::vec2& position, const glm::vec2& size, Color color);
		void DrawQuad(const glm::vec3& position, const glm::vec2& size, Color color);
		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const ResourceID& textureResource, float tilingFactor = 1.0f, Color tintColor = glm::vec4(1.0f));
		void DrawQuad(const glm::vec3& position, const glm::vec2& size, const ResourceID& textureResource, float tilingFactor = 1.0f, Color tintColor = glm::vec4(1.0f));

		void DrawQuad(const glm::vec2& position, const glm::vec2& size, const SharedPtr<SubTexture>& texture, float tilingFactor = 1.0f, Color tintColor = glm::vec4(1.0f));
		void DrawQuad(const glm::vec3& position, const glm::vec2& size, const SharedPtr<SubTexture>& texture, float tilingFactor = 1.0f, Color tintColor = glm::vec4(1.0f));

		void DrawQuad(const glm::mat4& transform, Color color, int m_gameObjectGUID = -1);
		void DrawQuad(const glm::mat4& transform, const ResourceID& textureResource, float tilingFactor = 1.0f, Color tintColor = glm::vec4(1.0f), int m_gameObjectGUID = -1);
		void DrawQuad(const glm::mat4& transform, const SharedPtr<SubTexture>& texture, float tilingFactor = 1.0f, Color tintColor = glm::vec4(1.0f), int m_gameObjectGUID = -1);

		void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Color color);
		void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Color color);
		void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ResourceID& textureResource, float tilingFactor = 1.0f, Color tintColor = glm::vec4(1.0f));
		void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ResourceID& textureResource, float tilingFactor = 1.0f, Color tintColor = glm::vec4(1.0f));

		void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const SharedPtr<SubTexture>& texture, float tilingFactor = 1.0f, Color tintColor = glm::vec4(1.0f));
		void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const SharedPtr<SubTexture>& texture, float tilingFactor = 1.0f, Color tintColor = glm::vec4(1.0f));

		void DrawCircle(const glm::mat4& transform, Color color, float thickness = 1.0f, float fade = 0.005f, int m_gameObjectGUID = -1);

		void DrawLine(const glm::vec3& p0, glm::vec3& p1, Color color, int m_gameObjectGUID = -1);

		void DrawRect(const glm::vec3& position, const glm::vec2& size, Color color, int m_gameObjectGUID = -1);
		void DrawRect(const glm::mat4& transform, Color color, int m_gameObjectGUID = -1);

		void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int m_gameObjectGUID);

		float GetLineWidth();
		void SetLineWidth(float width);

	private:
		const SharedPtr<IndexBuffer> MakeIndexBuffer();
		void InitializeQuadRendering(const SharedPtr<IndexBuffer>& pIndexBuffer);
		void InitializeCircleRendering(const SharedPtr<IndexBuffer>& pIndexBuffer);
		void InitializeLineRendering();
		void InitializeWhiteTexture();
		void InitializeShaders();

		void StartBatch();
		void NextBatch();

		void FlushQuads();
		void FlushCircles();
		void FlushLines();

		void BindShader(ResourceHandle& shaderResource);
	};
}
