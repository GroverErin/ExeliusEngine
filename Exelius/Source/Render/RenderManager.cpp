#include "EXEPCH.h"
#include "Source/Render/RenderManager.h"

#include <EASTL/numeric_limits.h>

namespace Exelius
{
	RenderManager::RenderManager()
		: m_quitThread(false)
	{
		//
	}

	RenderManager::~RenderManager()
	{
		m_advancedBuffer.clear();

		m_backBufferLock.lock();
		m_backBuffer.clear();
		m_backBufferLock.unlock();

		// Tell the thread we are done.
		m_quitThread = true;
		SignalAndWaitForRenderThread();
		m_renderThread.join();
	}

	bool RenderManager::Initialize([[maybe_unused]] const char* pScreenshotOutputPath)
	{
		m_advancedBuffer.clear();

		m_backBufferLock.lock();
		m_backBuffer.clear();
		m_backBufferLock.unlock();

		m_renderThread = std::thread(&RenderManager::RenderThread, this);

		return true;
	}

	void RenderManager::CreateWindow()
	{
	}

	void RenderManager::GetWindow()
	{
	}

	void RenderManager::PushRenderCommand(RenderCommand renderCommand)
	{
		m_advancedBuffer.emplace_back(renderCommand);
	}

	void RenderManager::CreateCamera()
	{
		//
	}

	void RenderManager::GetCamera()
	{
		//
	}

	void RenderManager::RenderThread()
	{
		EXELOG_ENGINE_INFO("Instantiating Resource Render Thread.");
		std::mutex waitMutex;
		eastl::vector<RenderCommand> activeBuffer;
		std::unique_lock<std::mutex> waitLock(waitMutex);

		// While the thread is still active (Not shut down)
		while (!m_quitThread)
		{
			// Wait until we are signaled to work.
			m_signalThread.wait(waitLock, [this]()
				{
					bool empty = true;

					m_backBufferLock.lock();
					empty = m_backBuffer.empty();
					m_backBufferLock.unlock();

					return m_quitThread || !empty;
				});

			// Don't do any work if we're exiting.
			if (m_quitThread)
				continue;

			/// Capture the deferred queue into the processing queue (swap buffers)
			SwapRenderCommandBuffer(activeBuffer);

			SortRenderQueue(activeBuffer);

			// Process the queue
			for (auto& renderCommand : activeBuffer)
			{
				// for (auto& view : m_views)
					// Set active view.
					// Render if in view.
			}
		}

		// Let the main thread know we are fully exiting in case they are waiting.
		EXELOG_ENGINE_INFO("Signaled Main Thread: Thread Terminating.");
		m_signalThread.notify_one();
	}

	void RenderManager::SwapRenderCommandBuffer(eastl::vector<RenderCommand>& bufferToSwap)
	{
		m_backBufferLock.lock();
		bufferToSwap.swap(m_backBuffer);
		m_backBufferLock.unlock();
	}

	void RenderManager::SortRenderQueue(eastl::vector<RenderCommand>& bufferToSort)
	{
		// TODO: Document this Radix Sort
		//https://bitsquid.blogspot.com/2017/02/stingray-renderer-walkthrough-4-sorting.html
		//https://www.interviewcake.com/concept/java/radix-sort

		// For each bit in the RenderCommand sort key...
		for (int bit = 0; bit <= s_kBitCount; ++bit)
		{
			// Sort the RenderCommand buffer by that bit.
			SortRenderQueueByBit(bufferToSort, bit);
		}
	}

	void RenderManager::SortRenderQueueByBit(eastl::vector<RenderCommand>& bufferToSort, int bit)
	{
		// Store the total count of 1's and 0's at this bit.
		// bitCounts[0] is the number of 0's.
		// bitCount[1] is the number of 1's.
		eastl::vector<int> bitCounts(2, 0);

		// For each rendercommand in the buffer.
		for (auto& command : bufferToSort)
		{
			// Get the value at that bit and increment the count for it's respective value.
			bitCounts[GetBitValue(command.m_renderSortKey, bit)] += 1;
		}

		eastl::vector<int> sortIndices{ 0, bitCounts[0] };

		// Create a vector of the same size as the buffer to sort.
		eastl::vector<RenderCommand> sortedBuffer(bufferToSort.size());

		for (auto& command : bufferToSort)
		{
			int keyBitValue = GetBitValue(command.m_renderSortKey, bit);

			sortedBuffer[sortIndices[keyBitValue]] = command;

			sortIndices[keyBitValue] += 1;
		}

		bufferToSort.swap(sortedBuffer);
	}

	int RenderManager::GetBitValue(uint64_t sortKey, int bit)
	{
		int mask = 1 << bit;
		if ((sortKey & mask) != 0)
			return 1;
		return 0;
	}

	void RenderManager::SignalAndWaitForRenderThread()
	{
		EXE_ASSERT(m_renderThread.joinable());
		EXELOG_ENGINE_TRACE("Signaling Render Thread.");
		m_signalThread.notify_one();

		std::mutex waitMutex;
		std::unique_lock<std::mutex> waitLock(waitMutex);

		EXELOG_ENGINE_INFO("Waiting for response from Render Thread.");
		m_signalThread.wait(waitLock);
	}
}