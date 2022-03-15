#pragma once
#include "source/os/platform/PlatformForwardDeclarations.h"
#include "source/utility/generic/Singleton.h"
#include "source/utility/generic/Color.h"
#include "source/render/camera/OrthographicCamera.h"
#include "source/utility/generic/SmartPointers.h"
#include "source/render/Window.h"

#include <glm/glm.hpp>

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
		Window m_window;

		UniquePtr<SceneData> s_pSceneData;
		RendererAPI* m_pRendererAPI;

	public:
		Renderer(eastl::string& windowTitle, const Vector2u& windowSize, bool isVSyncEnabled = true);
		~Renderer();

		void Update();

		void OnWindowResize(uint32_t width, uint32_t height);

		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void SetClearColor(Color color);

		void BeginScene(OrthographicCamera& camera);
		void EndScene();

		void Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t count = 0);

		void Clear();

		Window& GetWindow() { return m_window; }
	private:

		void Initialize();

		void Shutdown();
	};
}