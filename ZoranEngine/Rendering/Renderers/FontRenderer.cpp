#include "stdafx.h"
#include "FontRenderer.h"
#include <Core/Resources/FontResource.h>
#include <Utils/VectorAddons.hpp>
#include <vector>
#include <stdarg.h>

size_t UniWord::n_id = 0;
size_t UniLine::n_id = 0;

bool FontRenderer::UpdateWordFromGlyphInsert(UniWord & word, int position, uint32_t glyph)
{
	if (glyph == ' ')
	{
		UniWord copy = word;

		word.glyphs.erase(word.glyphs.begin() + position, word.glyphs.end());
		word.glyphs.push_back(glyph);
		word.spaceAdvance = spaceAdvance;

		UniWord nextWord(word.line);
		nextWord.glyphs.insert(nextWord.glyphs.begin(), copy.glyphs.begin() + position, copy.glyphs.end());
		word.line->words.insert(std::find(word.line->words.begin(), word.line->words.end(), word) + 1, nextWord);

		return true;
	}
	else if (glyph == '\n' || glyph == '\r')
	{
		UniWord copy = word;

		word.glyphs.erase(word.glyphs.begin() + position, word.glyphs.end());
		word.glyphs.push_back('\n');

		UniLine line;
		UniWord nextWord(&line);
		nextWord.glyphs.insert(nextWord.glyphs.begin(), copy.glyphs.begin() + position, copy.glyphs.end());
		line.words.push_back(nextWord);

		// insert new line
		lines->insert(std::find(lines->begin(), lines->end(),copy.line) + 1, line);

		lineCount++;

		return true;
	}
	else if (glyph == '\t')
	{
		UniWord copy = word;
		word.glyphs.erase(word.glyphs.begin() + position, word.glyphs.end());
		word.glyphs.push_back(glyph);

		UniWord nextWord(word.line);
		nextWord.glyphs.insert(nextWord.glyphs.begin(), copy.glyphs.begin() + position, copy.glyphs.end());
		word.line->words.insert(std::find(word.line->words.begin(), word.line->words.end(), word) + 1, nextWord);

		return true;
	}
	else
	{
		word.glyphs.insert(word.glyphs.begin() + position, glyph);
		word.advance += fontResource->GlyphForUnicode(glyph).advance;
		charCount++;
		return false;
	}
}

bool FontRenderer::UpdateWordFromGlyph(UniWord & word, UniLine& line, uint32_t glyph, bool& wasCarriageReturn, bool& wasNewLine, bool& wasTab, float& currentLineSize)
{
	float uniformScale = 1.0f;

	if (glyph == ' ')
	{
		wasCarriageReturn = false;
		wasNewLine = false;
		wasTab = false;
		word.glyphs.push_back(glyph);
		word.spaceAdvance = spaceAdvance;
		line.words.push_back(word);
		word = UniWord(&line);
		currentLineSize += spaceAdvance;
		return true;
	}
	else if (glyph == '\n' || glyph == '\r')
	{
		if (wasCarriageReturn && glyph == '\n')
		{
			wasNewLine = true;
			wasCarriageReturn = false;
			line.words[line.words.size() - 1].newLineType = NewLineType_EOL;
			return false;
		}
		
		// we only use new line char for new lines no carriage returnes
		word.glyphs.push_back('\n');

		if (wasNewLine == false && wasTab == false)
		{
			line.words.push_back(word);

			line.advance = currentLineSize;

			lines->push_back(line);

			line = UniLine();
			word = UniWord(&line);

			lineCount++;

			maxLineSize = max(maxLineSize, currentLineSize);

			currentLineSize = 0;
		}

		wasNewLine = true;

		if (glyph == '\r')
		{
			word.newLineType = NewLineType_Carriage_Return;
			wasCarriageReturn = true;
		}
		else
		{
			word.newLineType = NewLineType_Line_Feed;
			wasCarriageReturn = false;
		}

		return true;
	}
	else if (glyph == '\t')
	{
		word.glyphs.push_back(glyph);
		
		currentLineSize+= spaceAdvance * 4;

		wasTab = true;
		wasNewLine = false;
		wasCarriageReturn = false;

		line.words.push_back(word);
		word = UniWord(&line);
		return true;
	}
	else
	{
		double advance = fontResource->GlyphForUnicode(glyph).advance;
		word.glyphs.push_back(glyph);
		word.advance += advance;

		wasCarriageReturn = false;
		wasNewLine = false;
		wasTab = false;
		currentLineSize += (float)advance * uniformScale;
		charCount++;
		return false;
	}
}

