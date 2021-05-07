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
		eastl::vector<RenderCommand> m_advancedBuffer;
		eastl::vector<RenderCommand> m_backBuffer;
		
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

		bool Initialize(const char* pScreenshotOutputPath = nullptr);

		#undef CreateWindow // Defined in WinUser.h :(
		void CreateWindow();
		void GetWindow();

		void CreateCamera();
		void GetCamera();

		void PushRenderCommand(RenderCommand renderCommand);

	private:
		void RenderThread();

		void SwapRenderCommandBuffer(eastl::vector<RenderCommand>& bufferToSwap);
		
		void SortRenderQueue(eastl::vector<RenderCommand>& bufferToSort);

		void SortRenderQueueByBit(eastl::vector<RenderCommand>& bufferToSort, int bit);

		static int GetBitValue(uint64_t sortKey, int bit);
		
		void SignalAndWaitForRenderThread();
	};
}