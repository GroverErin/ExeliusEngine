#pragma once
#include "Source/Utility/Generic/Singleton.h"
#include "Source/Resource/ResourceHelpers.h"
#include "Source/Resource/ResourceHandle.h"
#include "Source/Utility/Math/Rectangle.h"

#include <EASTL/vector.h>

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
		///			^^^^^^^
		///			|||||||
		///			||||||
		///			|||||- bits for shader resource.
		///			||||- bits for the Texture Frame (rect on source texture).
		///			|||- bits for the Z-Order or World Coordinates.
		///			||- bits for the Texture.
		///			|-3 bits for the Renderlayer.
		/// </summary>
		uint32_t m_renderSortKey;

		void SetRenderLayer(RenderLayer layer) {}
		void SetTexture(const ResourceID& textureID) {}
		void SetWorldPosition(const Vector2f& postion) {}
		void SetTextureFrame(FRectangle rectangle) {}
		void SetShader(const ResourceID& shaderID) {}
	
		void SetWorldScale(float scaleFactor) {}

	private:
		RenderLayer m_renderLayer;
		ResourceHandle m_shader;
		ResourceHandle m_texture;
		FRectangle m_spriteFrame;
		Vector2f m_position;
		float m_scaleFactor;
	};

	class RenderManager
		: public Singleton<RenderManager>
	{
		eastl::vector<RenderCommand> m_advancedBuffer; // Main loop adds to this buffer.
		eastl::vector<RenderCommand> m_backBuffer; // Main loop will swap this buffer with advancedbuffer at the end of a frame.
		
		std::thread m_renderThread;
		std::mutex m_backBufferLock;
		std::atomic_bool m_quitThread;
		std::condition_variable m_signalThread;

		static constexpr int s_kBitCount = eastl::numeric_limits<decltype(RenderCommand::m_renderSortKey)>::digits;
	public:
		RenderManager();
		RenderManager(const RenderManager&) = delete;
		RenderManager(RenderManager&&) = delete;
		RenderManager& operator=(const RenderManager&) = delete;
		RenderManager& operator=(RenderManager&&) = delete;
		~RenderManager();

		// Spins up the thread and sets the screenshot output path (unused).
		bool Initialize(const char* pScreenshotOutputPath = nullptr);

		// My thoughts are that the RenderManager should own everything related to rendering.
		// Including Views(Camera) and Windows(rendertarget). 
		#undef CreateWindow // Defined in WinUser.h :(
		void CreateWindow();
		void GetWindow();
		void CreateView();
		void GetView();

		// Adds a command to the advanceBuffer (1 frame ahead of renderthread)
		void PushRenderCommand(RenderCommand renderCommand);

	private:
		// This is essentially the thread::do_work() function.
		void RenderThread();

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