bool FontRenderer::GlyphWalkForPos(int pos, UniLine** line,UniWord** word, int& insertPos)const
{
	if (pos < -1)return false;
	
	int currentPos = 0;

	for (auto& c_line : *lines)
	{
		for (auto& c_word : c_line.words)
		{
			int iPos = 0;
			for (auto& c : c_word.glyphs)
			{
				if (pos == currentPos)
				{
					*line = &c_line;
					*word = &c_word;
					insertPos = iPos;

					return true;
				}
				else
				{
					currentPos++;
					iPos++;
				}
			}
		}
	}

	return false;
}

FontRenderer::FontRenderer(FontResource* font) : isDirty(false), fontResource(font), shouldClip(true), shouldWordWrap(true),
		thickness(0.0f), border(0.0f), shadowSoftness(0.0f), shadowOpacity(0.0f), charCount(0), lineCount(0), maxLineSize(0)
{
	lines = new std::vector<UniLine>;
	shadowVector = Vector2D(0.0625f, 0.03125f);
	fontColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
	borderColor = Vector3D(1.0f, 0.0f, 0.0f);

	spaceAdvance = static_cast<float>(fontResource->GlyphForUnicode(' ').advance);
}

FontRenderer::~FontRenderer()
{
	delete lines;
}

TextGlyph * FontRenderer::GlyphForPos(int pos)const
{
	if(lines->size() < 1)
		return nullptr;
	
	UniLine* line;
	UniWord* word;
	int textPos;
	if (GlyphWalkForPos(pos, &line, &word, textPos))
	{
		return &word->glyphs[textPos];
	}
	else
	{
		return 0;
	}
}

int FontRenderer::CursorPosForLocation(Vec2D location)const
{
	int currentPos = 0;
	int closestPos = -1;

	Vector2D closestDiff(100000.0f,100000.0f);

	for (auto& line : *lines)
	{
		for (auto& w : line.words)
		{
			for (auto& bounds : w.glyphs)
			{
				if (auto i = bounds.Intersects(location))
				{
					return  i > 0 ? currentPos : currentPos - 1;
				}
				else
				{
					Vector2D diff(bounds.endline.x - location.x, bounds.baseline.y - location.y);
					diff = diff.getAbs();
					if (diff.y <= closestDiff.y)
					{
						if (diff.x <= closestDiff.x)
						{
							closestPos = currentPos;

							closestDiff.x = diff.x;
						}

						closestDiff.y = diff.y;
					}

					++currentPos;
				}
			}
		}
	}

	return closestPos;
}

int FontRenderer::CursorPosInAboveLine(int pos) const
{
	UniLine* line;
	UniWord* word = 0;
	int wordPos = 0;
	if (GlyphWalkForPos(pos, &line, &word, wordPos))
	{
		Vector2D newLocation(word->glyphs[wordPos].endline.x, word->glyphs[wordPos].baseline.y );

		newLocation.y += pptSize;

		return CursorPosForLocation(newLocation);
	}

	return pos;
}

int FontRenderer::CursorPosInBelowLine(int pos) const
{
	UniLine* line;
	UniWord* word = 0;
	int wordPos = 0;
	if (GlyphWalkForPos(pos, &line, &word, wordPos))
	{
		Vector2D newLocation(word->glyphs[wordPos].endline.x, word->glyphs[wordPos].baseline.y);

		newLocation.y -= pptSize * 0.5f;

		return CursorPosForLocation(newLocation);
		
	}

	return pos;
}

int FontRenderer::GetLastCursorPos() const
{
	int pos = 0;
	
	for (auto& line : *lines)
	{
		for (auto& w : line.words)
		{
			pos += static_cast<int>(w.glyphs.size());
		}
	}
	return pos;
}

void FontRenderer::InsertGlyph(uint32_t glyph, int pos)
{
	if (lines->size() == 0)
	{
		AddGlyph(glyph);
		return;
	}
	else if (lines->size() == 1 && (*lines)[0].words.size() == 0)
	{
		AddGlyph(glyph);
		return;
	}
	else if (lines->size() == 1 && (*lines)[0].words[0].glyphs.size() == 0)
	{
		UpdateWordFromGlyphInsert((*lines)[0].words[0], 0, glyph);
	}

	int insertPos = 0;
	UniWord* word = 0;
	UniLine* line = 0;

	if (GlyphWalkForPos(pos, &line, &word, insertPos))
	{
		UpdateWordFromGlyphInsert(*word, insertPos, glyph);
	}
	else
	{
		Log(LogLevel_Debug, "pos outside of range for InsertGlpyh, appending to end instead\n");
		AddGlyph(glyph);
	}

}

