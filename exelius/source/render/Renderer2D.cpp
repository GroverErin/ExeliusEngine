#include "EXEPCH.h"
#include "Renderer2D.h"
#include "source/utility/generic/SmartPointers.h"

#include "source/render/RendererAPI.h"
#include "source/render/VertexArray.h"
#include "source/render/Buffer.h"
#include "source/render/Shader.h"
#include "source/render/Texture.h"
#include "source/render/UniformBuffer.h"
#include "source/render/SubTexture.h"

// TODO: This is NOT within the render engine layer... maybe Renderer should be base class and Renderer2D should exist in Engine layer
#include "source/engine/scene/Components.h"

#include <EASTL/array.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct QuadVertex
	{
		glm::vec3 m_position;
		glm::vec4 m_color;
		glm::vec2 m_textureCoord;
		float m_textureIndex;
		float m_tilingFactor;

		// Editor-only
		int m_entityID;
	};

	struct Renderer2DData
	{
		static const uint32_t s_kMaxQuads = 20000;
		static const uint32_t s_kMaxVertices = s_kMaxQuads * 4;
		static const uint32_t s_kMaxIndices = s_kMaxQuads * 6;
		static const uint32_t s_kMaxTextureSlots = 32; // TODO: RenderCaps

		SharedPtr<VertexArray> m_pQuadVertexArray;
		SharedPtr<VertexBuffer> m_pQuadVertexBuffer;
		SharedPtr<Shader> m_pTextureShader;
		SharedPtr<Texture> m_pWhiteTexture;

		uint32_t m_quadIndexCount = 0;
		QuadVertex* m_pQuadVertexBufferBase = nullptr;
		QuadVertex* m_pQuadVertexBufferPtr = nullptr;

		eastl::array<SharedPtr<Texture>, s_kMaxTextureSlots> m_textureSlots;
		uint32_t m_textureSlotIndex = 1; // 0 = white texture

		glm::vec4 m_quadVertexPositions[4];

		Renderer2D::Statistics m_stats;

		struct CameraData
		{
			glm::mat4 m_viewProjection;
		};
		CameraData m_cameraBuffer;
		SharedPtr<UniformBuffer> m_pCameraUniformBuffer;
	};

	static Renderer2DData s_data;

	void Renderer2D::Initialize()
	{
		s_data.m_pQuadVertexArray = MakeShared<VertexArray>();

		s_data.m_pQuadVertexBuffer = MakeShared<VertexBuffer>(s_data.s_kMaxVertices * sizeof(QuadVertex));
		s_data.m_pQuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Int,    "a_EntityID"     }
			});
		s_data.m_pQuadVertexArray->AddVertexBuffer(s_data.m_pQuadVertexBuffer);

		s_data.m_pQuadVertexBufferBase = new QuadVertex[s_data.s_kMaxVertices];

		uint32_t* quadIndices = new uint32_t[s_data.s_kMaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.s_kMaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		SharedPtr<IndexBuffer> quadIB = MakeShared<IndexBuffer>(quadIndices, s_data.s_kMaxIndices);
		s_data.m_pQuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_data.m_pWhiteTexture = MakeShared<Texture>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.m_pWhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_data.s_kMaxTextureSlots];
		for (uint32_t i = 0; i < s_data.s_kMaxTextureSlots; ++i)
			samplers[i] = i;

		s_data.m_pTextureShader = MakeShared<Shader>("assets/shaders/texture.glsl");

		// Set first texture slot to 0
		s_data.m_textureSlots[0] = s_data.m_pWhiteTexture;

		s_data.m_quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.m_quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.m_quadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_data.m_quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_data.m_pCameraUniformBuffer = MakeShared<UniformBuffer>(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_data.m_pQuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_data.m_cameraBuffer.m_viewProjection = camera.GetViewProjectionMatrix();
		s_data.m_pCameraUniformBuffer->SetData(&s_data.m_cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		s_data.m_cameraBuffer.m_viewProjection = camera.GetProjection() * glm::inverse(transform);
		s_data.m_pCameraUniformBuffer->SetData(&s_data.m_cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		s_data.m_cameraBuffer.m_viewProjection = camera.GetViewProjection();
		s_data.m_pCameraUniformBuffer->SetData(&s_data.m_cameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_data.m_quadIndexCount = 0;
		s_data.m_pQuadVertexBufferPtr = s_data.m_pQuadVertexBufferBase;

		s_data.m_textureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (s_data.m_quadIndexCount == 0)
			return; // Nothing to draw

		uint32_t dataSize = (uint32_t)((uint8_t*)s_data.m_pQuadVertexBufferPtr - (uint8_t*)s_data.m_pQuadVertexBufferBase);
		s_data.m_pQuadVertexBuffer->SetData(s_data.m_pQuadVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_data.m_textureSlotIndex; ++i)
			s_data.m_textureSlots[i]->Bind(i);

		s_data.m_pTextureShader->Bind();
		m_pRendererAPI->DrawIndexed(s_data.m_pQuadVertexArray, s_data.m_quadIndexCount);
		++s_data.m_stats.m_drawCalls;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const SharedPtr<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const SharedPtr<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const SharedPtr<SubTexture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture->GetTexture(), tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const SharedPtr<SubTexture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad(position, size, texture->GetTexture(), tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_data.m_quadIndexCount >= Renderer2DData::s_kMaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; ++i)
		{
			s_data.m_pQuadVertexBufferPtr->m_position = transform * s_data.m_quadVertexPositions[i];
			s_data.m_pQuadVertexBufferPtr->m_color = color;
			s_data.m_pQuadVertexBufferPtr->m_textureCoord = textureCoords[i];
			s_data.m_pQuadVertexBufferPtr->m_textureIndex = textureIndex;
			s_data.m_pQuadVertexBufferPtr->m_tilingFactor = tilingFactor;
			s_data.m_pQuadVertexBufferPtr->m_entityID = entityID;
			++s_data.m_pQuadVertexBufferPtr;
		}

		s_data.m_quadIndexCount += 6;

		++s_data.m_stats.m_quadCount;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const SharedPtr<Texture>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_data.m_quadIndexCount >= Renderer2DData::s_kMaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_data.m_textureSlotIndex; ++i)
		{
			if (*s_data.m_textureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_data.m_textureSlotIndex >= Renderer2DData::s_kMaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_data.m_textureSlotIndex;
			s_data.m_textureSlots[s_data.m_textureSlotIndex] = texture;
			++s_data.m_textureSlotIndex;
		}

		for (size_t i = 0; i < quadVertexCount; ++i)
		{
			s_data.m_pQuadVertexBufferPtr->m_position = transform * s_data.m_quadVertexPositions[i];
			s_data.m_pQuadVertexBufferPtr->m_color = tintColor;
			s_data.m_pQuadVertexBufferPtr->m_textureCoord = textureCoords[i];
			s_data.m_pQuadVertexBufferPtr->m_textureIndex = textureIndex;
			s_data.m_pQuadVertexBufferPtr->m_tilingFactor = tilingFactor;
			s_data.m_pQuadVertexBufferPtr->m_entityID = entityID;
			++s_data.m_pQuadVertexBufferPtr;
		}

		s_data.m_quadIndexCount += 6;

		++s_data.m_stats.m_quadCount;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const SharedPtr<SubTexture>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		DrawQuad(transform, texture->GetTexture(), tilingFactor, tintColor, entityID);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const SharedPtr<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const SharedPtr<Texture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const SharedPtr<SubTexture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture->GetTexture(), tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const SharedPtr<SubTexture>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad(position, size, rotation, texture->GetTexture(), tilingFactor, tintColor);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Texture)
			DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_data.m_stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_data.m_stats;
	}
}
