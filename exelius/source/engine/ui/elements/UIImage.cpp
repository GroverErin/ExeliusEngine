#include "EXEPCH.h"
#include "UIImage.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	bool UIImage::Initialize(const rapidjson::Value& jsonUIElementData)
	{
		auto textureData = jsonUIElementData.FindMember("Texture");

		if (textureData == jsonUIElementData.MemberEnd())
		{
			return false;
		}
		EXE_ASSERT(textureData->value.IsString());
		m_textureID = textureData->value.GetString();
		EXE_ASSERT(m_textureID.IsValid());

		ResourceHandle texture(m_textureID);
		texture.QueueLoad(true);
		texture.LockResource();

		eastl::array<float, 4> sourceDimensions;
		auto sourceRectData = jsonUIElementData.FindMember("SourceRect");

		if (sourceRectData != jsonUIElementData.MemberEnd())
		{
			EXE_ASSERT(sourceRectData->value.IsArray());

			size_t index = 0;
			for (auto& dimension : sourceRectData->value.GetArray())
			{
				EXE_ASSERT(dimension.IsNumber());
				sourceDimensions[index] = dimension.GetFloat();
				++index;
			}
		}
		FRectangle sourceRect;
		sourceRect.x = sourceDimensions[0];
		sourceRect.y = sourceDimensions[1];
		sourceRect.w = sourceDimensions[2];
		sourceRect.h = sourceDimensions[3];
		m_sourceSize.w = sourceRect.w;
		m_sourceSize.h = sourceRect.h;

		// Create an array of the source bounds, so the default 9 slice is just the center of the 9 slices,
		// creating a "regular" image. This allows slices to not be defined and get the expected behavior.
		eastl::array<float, 4> slices = { sourceRect.x, sourceRect.y, sourceRect.w, sourceRect.h };
		auto sliceData = jsonUIElementData.FindMember("9Slices");

		if (sliceData != jsonUIElementData.MemberEnd())
		{
			EXE_ASSERT(sliceData->value.IsArray());

			size_t index = 0;
			for (auto& slice : sliceData->value.GetArray())
			{
				EXE_ASSERT(slice.IsNumber());
				slices[index] = slice.GetFloat();
				++index;
			}
		}

		m_commands[0].m_sourceFrame = FRectangle(sourceRect.x, sourceRect.y, slices[0], slices[2]); // Top-Left
		m_commands[1].m_sourceFrame = FRectangle(slices[0], sourceRect.y, slices[1] - slices[0], slices[2]); // Top
		m_commands[2].m_sourceFrame = FRectangle(slices[1], sourceRect.y, sourceRect.w - slices[1], slices[2]); // Top-Right
		m_commands[3].m_sourceFrame = FRectangle(sourceRect.x, slices[2], slices[0], slices[3] - slices[2]); // Left
		m_commands[4].m_sourceFrame = FRectangle(slices[0], slices[2], slices[1] - slices[0], slices[3] - slices[2]); // Center
		m_commands[5].m_sourceFrame = FRectangle(slices[1], slices[2], sourceRect.w - slices[1], slices[3] - slices[2]); // Right
		m_commands[6].m_sourceFrame = FRectangle(sourceRect.x, slices[3], slices[0], sourceRect.h - slices[3]); // Bottom-Left
		m_commands[7].m_sourceFrame = FRectangle(slices[0], slices[3], slices[1] - slices[0], sourceRect.h - slices[3]); // Bottom
		m_commands[8].m_sourceFrame = FRectangle(slices[1], slices[3], sourceRect.w - slices[1], sourceRect.h - slices[3]); // Bottom-Right

		for (size_t i = 0; i < m_commands.size(); ++i)
		{
			m_commands[i].m_renderLayer = RenderCommand::RenderLayer::UI;
			m_commands[i].m_texture = m_textureID;
		}

		return UIElement::Initialize(jsonUIElementData);
	}

	void UIImage::OnRender()
	{
		// TODO:
		//if (!isVisible)
		//	return;

		if (!m_textureID.IsValid())
		{
			// TODO: Logs here..
			return;
		}	

		CalculateDestinationRects();

		for (auto& command : m_commands)
		{
			command.m_tint = m_color;
			RenderManager::GetInstance()->PushRenderCommand(command);
		}
	}

	void UIImage::OnDestroy()
	{
		ResourceHandle texture(m_textureID);
		texture.UnlockResource();

		UIElement::OnDestroy();
	}

	void UIImage::CalculateDestinationRects()
	{
		Vector2f sizeRatio;
		sizeRatio.w = m_actualRegion.w / m_sourceSize.w;
		sizeRatio.h = m_actualRegion.h / m_sourceSize.h;

		float bonusWidth = (m_commands[0].m_sourceFrame.w + m_commands[2].m_sourceFrame.w) * sizeRatio.w - (m_commands[0].m_sourceFrame.w + m_commands[2].m_sourceFrame.w);
		float bonusHeight = (m_commands[0].m_sourceFrame.h + m_commands[6].m_sourceFrame.h) * sizeRatio.h - (m_commands[0].m_sourceFrame.h + m_commands[6].m_sourceFrame.h);

		FRectangle topLeftDestFrame(m_actualRegion.x, m_actualRegion.y, m_commands[0].m_sourceFrame.w, m_commands[0].m_sourceFrame.h);
		FRectangle topDestFrame(topLeftDestFrame.x + topLeftDestFrame.w, m_actualRegion.y, m_commands[1].m_sourceFrame.w * sizeRatio.w + bonusWidth, m_commands[1].m_sourceFrame.h);
		FRectangle topRightDestFrame(topDestFrame.x + topDestFrame.w, m_actualRegion.y, m_commands[2].m_sourceFrame.w, m_commands[2].m_sourceFrame.h);

		FRectangle leftDestFrame(m_actualRegion.x, topLeftDestFrame.y + topLeftDestFrame.h, m_commands[3].m_sourceFrame.w, m_commands[3].m_sourceFrame.h * sizeRatio.h + bonusHeight);
		FRectangle centerDestFrame(leftDestFrame.x + leftDestFrame.w, topDestFrame.y + topDestFrame.h, m_commands[4].m_sourceFrame.w * sizeRatio.w + bonusWidth, m_commands[4].m_sourceFrame.h * sizeRatio.h + bonusHeight);
		FRectangle rightDestFrame(centerDestFrame.x + centerDestFrame.w, topRightDestFrame.y + topRightDestFrame.h, m_commands[5].m_sourceFrame.w, m_commands[5].m_sourceFrame.h * sizeRatio.h + bonusHeight);

		FRectangle bottomLeftDestFrame(m_actualRegion.x, leftDestFrame.y + leftDestFrame.h, m_commands[6].m_sourceFrame.w, m_commands[6].m_sourceFrame.h);
		FRectangle bottomDestFrame(bottomLeftDestFrame.x + bottomLeftDestFrame.w, centerDestFrame.y + centerDestFrame.h, m_commands[7].m_sourceFrame.w * sizeRatio.w + bonusWidth, m_commands[7].m_sourceFrame.h);
		FRectangle bottomRightDestFrame(bottomDestFrame.x + bottomDestFrame.w, rightDestFrame.y + rightDestFrame.h, m_commands[8].m_sourceFrame.w, m_commands[8].m_sourceFrame.h);

		m_commands[0].m_destinationFrame = topLeftDestFrame;
		m_commands[1].m_destinationFrame = topDestFrame;
		m_commands[2].m_destinationFrame = topRightDestFrame;
		m_commands[3].m_destinationFrame = leftDestFrame;
		m_commands[4].m_destinationFrame = centerDestFrame;
		m_commands[5].m_destinationFrame = rightDestFrame;
		m_commands[6].m_destinationFrame = bottomLeftDestFrame;
		m_commands[7].m_destinationFrame = bottomDestFrame;
		m_commands[8].m_destinationFrame = bottomRightDestFrame;
	}
}
