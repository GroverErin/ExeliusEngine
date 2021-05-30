#pragma once
#include "Source/Utility/Generic/Singleton.h"
#include "Source/Utility/Math/Rectangle.h"
#include "Source/Resource/ResourceHelpers.h"
#include "Source/Resource/ResourceHandle.h"
#include "Source/OS/Platform/PlatformForwardDeclarations.h"

#include "Source/OS/Interface/Graphics/View.h"
#include "Source/OS/Interface/Graphics/Vertex.h"

#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>

#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>

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
		ResourceID m_shader;
		ResourceID m_texture;
		IRectangle m_spriteFrame;
		Vector2f m_position;
		Vector2f m_scaleFactor;
		int m_zOrder;

		bool operator<(const RenderCommand& command) const;
	};

	FORWARD_DECLARE(Window);
	FORWARD_DECLARE(VertexArray);

	class RenderManager
		: public Singleton<RenderManager>
	{
		static constexpr int s_kMaxFramesBehind = 2;
		Window* m_pWindow;
		
		eastl::vector<RenderCommand> m_advancedBuffer; // Main loop adds to this buffer.
		
		eastl::vector<RenderCommand> m_intermediateBuffer; // Main loop will swap this buffer with advancedbuffer at the end of a frame. Render Thread will swap with this buffer if it is not processing.
		std::mutex m_intermediateBufferMutex;

		std::thread m_renderThread;
		std::atomic_bool m_quitThread;
		std::atomic_int m_framesBehind;

		std::mutex m_signalMutex;
		std::condition_variable m_signalThread;

		eastl::vector<eastl::pair<StringIntern, View>> m_views;
		std::mutex m_viewListLock;
	public:
		RenderManager();
		RenderManager(const RenderManager&) = delete;
		RenderManager(RenderManager&&) = delete;
		RenderManager& operator=(const RenderManager&) = delete;
		RenderManager& operator=(RenderManager&&) = delete;
		~RenderManager();

		// Spins up the thread.
		bool Initialize(const eastl::string& title, const Vector2u& windowSize);

		// Adds a command to the advanceBuffer (1 frame ahead of renderthread)
		void PushRenderCommand(RenderCommand renderCommand);

		void Update();

		void EndRenderFrame();

		void AddView(const StringIntern& viewID, const View& view);

		Window* GetWindow();

	private:
		// This is the thread::do_work() function.
		void RenderThread();

		void DrawToWindow(const eastl::vector<RenderCommand>& backBuffer);

		void DrawToViews(const eastl::vector<RenderCommand>& backBuffer);

		// Swap the input buffer with the temp buffer.
		void SwapRenderCommandBuffer(eastl::vector<RenderCommand>& bufferToSwap);
		
		// Sort RenderCommands
		void SortRenderCommands(eastl::vector<RenderCommand>& bufferToSort);

		bool IsInViewBounds(const RenderCommand& command, const IRectangle& viewBounds) const;

		void AddVertexToArray(VertexArray& vertexArray, const RenderCommand& command) const;

		// Used when this Manager is destroyed in order to stop the thread.
		void SignalAndWaitForRenderThread();
	};
}