void FontRenderer::AddGlyph(uint32_t glyph)
{
	if (lines->size() == 0)
	{
		UniLine line;
		UniWord word(&line);
		UpdateWordFromGlyphInsert(word, 0, glyph);
		line.words.push_back(word);
		lines->push_back(line);
	}	
	else
	{
		if (lines->back().words.size() == 0)
		{
			UniLine& line = lines->back();
			UniWord word(&line);
			UpdateWordFromGlyphInsert(word, 0, glyph);
			line.words.push_back(word);
		}
		else
		{
			UniWord& word = lines->back().words.back();
			UpdateWordFromGlyphInsert(word, static_cast<int>(word.glyphs.size()), glyph);
		}
	}
	isDirty = true;
}

void FontRenderer::RemoveLastGlyph()
{
	if (lines->size() > 0)
	{
		if (lines->back().words.size() > 0)
		{
			UniWord& word = lines->back().words[lines->back().words.size() - 1];

			charCount--;
			if (word.glyphs.size() > 0)
			{
				word.glyphs.pop_back();
				if (word.glyphs.size() == 0)
					lines->back().words.pop_back();
			}
		}
	}
	isDirty = true;
}

void FontRenderer::RemoveGlyphAtCursurPos(int pos)
{
	UniLine* line = 0;
	UniWord* word = 0;
	int location = 0;

	if (GlyphWalkForPos(pos, &line, &word,location))
	{
		if (word->glyphs[location] != '\n' && word->glyphs[location] != '\t' && word->glyphs[location] != ' ')
			charCount--;
		word->glyphs.erase(word->glyphs.begin() + location);
		if (word->glyphs.size() == 0)
		{
			remove(line->words, *word);
			if(line->words.size() == 0)
				remove(*lines, *line);
		}
		isDirty = true;
	}
}

void FontRenderer::SetText(const char * text)
{
	lines->clear();
	const char* ptr = text;
	
	charCount = 0;
	lineCount = 0;
	maxLineSize = 0;
	float currentLineSize = 0;
	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	UniLine line;
	UniWord word(&line);
	for (; *ptr != 0; ptr++)
	{
		UpdateWordFromGlyph(word, line, *ptr, wasCarriageReturn, wasNewLine, wasTab, currentLineSize);

	}
	lines->push_back(line);

	maxLineSize = max(maxLineSize, currentLineSize);
	lineCount++;

	isDirty = true;
}

void FontRenderer::SetText(const char16_t * text)
{
	const char16_t* ptr = text;

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	lineCount = 0;
	maxLineSize = 0;
	float currentLineSize = 0;
	UniLine line;
	UniWord word(&line);
	for (; *ptr != 0; ptr++)
	{
		UpdateWordFromGlyph(word, line, *ptr,wasCarriageReturn,wasNewLine,wasTab, currentLineSize);
	}
	lines->push_back(line);

	maxLineSize = max(maxLineSize, currentLineSize);
	lineCount++;

	isDirty = true;
}

void FontRenderer::SetText(const char32_t * text)
{
	lines->clear();
	const char32_t* ptr = text;

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	lineCount = 0;
	maxLineSize = 0;
	float currentLineSize = 0;
	UniLine line;
	UniWord word(&line);
	for (; *ptr != 0; ptr++)
	{
		UpdateWordFromGlyph(word, line, *ptr, wasCarriageReturn, wasNewLine, wasTab, currentLineSize);
	}
	lines->push_back(line);

	maxLineSize = max(maxLineSize, currentLineSize);
	lineCount++;

	isDirty = true;
}

void FontRenderer::SetText(const std::string & text)
{
	lines->clear();

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	lineCount = 0;
	maxLineSize = 0;
	float currentLineSize = 0;
	UniLine line;
	UniWord word(&line);
	for (const char c : text)
	{
		UpdateWordFromGlyph(word, line, c, wasCarriageReturn, wasNewLine, wasTab, currentLineSize);
	}
	lines->push_back(line);

	maxLineSize = max(maxLineSize, currentLineSize);
	lineCount++;

	isDirty = true;
}

void FontRenderer::SetText(const std::wstring & text)
{
	lines->clear();

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	lineCount = 0;
	maxLineSize = 0;
	float currentLineSize = 0;
	UniLine line;
	UniWord word(&line);
	for (const wchar_t c : text)
	{
		UpdateWordFromGlyph(word, line, c, wasCarriageReturn, wasNewLine, wasTab, currentLineSize);
	}
	lines->push_back(line);

	maxLineSize = max(maxLineSize, currentLineSize);
	lineCount++;

	isDirty = true;
}

void FontRenderer::RenderObject(const Matrix44 & projection)
{
	if (isDirty)
	{
		UpdateRender();
		isDirty = false;
	}
}
