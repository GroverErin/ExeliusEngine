#include "EXEPCH.h"
#include "Source/Render/RenderManager.h"
#include "Source/OS/Interface/Graphics/Window.h"

// TEMP
#include "Source/OS/Interface/Graphics/Sprite.h"
#include "Source/Engine/Resources/ResourceTypes/TextureResource.h"

#include <EASTL/numeric_limits.h>
#include <EASTL/sort.h>

namespace Exelius
{
	bool RenderCommand::operator<(const RenderCommand& command) const
	{
		// Sort By Z
		if (m_zOrder != command.m_zOrder)
			return (m_zOrder < command.m_zOrder);

		float leftLowestY = m_position.y + (m_spriteFrame.m_width * m_scaleFactor.y);
		float rightLowestY = command.m_position.y + (command.m_spriteFrame.m_width * command.m_scaleFactor.y);

		// Sort By Bottom of Sprite y + h
		if (leftLowestY != rightLowestY)
			return (leftLowestY < rightLowestY);

		// sort by x
		return (m_position.x < command.m_position.x);
	}

	RenderManager::RenderManager()
		: m_quitThread(false)
		, m_framesBehind(0)
		, m_pWindow(nullptr)
	{
		//
	}

	RenderManager::~RenderManager()
	{
		m_advancedBuffer.clear();

		m_intermediateBufferMutex.lock();
		m_intermediateBuffer.clear();
		m_intermediateBufferMutex.unlock();

		// Tell the thread we are done.
		m_quitThread = true;
		SignalAndWaitForRenderThread();
		m_renderThread.join();

		m_pWindow->SetActive(true);

		delete m_pWindow;
		m_pWindow = nullptr;
	}

	bool RenderManager::Initialize(const eastl::string& title, const Vector2u& windowSize)
	{
		m_advancedBuffer.clear();

		m_intermediateBufferMutex.lock();
		m_intermediateBuffer.clear();
		m_intermediateBufferMutex.unlock();

		EXE_ASSERT(!m_pWindow);
		m_pWindow = new Window(title, windowSize);
		EXE_ASSERT(m_pWindow);
		m_pWindow->SetVSync(false);
		m_pWindow->SetActive(false);

		m_renderThread = std::thread(&RenderManager::RenderThread, this);

		return true;
	}

	void RenderManager::PushRenderCommand(RenderCommand renderCommand)
	{
		m_advancedBuffer.emplace_back(renderCommand);
	}

	void RenderManager::Update()
	{
		EXE_ASSERT(m_pWindow);
		m_pWindow->Update();
	}

	void RenderManager::EndRenderFrame()
	{
		if (!m_advancedBuffer.empty())
		{
			SwapRenderCommandBuffer(m_advancedBuffer);
			++m_framesBehind;
			m_advancedBuffer.clear();
		}

		if (m_framesBehind > s_kMaxFramesBehind)
		{
			// wait for render thread.
			SignalAndWaitForRenderThread();
		}
		else
		{
			EXELOG_ENGINE_INFO("Signaling Render Thread.");
			m_signalThread.notify_one();
		}

		EXE_ASSERT(m_framesBehind <= s_kMaxFramesBehind);
		EXE_ASSERT(m_renderThread.joinable());
	}

	Window* RenderManager::GetWindow()
	{
		EXE_ASSERT(m_pWindow);
		return m_pWindow;
	}

	void RenderManager::AddView(const StringIntern& viewID, const View& view)
	{
		// TODO:
		// Check if this view already exists?
		// Maybe not necessary, as it should replace any existing view.
		m_viewListLock.lock();
		m_views.emplace_back(viewID, view);
		m_viewListLock.unlock();
	}

	void RenderManager::RenderThread()
	{
		EXELOG_ENGINE_INFO("Instantiating Render Thread.");
		eastl::vector<RenderCommand> backBuffer;

		EXE_ASSERT(m_pWindow);
		if (!m_pWindow->SetActive(true))
		{
			EXELOG_ENGINE_FATAL("Failed to activate Window on Render Thread. TODO: Bail Here");
		}

		// While the thread is still active (Not shut down)
		while (!m_quitThread)
		{
			// Wait until we are signaled to work.
			std::unique_lock<std::mutex> waitLock(m_signalMutex);
			m_signalThread.wait(waitLock, [this]()
				{
					bool empty = true;

					m_intermediateBufferMutex.lock();
					empty = m_intermediateBuffer.empty();
					m_intermediateBufferMutex.unlock();

					return m_quitThread || !empty;
				});

			EXELOG_ENGINE_ERROR("Render Thread Recieved Signal");

			// Don't do any work if we're exiting.
			if (m_quitThread)
				break;

			/// Capture the deferred queue into the processing queue (swap buffers)
			SwapRenderCommandBuffer(backBuffer);
			m_framesBehind = 0;

			SortRenderCommands(backBuffer);

			// Render Clear
			m_pWindow->Clear();

			// Render Draw
			m_viewListLock.lock();
			if (m_views.empty())
				DrawToWindow(backBuffer);
			else
				DrawToViews(backBuffer);
			m_viewListLock.unlock();

			backBuffer.clear();

			// Render Display
			m_pWindow->Render();

			waitLock.unlock();
			m_signalThread.notify_one();
		}

		m_pWindow->SetActive(false);

		// Let the main thread know we are fully exiting in case they are waiting.
		EXELOG_ENGINE_INFO("Signaled Main Thread: Render Thread Terminating.");
		m_signalThread.notify_one();
	}

