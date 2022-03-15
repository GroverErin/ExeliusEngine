#include "EXEPCH.h"
#include "UILabel.h"
#include "source/resource/ResourceHandle.h"
#include "source/engine/resources/resourcetypes/FontResource.h"

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// </summary>
namespace Exelius
{
    bool UILabel::Initialize(const rapidjson::Value& jsonUIElementData)
    {
        auto fontData = jsonUIElementData.FindMember("Font");

        if (fontData == jsonUIElementData.MemberEnd())
        {
            return false;
        }
        EXE_ASSERT(fontData->value.IsString());
        m_fontID = fontData->value.GetString();
        EXE_ASSERT(m_fontID.IsValid());

        ResourceHandle fontSheet(m_fontID);
        fontSheet.QueueLoad(true);
        fontSheet.LockResource();

        auto textData = jsonUIElementData.FindMember("Text");
        if (textData == jsonUIElementData.MemberEnd())
        {
            m_text = "";
        }
        else
        {
            EXE_ASSERT(textData->value.IsString());
            m_text = textData->value.GetString();
        }

        auto horizontalAlignmentData = jsonUIElementData.FindMember("HorizontalAlignment");
        if (horizontalAlignmentData != jsonUIElementData.MemberEnd())
        {
            EXE_ASSERT(horizontalAlignmentData->value.IsNumber());
            int alignmentVal = horizontalAlignmentData->value.GetInt();
            if (alignmentVal >= (int)HorizontalAlignment::Error || alignmentVal <= 0)
                m_horizontalAlignment = HorizontalAlignment::Left;

            m_horizontalAlignment = static_cast<HorizontalAlignment>(alignmentVal);
        }

        auto verticalAlignmentData = jsonUIElementData.FindMember("VerticalAlignment");
        if (verticalAlignmentData != jsonUIElementData.MemberEnd())
        {
            EXE_ASSERT(verticalAlignmentData->value.IsNumber());
            int alignmentVal = verticalAlignmentData->value.GetInt();
            if (alignmentVal >= (int)VerticalAlignment::Error || alignmentVal <= 0)
                m_verticalAlignment = VerticalAlignment::Top;

            m_verticalAlignment = static_cast<VerticalAlignment>(alignmentVal);
        }

        auto sizeData = jsonUIElementData.FindMember("Size");
        if (sizeData != jsonUIElementData.MemberEnd())
        {
            EXE_ASSERT(sizeData->value.IsUint());
            unsigned int sizeVal = sizeData->value.GetUint();
            if (sizeVal <= 1)
            {
                m_textWidth = 1.0f;
                m_textHeight = 1.0f;
            }
            
            m_textWidth = static_cast<float>(sizeVal);
            m_textHeight = static_cast<float>(sizeVal);
        }

        return UIElement::Initialize(jsonUIElementData);
    }

    void UILabel::OnRender()
    {
        if (!m_fontID.IsValid())
        {
            // TODO: Logs here..
            return;
        }

        ResourceHandle font(m_fontID);
        auto* pFont = font.GetAs<FontResource>();
        if (!pFont)
            return;

        if (m_textWidth < pFont->GetDefaultFontWidth())
            m_textWidth = pFont->GetDefaultFontWidth();

        if (m_textHeight < pFont->GetDefaultFontHeight())
            m_textHeight = pFont->GetDefaultFontHeight();

        RenderCommand command;
        command.m_renderLayer = RenderCommand::RenderLayer::UI;
        command.m_texture = pFont->GetTextureResource();
        command.m_tint = m_color;

        // An optimization for this could be that we do it on
        // the text is changed, so it isn't done every frame.
        BuildLines();

        float nextX = 0.0f;
        float nextY = HandleVerticalAlignment();

        // This might seem O(n^3) but is is essentially O(n)
        // where n is the number of chars in m_text.
        for (auto& line : m_lineList)
        {
            nextX = HandleHorizontalAlignment(line);

            for (const auto& word : line.m_words)
            {
                for (char c : word.m_word)
                {
                    if (DidHandleSpecialCharacter(c, nextX))
                        continue;

                    command.m_sourceFrame = pFont->GetGlyphRect(c);

                    command.m_destinationFrame.x = nextX + m_actualRegion.x;
                    command.m_destinationFrame.y = nextY + m_actualRegion.y;
                    command.m_destinationFrame.h = m_textHeight;
                    command.m_destinationFrame.w = m_textWidth;
                    nextX += m_textWidth;

                    RenderManager::GetInstance()->PushRenderCommand(command);
                }
                nextX += word.m_wordSpacing;
            }
            nextY += m_textHeight;
        }

        UIElement::OnRender();
    }

