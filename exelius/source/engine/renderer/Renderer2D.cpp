#include "EXEPCH.h"
#include "Renderer2D.h"

#include "source/render/RendererAPI.h"
#include "source/render/VertexArray.h"
#include "source/render/Buffer.h"
#include "source/render/Shader.h"
#include "source/render/Texture.h"
#include "source/render/UniformBuffer.h"
#include "source/render/SubTexture.h"

#include "source/engine/resources/resourcetypes/TextureResource.h"
#include "source/engine/resources/resourcetypes/ShaderResource.h"
#include "source/engine/gameobjects/components/SpriteRendererComponent.h"

#include <EASTL/array.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	Renderer2D::Renderer2D(const WindowProperties& windowProperties)
		: Renderer(windowProperties)
		, m_pRendererAPI(nullptr)
		, m_pQuadVertexArray(nullptr)
		, m_pQuadVertexBuffer(nullptr)
		, m_quadShaderResource("assets/shaders/quadrenderer.glsl")
		, m_pCircleVertexArray(nullptr)
		, m_pCircleVertexBuffer(nullptr)
		, m_circleShaderResource("assets/shaders/circlerenderer.glsl")
		, m_pLineVertexArray(nullptr)
		, m_pLineVertexBuffer(nullptr)
		, m_lineShaderResource("assets/shaders/linerenderer.glsl")
		, m_quadIndexCount(0)
		, m_pQuadVertexBufferBase(nullptr)
		, m_pQuadVertexBufferPtr(nullptr)
		, m_circleIndexCount(0)
		, m_pCircleVertexBufferBase(nullptr)
		, m_pCircleVertexBufferPtr(nullptr)
		, m_lineVertexCount(0)
		, m_pLineVertexBufferBase(nullptr)
		, m_pLineVertexBufferPtr(nullptr)
		, m_lineWidth(2.0f)
		, m_textureSlotIndex(1)
		, m_cameraBuffer()
		, m_pCameraUniformBuffer(nullptr)
	{
		Initialize();
	}

	Renderer2D::~Renderer2D()
	{
		Shutdown();
	}

	void Renderer2D::Initialize()
	{
		SharedPtr<IndexBuffer> tpIndexBuffer = MakeIndexBuffer();

		InitializeQuadRendering(tpIndexBuffer);
		InitializeCircleRendering(tpIndexBuffer);
		InitializeLineRendering();

		InitializeWhiteTexture();

		InitializeShaders();

		m_pCameraUniformBuffer = EXELIUS_NEW(UniformBuffer(sizeof(CameraData), 0));
	}

	void Renderer2D::Shutdown()
	{
		EXELIUS_DELETE(m_pCameraUniformBuffer);
		EXELIUS_DELETE_ARRAY(m_pQuadVertexBufferBase);
		EXELIUS_DELETE_ARRAY(m_pCircleVertexBufferBase);
		EXELIUS_DELETE_ARRAY(m_pLineVertexBufferBase);

		m_pQuadVertexBufferPtr = nullptr;
		m_pCircleVertexBufferPtr = nullptr;
		m_pLineVertexBufferPtr = nullptr;
	}

	void Renderer2D::Begin2DScene(const Camera& camera, const glm::mat4& transform)
	{
		m_cameraBuffer.m_viewProjection = camera.GetProjection() * glm::inverse(transform);
		m_pCameraUniformBuffer->SetData(&m_cameraBuffer, sizeof(CameraData));

		StartBatch();
	}

	void Renderer2D::Begin2DScene(const EditorCamera& camera)
	{
		m_cameraBuffer.m_viewProjection = camera.GetViewProjection();
		m_pCameraUniformBuffer->SetData(&m_cameraBuffer, sizeof(CameraData));

		StartBatch();
	}

	void Renderer2D::End2DScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		FlushQuads();
		FlushCircles();
		FlushLines();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, Color color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Color color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const ResourceID& texture, float tilingFactor, Color tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const ResourceID& texture, float tilingFactor, Color tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const SharedPtr<SubTexture>& texture, float tilingFactor, Color tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture->GetTextureResourceID(), tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const SharedPtr<SubTexture>& texture, float tilingFactor, Color tintColor)
	{
		DrawQuad(position, size, texture->GetTextureResourceID(), tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, Color color, int m_gameObjectGUID)
	{
		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (m_quadIndexCount >= s_kMaxIndices)
			NextBatch();

		glm::vec4 vertexPositions[4];
		vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		for (size_t i = 0; i < quadVertexCount; ++i)
		{
			m_pQuadVertexBufferPtr->m_position = transform * vertexPositions[i];
			m_pQuadVertexBufferPtr->m_color = color.GetColorVector();
			m_pQuadVertexBufferPtr->m_textureCoord = textureCoords[i];
			m_pQuadVertexBufferPtr->m_textureIndex = textureIndex;
			m_pQuadVertexBufferPtr->m_tilingFactor = tilingFactor;
			m_pQuadVertexBufferPtr->m_gameObjectGUID = m_gameObjectGUID;
			++m_pQuadVertexBufferPtr;
		}

		m_quadIndexCount += 6;

		++m_stats.m_quadCount;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const ResourceID& texture, float tilingFactor, Color tintColor, int m_gameObjectGUID)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (m_quadIndexCount >= s_kMaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < m_textureSlotIndex; ++i)
		{
			if (m_textureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (m_textureSlotIndex >= s_kMaxTextureSlots)
				NextBatch();

			textureIndex = (float)m_textureSlotIndex;
			m_textureSlots[m_textureSlotIndex] = texture;
			++m_textureSlotIndex;
		}

		glm::vec4 vertexPositions[4];
		vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		for (size_t i = 0; i < quadVertexCount; ++i)
		{
			m_pQuadVertexBufferPtr->m_position = transform * vertexPositions[i];
			m_pQuadVertexBufferPtr->m_color = tintColor.GetColorVector();
			m_pQuadVertexBufferPtr->m_textureCoord = textureCoords[i];
			m_pQuadVertexBufferPtr->m_textureIndex = textureIndex;
			m_pQuadVertexBufferPtr->m_tilingFactor = tilingFactor;
			m_pQuadVertexBufferPtr->m_gameObjectGUID = m_gameObjectGUID;
			++m_pQuadVertexBufferPtr;
		}

		m_quadIndexCount += 6;

		++m_stats.m_quadCount;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const SharedPtr<SubTexture>& texture, float tilingFactor, Color tintColor, int m_gameObjectGUID)
	{
		DrawQuad(transform, texture->GetTextureResourceID(), tilingFactor, tintColor, m_gameObjectGUID);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Color color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Color color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const ResourceID& texture, float tilingFactor, Color tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const ResourceID& texture, float tilingFactor, Color tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const SharedPtr<SubTexture>& texture, float tilingFactor, Color tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture->GetTextureResourceID(), tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const SharedPtr<SubTexture>& texture, float tilingFactor, Color tintColor)
	{
		DrawRotatedQuad(position, size, rotation, texture->GetTextureResourceID(), tilingFactor, tintColor);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, Color color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int m_gameObjectGUID /*= -1*/)
	{
		// TODO: implement for circles
		// if (QuadIndexCount >= Renderer2DData::MaxIndices)
		// 	NextBatch();


		glm::vec4 vertexPositions[4];
		vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		for (size_t i = 0; i < 4; i++)
		{
			m_pCircleVertexBufferPtr->m_worldPosition = transform * vertexPositions[i];
			m_pCircleVertexBufferPtr->m_localPosition = vertexPositions[i] * 2.0f;
			m_pCircleVertexBufferPtr->m_color = color.GetColorVector();
			m_pCircleVertexBufferPtr->m_thickness = thickness;
			m_pCircleVertexBufferPtr->m_fade = fade;
			m_pCircleVertexBufferPtr->m_gameObjectGUID = m_gameObjectGUID;
			m_pCircleVertexBufferPtr++;
		}

		m_circleIndexCount += 6;

		m_stats.m_quadCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, glm::vec3& p1, Color color, int m_gameObjectGUID)
	{
		m_pLineVertexBufferPtr->m_position = p0;
		m_pLineVertexBufferPtr->m_color = color.GetColorVector();
		m_pLineVertexBufferPtr->m_gameObjectGUID = m_gameObjectGUID;
		m_pLineVertexBufferPtr++;

		m_pLineVertexBufferPtr->m_position = p1;
		m_pLineVertexBufferPtr->m_color = color.GetColorVector();
		m_pLineVertexBufferPtr->m_gameObjectGUID = m_gameObjectGUID;
		m_pLineVertexBufferPtr++;

		m_lineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, Color color, int m_gameObjectGUID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, m_gameObjectGUID);
		DrawLine(p1, p2, color, m_gameObjectGUID);
		DrawLine(p2, p3, color, m_gameObjectGUID);
		DrawLine(p3, p0, color, m_gameObjectGUID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, Color color, int m_gameObjectGUID)
	{
		glm::vec4 vertexPositions[4];
		vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * vertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color, m_gameObjectGUID);
		DrawLine(lineVertices[1], lineVertices[2], color, m_gameObjectGUID);
		DrawLine(lineVertices[2], lineVertices[3], color, m_gameObjectGUID);
		DrawLine(lineVertices[3], lineVertices[0], color, m_gameObjectGUID);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int m_gameObjectGUID)
	{
		if (src.m_textureResource.IsReferenceHeld())
			DrawQuad(transform, src.m_textureResource.GetID(), src.m_textureTilingMultiplier, src.m_color, m_gameObjectGUID);
		else
			DrawQuad(transform, src.m_color, m_gameObjectGUID);
	}

	void Renderer2D::DrawRawVertexRect(const eastl::array<glm::vec4, 4>& vertices, Color color)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = vertices[i];

		DrawLine(lineVertices[0], lineVertices[1], color, -1);
		DrawLine(lineVertices[1], lineVertices[2], color, -1);
		DrawLine(lineVertices[2], lineVertices[3], color, -1);
		DrawLine(lineVertices[3], lineVertices[0], color, -1);
	}

	float Renderer2D::GetLineWidth()
	{
		return m_lineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		m_lineWidth = width;
	}

	const SharedPtr<IndexBuffer> Renderer2D::MakeIndexBuffer()
	{
		// We need to heap allocate here because its size is so large.
		uint32_t* pQuadIndices = EXELIUS_NEW_ARRAY(uint32_t, s_kMaxIndices);
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_kMaxIndices; i += 6)
		{
			pQuadIndices[i + 0] = offset + 0;
			pQuadIndices[i + 1] = offset + 1;
			pQuadIndices[i + 2] = offset + 2;

			pQuadIndices[i + 3] = offset + 2;
			pQuadIndices[i + 4] = offset + 3;
			pQuadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		SharedPtr<IndexBuffer> pIndexBuffer = MakeShared<IndexBuffer>(pQuadIndices, s_kMaxIndices);
		EXELIUS_DELETE_ARRAY(pQuadIndices);
		return pIndexBuffer;
	}

	void Renderer2D::InitializeQuadRendering(const SharedPtr<IndexBuffer>& pIndexBuffer)
	{
		m_pQuadVertexArray = MakeShared<VertexArray>();

		m_pQuadVertexBuffer = MakeShared<VertexBuffer>(s_kMaxVertices * (uint32_t)sizeof(QuadVertex));
		m_pQuadVertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3,	"a_position" },
				{ ShaderDataType::Float4,	"a_color" },
				{ ShaderDataType::Float2,	"a_textureCoordinate" },
				{ ShaderDataType::Float,	"a_textureIndex" },
				{ ShaderDataType::Float,	"a_tilingMultiplier" },
				{ ShaderDataType::Int,		"a_gameObjectGUID" }
			});
		m_pQuadVertexArray->AddVertexBuffer(m_pQuadVertexBuffer);

		m_pQuadVertexBufferBase = EXELIUS_NEW_ARRAY(QuadVertex, s_kMaxVertices);

		m_pQuadVertexArray->SetIndexBuffer(pIndexBuffer);
	}

	void Renderer2D::InitializeCircleRendering(const SharedPtr<IndexBuffer>& pIndexBuffer)
	{
		m_pCircleVertexArray = MakeShared<VertexArray>();

		m_pCircleVertexBuffer = MakeShared<VertexBuffer>(s_kMaxVertices * (uint32_t)sizeof(CircleVertex));
		m_pCircleVertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3,	"a_worldPosition" },
				{ ShaderDataType::Float3,	"a_localPosition" },
				{ ShaderDataType::Float4,	"a_color" },
				{ ShaderDataType::Float,	"a_thickness" },
				{ ShaderDataType::Float,	"a_fade" },
				{ ShaderDataType::Int,		"a_gameObjectGUID" }
			});
		m_pCircleVertexArray->AddVertexBuffer(m_pCircleVertexBuffer);
		m_pCircleVertexArray->SetIndexBuffer(pIndexBuffer);

		m_pCircleVertexBufferBase = EXELIUS_NEW_ARRAY(CircleVertex, s_kMaxVertices);
	}

	void Renderer2D::InitializeLineRendering()
	{
		m_pLineVertexArray = MakeShared<VertexArray>();

		m_pLineVertexBuffer = MakeShared<VertexBuffer>(s_kMaxVertices * (uint32_t)sizeof(LineVertex));
		m_pLineVertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3,	"a_position" },
				{ ShaderDataType::Float4,	"a_color" },
				{ ShaderDataType::Int,		"a_gameObjectGUID" }
			});
		m_pLineVertexArray->AddVertexBuffer(m_pLineVertexBuffer);

		m_pLineVertexBufferBase = EXELIUS_NEW_ARRAY(LineVertex, s_kMaxVertices);
	}

	void Renderer2D::InitializeWhiteTexture()
	{
		m_whiteTextureResource.CreateNew("WhiteTexture.png"); // Forcefully create a new texture resource.

		TextureResource* pTextureResource = m_whiteTextureResource.GetAs<TextureResource>();
		if (pTextureResource)
		{
			pTextureResource->SetTexture(EXELIUS_NEW(Texture(1, 1)));

			uint32_t whiteTextureData = 0xffffffff;
			Texture* pTexture = pTextureResource->GetTexture();
			if (pTexture)
				pTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		}

		// Set first texture slot to 0
		m_textureSlots[0] = m_whiteTextureResource.GetID();
	}

	void Renderer2D::InitializeShaders()
	{
		m_quadShaderResource.LoadNow();
		if (!m_quadShaderResource.IsReferenceHeld())
			m_quadShaderResource.Release();

		m_circleShaderResource.LoadNow();
		if (!m_circleShaderResource.IsReferenceHeld())
			m_circleShaderResource.Release();

		m_lineShaderResource.LoadNow();
		if (!m_lineShaderResource.IsReferenceHeld())
			m_lineShaderResource.Release();
	}

	void Renderer2D::StartBatch()
	{
		m_quadIndexCount = 0;
		m_pQuadVertexBufferPtr = m_pQuadVertexBufferBase;

		m_circleIndexCount = 0;
		m_pCircleVertexBufferPtr = m_pCircleVertexBufferBase;

		m_lineVertexCount = 0;
		m_pLineVertexBufferPtr = m_pLineVertexBufferBase;

		m_textureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::FlushQuads()
	{
		if (m_quadIndexCount <= 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)m_pQuadVertexBufferPtr - (uint8_t*)m_pQuadVertexBufferBase);
		m_pQuadVertexBuffer->SetData(m_pQuadVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < m_textureSlotIndex; i++)
		{
			ResourceHandle textureHandle(m_textureSlots[i]);
			TextureResource* pTextureResource = textureHandle.GetAs<TextureResource>();
			if (!pTextureResource)
				return;

			Texture* pTexture = pTextureResource->GetTexture();

			if (pTexture)
				pTexture->Bind(i);
		}

		BindShader(m_quadShaderResource);

		m_pRendererAPI->DrawIndexed(m_pQuadVertexArray, m_quadIndexCount);
		m_stats.m_drawCalls++;
	}

	void Renderer2D::FlushCircles()
	{
		if (m_circleIndexCount <= 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)m_pCircleVertexBufferPtr - (uint8_t*)m_pCircleVertexBufferBase);
		m_pCircleVertexBuffer->SetData(m_pCircleVertexBufferBase, dataSize);

		BindShader(m_circleShaderResource);

		m_pRendererAPI->DrawIndexed(m_pCircleVertexArray, m_circleIndexCount);
		m_stats.m_drawCalls++;
	}

	void Renderer2D::FlushLines()
	{
		if (m_lineVertexCount <= 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)m_pLineVertexBufferPtr - (uint8_t*)m_pLineVertexBufferBase);
		m_pLineVertexBuffer->SetData(m_pLineVertexBufferBase, dataSize);

		BindShader(m_lineShaderResource);

		m_pRendererAPI->SetLineWidth(m_lineWidth);
		m_pRendererAPI->DrawLines(m_pLineVertexArray, m_lineVertexCount);
		m_stats.m_drawCalls++;
	}

	void Renderer2D::BindShader(ResourceHandle& shaderResource)
	{
		if (!shaderResource.IsReferenceHeld())
			return;

		shaderResource.GetAs<ShaderResource>();

		ShaderResource* pShaderResource = shaderResource.GetAs<ShaderResource>();;
		if (pShaderResource)
		{
			Shader& shader = pShaderResource->GetShader();
			shader.Bind();
		}
	}
}
