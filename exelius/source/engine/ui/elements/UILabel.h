#pragma once
#include "source/engine/ui/UIElement.h"

#include "source/resource/ResourceHelpers.h"

#include <EASTL/string.h>

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
	struct Word
	{
		eastl::string m_word;
		float m_wordWidth = 0.0f;
		float m_wordSpacing = 0.0f;

		Word(const eastl::string& word, float width)
			: m_word(word)
			, m_wordWidth(width)
		{
			//
		}
	};

	struct Line
	{
		float m_lineWidth = 0.0f;
		eastl::vector<Word> m_words;
	};

	class UILabel
		: public UIElement
	{
	public:
		enum class HorizontalAlignment
		{
			Left = 0,
			Right = 1,
			Centered = 2,
			Justified = 3,
			Error
		};

		enum class VerticalAlignment
		{
			Top = 0,
			Centered = 2,
			Bottom = 3,
			Error
		};
	private:
		ResourceID m_fontID;
		eastl::string m_text;
		float m_textWidth;
		float m_textHeight;
		HorizontalAlignment m_horizontalAlignment;
		VerticalAlignment m_verticalAlignment;

		eastl::vector<Line> m_lineList;
	public:

		UILabel(UIElement* pParent = nullptr)
			: UIElement(pParent)
			, m_textWidth(0.0f)
			, m_textHeight(0.0f)
			, m_horizontalAlignment(HorizontalAlignment::Left)
			, m_verticalAlignment(VerticalAlignment::Top)
		{
			//
		}

		virtual bool Initialize(const rapidjson::Value& jsonUIElementData) final override;

		virtual void OnRender() final override;
		virtual void OnDestroy() final override;

		float GetTextWidth() const { return m_textWidth; }
		float GetTextHeight() const { return m_textHeight; }

		void SetTextWidth(float newWidth) { m_textWidth = newWidth; }
		void SetTextHeight(float newHeight) { m_textHeight = newHeight; }

	private:
		bool DidHandleSpecialCharacter(char character, float& xToSet) const;

		eastl::vector<Word> ParseTextIntoWords();
		void BuildLines();

		float CalculateWordWidth(const eastl::string& word) const;

		float HandleHorizontalAlignment(Line& line) const;
		void HandleJustifiedAlignment(Line& line) const;

		float HandleVerticalAlignment() const;
	};
}