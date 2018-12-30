#include "stdafx.h"
#include "FontRenderer.h"
#include <Core/Resources/FontResource.h>
#include <Utils/VectorAddons.hpp>
#include <vector>
#include <stdarg.h>

size_t UniWord::n_id = 0;

bool FontRenderer::UpdateWordFromGlyphInsert(UniWord & word, int position, uint32_t glyph)
{
	if (glyph == ' ')
	{
		UniWord copy = word;

		word.glyphs.erase(word.glyphs.begin() + position, word.glyphs.end());
		word.glyphs.push_back(glyph);
		word.spaceAdvance = spaceAdvance;

		UniWord nextWord;
		nextWord.glyphs.insert(nextWord.glyphs.begin(), copy.glyphs.begin() + position, copy.glyphs.end());
		words->insert(std::find(words->begin(), words->end(), word) + 1, nextWord);

		return true;
	}
	else if (glyph == '\n' || glyph == '\r')
	{
		UniWord copy = word;

		word.glyphs.erase(word.glyphs.begin() + position, word.glyphs.end());
		word.glyphs.push_back('\n');

		UniWord nextWord;
		nextWord.glyphs.insert(nextWord.glyphs.begin(), copy.glyphs.begin() + position, copy.glyphs.end());
		words->insert(std::find(words->begin(), words->end(), word) + 1, nextWord);

		return true;
	}
	else if (glyph == '\t')
	{
		UniWord copy = word;
		word.glyphs.erase(word.glyphs.begin() + position, word.glyphs.end());
		word.glyphs.push_back(glyph);

		UniWord nextWord;
		nextWord.glyphs.insert(nextWord.glyphs.begin(), copy.glyphs.begin() + position, copy.glyphs.end());
		words->insert(std::find(words->begin(), words->end(), word) + 1, nextWord);

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

bool FontRenderer::UpdateWordFromGlyph(UniWord & word, uint32_t glyph, bool& wasCarriageReturn, bool& wasNewLine, bool& wasTab)
{
	if (glyph == ' ')
	{
		wasCarriageReturn = false;
		wasNewLine = false;
		wasTab = false;
		word.glyphs.push_back(glyph);
		word.spaceAdvance = spaceAdvance;
		words->push_back(word);
		word = UniWord();
		return true;
	}
	else if (glyph == '\n' || glyph == '\r')
	{
		if (wasCarriageReturn && glyph == '\n')
		{
			wasNewLine = true;
			wasCarriageReturn = false;
			(*words)[words->size() - 1].newLineType = NewLineType_EOL;
			return false;
		}
		
		wasNewLine = true;
		// we only use new line char for new lines no carriage returnes
		word.glyphs.push_back('\n');

		if (wasNewLine == false && wasTab == false)
		{
			words->push_back(word);
			word = UniWord();
		}
		
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
		
		wasTab = true;
		wasNewLine = false;
		wasCarriageReturn = false;

		words->push_back(word);
		word = UniWord();
		return true;
	}
	else
	{
		word.glyphs.push_back(glyph);
		word.advance += fontResource->GlyphForUnicode(glyph).advance;
		wasCarriageReturn = false;
		wasNewLine = false;
		wasTab = false;
		charCount++;
		return false;
	}
}

bool FontRenderer::GlyphWalkForPos(int pos, UniWord** word, int& insertPos)const
{
	if (pos < -1)return false;
	
	int currentPos = 0;

	for (auto& c_word : *words)
	{
		int iPos = 0;
		for (auto& c : c_word.glyphs)
		{
			if (pos == currentPos)
			{
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

	return false;
}

FontRenderer::FontRenderer(FontResource* font) : isDirty(false), fontResource(font), shouldClip(true), shouldWordWrap(true),
		thickness(0.0f), border(0.0f), shadowSoftness(0.0f), shadowOpacity(0.0f), charCount(0)
{
	words = new std::vector<UniWord>();
	shadowVector = Vector2D(0.0625f, 0.03125f);
	fontColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
	borderColor = Vector3D(1.0f, 0.0f, 0.0f);

	spaceAdvance = static_cast<float>(fontResource->GlyphForUnicode(' ').advance);
}

FontRenderer::~FontRenderer()
{
	delete words;
}

TextGlyph * FontRenderer::GlyphForPos(int pos)const
{
	if(words->size() < 1)
		return nullptr;
	
	UniWord* word;
	int textPos;
	if (GlyphWalkForPos(pos, &word, textPos))
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

	for (auto& w : *words)
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

	return closestPos;
}

int FontRenderer::CursorPosInAboveLine(int pos) const
{
	UniWord* word = 0;
	int wordPos = 0;
	if (GlyphWalkForPos(pos, &word, wordPos))
	{
		Vector2D newLocation(word->glyphs[wordPos].endline.x, word->glyphs[wordPos].baseline.y );

		newLocation.y += pptSize;

		return CursorPosForLocation(newLocation);
	}

	return pos;
}

int FontRenderer::CursorPosInBelowLine(int pos) const
{
	UniWord* word = 0;
	int wordPos = 0;
	if (GlyphWalkForPos(pos, &word, wordPos))
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
	
	for (auto& w : *words)
	{
		pos += static_cast<int>(w.glyphs.size());
	}
	
	return pos;
}

void FontRenderer::InsertGlyph(uint32_t glyph, int pos)
{
	int insertPos = 0;
	UniWord* word = 0;

	if (words->size() == 0)
	{
		UniWord word;
		UpdateWordFromGlyphInsert(word, 0, glyph);
		words->push_back(word);

		isDirty = true;
		return;
	}

	else if (words->size() == 1 && (*words)[0].glyphs.size() == 0)
	{
		UpdateWordFromGlyphInsert((*words)[0], 0, glyph);

		isDirty = true;
		return;
	}


	if (GlyphWalkForPos(pos, &word, insertPos))
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
	if (words->size() == 0)
	{
		UniWord word;
		UpdateWordFromGlyphInsert(word, 0, glyph);
		words->push_back(word);
	}
	else if (words->size() == 1 && (*words)[0].glyphs.size() == 0)
	{
		UpdateWordFromGlyphInsert((*words)[0], 0, glyph);
	}

	UniWord& word = (*words)[words->size() - 1];
	UpdateWordFromGlyphInsert(word, static_cast<int>(word.glyphs.size()), glyph);

	isDirty = true;
}

void FontRenderer::RemoveLastGlyph()
{
	if (words->size() > 0)
	{
		UniWord& word = (*words)[words->size() - 1];
		
		charCount--;
		if (word.glyphs.size() > 0)
		{
			word.glyphs.pop_back();
			if (word.glyphs.size() == 0)
				words->pop_back();
		}

	}
	isDirty = true;
}

void FontRenderer::RemoveGlyphAtCursurPos(int pos)
{
	UniWord* word = 0;
	int location = 0;

	if (GlyphWalkForPos(pos, &word,location))
	{
		if (word->glyphs[location] != '\n' && word->glyphs[location] != '\t' && word->glyphs[location] != ' ')
			charCount--;
		word->glyphs.erase(word->glyphs.begin() + location);
		if (word->glyphs.size() == 0)
		{
			remove(*words, *word);
		}
		isDirty = true;
	}
}

void FontRenderer::SetText(const char * text)
{
	words->clear();
	const char* ptr = text;
	
	charCount = 0;
	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	UniWord word;
	for (; *ptr != 0; ptr++)
	{
		UpdateWordFromGlyph(word, *ptr, wasCarriageReturn, wasNewLine, wasTab);

	}
	words->push_back(word);

	isDirty = true;
}

void FontRenderer::SetText(const char16_t * text)
{
	words->clear();
	const char16_t* ptr = text;

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	UniWord word;
	for (; *ptr != 0; ptr++)
	{
		UpdateWordFromGlyph(word,*ptr,wasCarriageReturn,wasNewLine,wasTab);
	}
	words->push_back(word);

	isDirty = true;
}

void FontRenderer::SetText(const char32_t * text)
{
	words->clear();
	const char32_t* ptr = text;

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	UniWord word;
	for (; *ptr != 0; ptr++)
	{
		UpdateWordFromGlyph(word, *ptr, wasCarriageReturn, wasNewLine, wasTab);
	}
	words->push_back(word);

	isDirty = true;
}

void FontRenderer::SetText(const std::string & text)
{
	words->clear();

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	UniWord word;
	for (const char c : text)
	{
		UpdateWordFromGlyph(word, c, wasCarriageReturn, wasNewLine, wasTab);
	}
	words->push_back(word);

	isDirty = true;
}

void FontRenderer::SetText(const std::wstring & text)
{
	words->clear();

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	UniWord word;

	for (const wchar_t c : text)
	{
		UpdateWordFromGlyph(word, c, wasCarriageReturn, wasNewLine, wasTab);
	}
	words->push_back(word);

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