    void UILabel::OnDestroy()
    {
        ResourceHandle texture(m_fontID);
        texture.UnlockResource();

        UIElement::OnDestroy();
    }

    bool UILabel::DidHandleSpecialCharacter(char wordsFirstCharacter, float& xToSet) const
    {
        switch (wordsFirstCharacter)
        {
        case ' ':
            xToSet += m_textWidth;
            return true;
            break;
        case '\t':
            xToSet += (m_textWidth * 4);
            return true;
            break;
        case '\n':
            return true; 
            break;
        default:
            // If it is any other char, we bail.
            return false;
            break;
        }
    }

    eastl::vector<Word> UILabel::ParseTextIntoWords()
    {
        eastl::string word;
        eastl::vector<Word> wordList;
        for (char c : m_text)
        {

            if (c == ' ' || c == '\t' || c == '\n')
            {
                if (!word.empty())
                {
                    wordList.emplace_back(word, CalculateWordWidth(word));
                    word.clear();
                }
                word += c;
                wordList.emplace_back(word, CalculateWordWidth(word));
                word.clear();
            }
            else
            {
                word += c;
            }
        }
        wordList.emplace_back(word, CalculateWordWidth(word));

        return wordList;
    }

    void UILabel::BuildLines()
    {
        m_lineList.clear();
        eastl::vector<Word> wordList = ParseTextIntoWords();
        Line currentLine;

        for (const Word& word : wordList)
        {
            currentLine.m_lineWidth += word.m_wordWidth;

            // Bail if the word width is smaller than a single character.
            // This occurs with newlines.
            if (word.m_wordWidth < m_textWidth)
            {
                // Add the newline word to this line.
                currentLine.m_words.emplace_back(word);
                // Add line to line list. (newline, end current line)
                m_lineList.emplace_back(currentLine);
                currentLine.m_words.clear();
                currentLine.m_lineWidth = 0.0f;
                continue;
            }

            // If the right side of the line width exceeds the
            // width of the label region, then move to the next line.
            if (currentLine.m_lineWidth > m_actualRegion.w)
            {
                // Remove the "test-fit" word.
                currentLine.m_lineWidth -= word.m_wordWidth;
                // Add line to line list. (Exceeded region width)
                m_lineList.emplace_back(currentLine);
                currentLine.m_words.clear();
                currentLine.m_lineWidth = word.m_wordWidth;
                currentLine.m_words.emplace_back(word);
                continue;
            }

            currentLine.m_words.emplace_back(word);
        }
        m_lineList.emplace_back(currentLine);
    }

    float UILabel::CalculateWordWidth(const eastl::string& word) const
    {
        switch (word[0])
        {
            case ' ':   return m_textWidth;
            case '\t':  return (m_textWidth * 4);
            case '\n':  return 0.0f;
            default:    return word.size() * m_textWidth;
        }
    }

    float UILabel::HandleHorizontalAlignment(Line& line) const
    {
        switch (m_horizontalAlignment)
        {
        case HorizontalAlignment::Left: return 0.0f;
        case HorizontalAlignment::Right: return m_actualRegion.w - line.m_lineWidth;
        case HorizontalAlignment::Centered: return (m_actualRegion.w - line.m_lineWidth) / 2.0f;
        case HorizontalAlignment::Justified: HandleJustifiedAlignment(line); return 0.0f;
        default: return 0.0f;
        }
    }

    void UILabel::HandleJustifiedAlignment(Line& line) const
    {
        float nextX = 0.0f;
        for (auto& word : line.m_words)
        {
            if (DidHandleSpecialCharacter(word.m_word[0], nextX))
                continue;

            word.m_wordSpacing = (m_actualRegion.w - line.m_lineWidth) / (float)(line.m_words.size() - 1);
        }
    }

    float UILabel::HandleVerticalAlignment() const
    {
        switch (m_verticalAlignment)
        {
        case VerticalAlignment::Top: return 0.0f;
        case VerticalAlignment::Centered: return (m_actualRegion.h - (m_lineList.size() * m_textHeight)) / 2.0f;
        case VerticalAlignment::Bottom: return (m_actualRegion.h - (m_lineList.size() * m_textHeight));
        default: return 0.0f;
        }
    }
}
