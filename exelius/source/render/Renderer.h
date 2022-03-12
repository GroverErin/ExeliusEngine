#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/Singleton.h"
#include "source/utility/generic/Color.h"
#include "source/render/camera/OrthographicCamera.h"

#include <glm/glm.hpp>
#include <EASTL/unique_ptr.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	FORWARD_DECLARE(RendererAPI);
	FORWARD_DECLARE(Shader);
	FORWARD_DECLARE(VertexArray);

	class Renderer
		: public Singleton<Renderer>
	{
		struct SceneData
		{
			glm::mat4 m_viewProjectionMatrix;
		};

		SceneData m_sceneData;

		eastl::unique_ptr<SceneData> s_pSceneData;
		RendererAPI* m_pRendererAPI;

	public:
		Renderer();
		~Renderer();

		void OnWindowResize(uint32_t width, uint32_t height);

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void SetClearColor(Color color);

		void BeginScene(OrthographicCamera& camera);
		void EndScene();

		void Submit(const eastl::unique_ptr<Shader>& shader, const eastl::unique_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		void DrawIndexed(const eastl::unique_ptr<VertexArray>& vertexArray, uint32_t count = 0);

		void Clear();
	private:

		void Initialize();

		void Shutdown();
	};
}