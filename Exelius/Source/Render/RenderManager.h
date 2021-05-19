#pragma once
#include "Source/Utility/Generic/Singleton.h"
#include "Source/Utility/Math/Rectangle.h"
#include "Source/Resource/ResourceHelpers.h"
#include "Source/Resource/ResourceHandle.h"
#include "Source/OS/Platform/PlatformForwardDeclarations.h"

#include "Source/OS/Interface/Graphics/View.h"

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

		/// <summary>
		/// MSB [ 00000000 00000000 00000000 00000000 ] LSB
		///			^				  ^^^
		///			|				  |||
		///			|				  |||
		///			|				  |||
		///			|				  |||-16 bits for shader resource.
		///			|				  ||-16 bits for the Z-Order or World Coordinates.
		///			|				  |-16 bits for the Texture.
		///			|-3 bits for the Renderlayer.
		/// </summary>
		uint32_t m_renderSortKey;

		void SetRenderLayer(RenderLayer layer) { m_renderLayer = layer; }
		void SetTexture(const ResourceID& textureID) { m_texture = textureID; }
		void SetWorldPosition(const Vector2f& postion) { m_position = postion; }
		void SetTextureFrame(IRectangle rectangle) { m_spriteFrame = rectangle; }
		void SetShader(const ResourceID& shaderID) { m_shader = shaderID; }
	
		void SetWorldScale(const Vector2f& scaleFactor) { m_scaleFactor = scaleFactor; }

		RenderLayer m_renderLayer;
		ResourceID m_shader;
		ResourceID m_texture;
		IRectangle m_spriteFrame;
		Vector2f m_position;
		Vector2f m_scaleFactor;
	};

	FORWARD_DECLARE(Window);

	class RenderManager
		: public Singleton<RenderManager>
	{
		eastl::vector<RenderCommand> m_advancedBuffer; // Main loop adds to this buffer.
		eastl::vector<RenderCommand> m_intermediateBuffer; // Main loop will swap this buffer with advancedbuffer at the end of a frame. Render Thread will swap with this buffer if it is not processing.
		
		std::thread m_renderThread;
		std::mutex m_intermediateBufferLock;
		std::atomic_bool m_quitThread;
		std::atomic_int m_framesBehind;
		static constexpr int s_kMaxFramesBehind = 2;
		std::condition_variable m_signalThread;

		Window* m_pWindow;

		eastl::vector<eastl::pair<StringIntern, View>> m_views;
		std::mutex m_viewListLock;

		static constexpr int s_kBitCount = eastl::numeric_limits<decltype(RenderCommand::m_renderSortKey)>::digits;
	public:
		RenderManager();
		RenderManager(const RenderManager&) = delete;
		RenderManager(RenderManager&&) = delete;
		RenderManager& operator=(const RenderManager&) = delete;
		RenderManager& operator=(RenderManager&&) = delete;
		~RenderManager();

		// Spins up the thread.
		bool Initialize(const eastl::string& title, const Vector2u& windowSize);

		Window* GetWindow();

		void AddView(const StringIntern& viewID, const View& view);

		// Adds a command to the advanceBuffer (1 frame ahead of renderthread)
		void PushRenderCommand(RenderCommand renderCommand);

		void Update();

		void EndRenderFrame();

	private:
		// This is the thread::do_work() function.
		void RenderThread();

		void DrawToWindow(const eastl::vector<RenderCommand>& backBuffer);

		void DrawToViews(const eastl::vector<RenderCommand>& backBuffer);

		// Swap the input buffer with the temp buffer.
		void SwapRenderCommandBuffer(eastl::vector<RenderCommand>& bufferToSwap);
		
		// Sort the rendercommands by key. This uses a Radix Sort. See comments in .cpp
		void SortRenderQueue(eastl::vector<RenderCommand>& bufferToSort);

		// See: SortRenderQueue. This is the "internal" sort.
		void SortRenderQueueByBit(eastl::vector<RenderCommand>& bufferToSort, int bit);

		// Helper function for sort algorithm.
		static int GetBitValue(uint64_t sortKey, int bit);
		
		// Used when this Manager is destroyed in order to stop the thread.
		void SignalAndWaitForRenderThread();
	};
}