	void RenderManager::DrawToWindow(const eastl::vector<RenderCommand>& backBuffer)
	{
		IRectangle windowRect({ 0,0 }, static_cast<Vector2i>(m_pWindow->GetWindowSize()));

		// For each rendercommand...
		for (auto& command : backBuffer)
		{
			if (!IsInViewBounds(command, windowRect))
				continue;

			// If rendercommand can be batched
				// Batch it
				// continue	
			// else
				// Render current vertex buffer
				// Clear Vertex Buffer
				// Add this command to the buffer
				// continue

			ResourceHandle texture(command.m_texture);

			auto* pTextureResource = texture.GetAs<TextureResource>();

			if (!pTextureResource)
				continue;

			Sprite newSprite(*pTextureResource->GetTexture(), command.m_spriteFrame);
			newSprite.SetPosition(command.m_position);
			newSprite.SetScale(command.m_scaleFactor);
			newSprite.GetNativeSprite().Render();
		}
	}

	void RenderManager::DrawToViews(const eastl::vector<RenderCommand>& backBuffer)
	{
		for (auto& view : m_views)
		{
			m_pWindow->SetView(view.second);

			// Build the view rectangle.
			IRectangle viewRect;
			viewRect.m_top = static_cast<int>(view.second.GetCenter().y - (view.second.GetSize().h / 2.0f));
			viewRect.m_left = static_cast<int>(view.second.GetCenter().x - (view.second.GetSize().w / 2.0f));
			viewRect.m_width = view.second.GetSize().w;
			viewRect.m_height = view.second.GetSize().h;

			// For each rendercommand...
			for (auto& command : backBuffer)
			{
				if (!IsInViewBounds(command, viewRect))
					continue;

				// If rendercommand can be batched
					// Batch it
					// continue	
				// else
					// Render current vertex buffer
					// Clear Vertex Buffer
					// Add this command to the buffer
					// continue

				ResourceHandle texture(command.m_texture);

				auto* pTextureResource = texture.GetAs<TextureResource>();

				if (!pTextureResource)
					continue;

				Sprite newSprite(*pTextureResource->GetTexture(), command.m_spriteFrame);
				newSprite.SetPosition(command.m_position);
				newSprite.SetScale(command.m_scaleFactor);
				newSprite.GetNativeSprite().Render();
			}
		}
	}

	void RenderManager::SwapRenderCommandBuffer(eastl::vector<RenderCommand>& bufferToSwap)
	{
		m_intermediateBufferMutex.lock();
		bufferToSwap.swap(m_intermediateBuffer);
		m_intermediateBufferMutex.unlock();
	}

	void RenderManager::SortRenderCommands(eastl::vector<RenderCommand>& bufferToSort)
	{
		// TODO:
			// Maybe eastl::stable_sort is what I want here?
		// Sort based on the Z, Y, or X in that order.
		eastl::sort(bufferToSort.begin(), bufferToSort.end());

		// Without disturbing above sort, sort by texture.
		eastl::stable_sort(bufferToSort.begin(), bufferToSort.end(), [](const RenderCommand& left, const RenderCommand& right)
			{
				// Sort By Texture
				if (left.m_texture != right.m_texture)
					return (left.m_texture < right.m_texture);

				// Sort By Shader
				return (left.m_shader < right.m_shader); // TODO: This may be a bug if m_shader is not set?
			});
	}

	bool RenderManager::IsInViewBounds(const RenderCommand& command, const IRectangle& viewBounds) const
	{
		// TODO:
			//	Crappy Conversion.
		Vector2f tempVect;
		tempVect.x = command.m_spriteFrame.GetSize().x * command.m_scaleFactor.x;
		tempVect.y = command.m_spriteFrame.GetSize().y * command.m_scaleFactor.y;

		IRectangle rectToDraw(static_cast<Vector2i>(command.m_position), static_cast<Vector2i>(tempVect));
		if (!viewBounds.Intersects(rectToDraw))
			return false;

		return true;
	}

	void RenderManager::SignalAndWaitForRenderThread()
	{
		EXE_ASSERT(m_renderThread.joinable());
		EXELOG_ENGINE_INFO("Signaling Render Thread.");
		m_signalThread.notify_one();

		std::unique_lock<std::mutex> waitLock(m_signalMutex);
		m_signalThread.wait(waitLock, [this]()
			{
				return (m_framesBehind == 0);
			});

		waitLock.unlock();
	}
}