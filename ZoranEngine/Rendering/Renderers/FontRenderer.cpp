#include "stdafx.h"
#include "FontRenderer.h"
#include <Core/Resources/FontResource.h>
#include <stdarg.h>

bool FontRenderer::UpdareWordFromGlyphSingle(UniWord & word, uint32_t glyph, bool & wasCarriageReturn, bool & wasNewLine, bool & wasTab)
{
	if (glyph == ' ')
	{
		word.glyphs.push_back(glyph);
		word.spaceAdvance = spaceAdvance;
		words->push_back(UniWord());
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
		if (wasNewLine == false && wasTab == false)
		{
			words->push_back(UniWord());
		}

		wasNewLine = true;
		word.isNewLine = true;

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

		words->push_back(UniWord());

		return true;
	}
	else if (glyph == '\t')
	{
		if (!wasTab && !wasNewLine)
		{
			words->push_back(word);
		}
		word.isTab = true;
		wasTab = true;
		wasNewLine = false;
		wasCarriageReturn = false;

		words->push_back(UniWord());
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

bool FontRenderer::UpdareWordFromGlyph(UniWord & word, uint32_t glyph, bool& wasCarriageReturn, bool& wasNewLine, bool& wasTab)
{
	if (glyph == ' ')
	{
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
		if (wasNewLine == false && wasTab == false)
		{
			words->push_back(word);
			word = UniWord();
		}

		wasNewLine = true;
		word.isNewLine = true;

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

		words->push_back(word);

		word = UniWord();
		return true;
	}
	else if (glyph == '\t')
	{
		if (!wasTab && !wasNewLine)
		{
			words->push_back(word);
		}
		word = UniWord();
		word.isTab = true;
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
	int currentPos = 0;

	for (auto& c_word : *words)
	{
		int iPos = 0;
		for (auto c : c_word.glyphs)
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
	wasListSingleCarriageReturn = false;
	wasListSingleNewLine = false;
	wasListSingleTab = false;

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
		int i = 1;

		while (true)
		{
			if (words->size() - i < 0)return 0;
			UniWord& word = (*words)[words->size() - i];
			if (word.glyphs.size() == 0)
			{
				i++;
				continue;
			}
			return &word.glyphs[word.glyphs.size() - 1];
		}
	}
}

int FontRenderer::CursorPosForLocation(Vec2D location)const
{
	int currentPos = 0;

	for (auto& w : *words)
	{
		for (auto& bounds : w.glyphs)
		{
			if (bounds.Intersects(location))
			{
				return currentPos;
			}
			else currentPos++;
		}
		currentPos++;
	}

	return currentPos;
}

void FontRenderer::InsertGlyph(uint32_t glyph, int pos)
{
	int insertPos = 0;
	UniWord* word = 0;

	if (GlyphWalkForPos(pos, &word, insertPos))
	{
		word->glyphs.insert(word->glyphs.begin() + insertPos, glyph);
		charCount++;
	}
	else
	{
		Log(LogLevel_Warning, "pos outside of range for InsertGlpyh, appending to end instead");
		AddGlyph(glyph);
	}

	isDirty = true;
}

void FontRenderer::AddGlyph(uint32_t glyph)
{
	if (words->size() < 1)
	{
		UniWord word;

		if (UpdareWordFromGlyphSingle(word, glyph, wasListSingleCarriageReturn, wasListSingleNewLine, wasListSingleTab) == false)
			words->push_back(word);
	}
	else
	{
		UniWord& word = (*words)[words->size() - 1];

		UpdareWordFromGlyphSingle(word, glyph, wasListSingleCarriageReturn, wasListSingleNewLine, wasListSingleTab);
	}

	isDirty = true;
}

void FontRenderer::RemoveLastGlyph()
{
	if (words->size() > 0)
	{
		UniWord& word = (*words)[words->size() - 1];
		if (word.glyphs.size() > 0)
		{
			// needs charCount--
			// but that breaks it for some reason
			if((word.isNewLine || word.isTab) == false)
				charCount--;
			word.glyphs.pop_back();
		}
		else
		{
			words->pop_back();
		}

	}
	isDirty = true;
}

void FontRenderer::RemoveGlyphAtCursur()
{
	RemoveLastGlyph();
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
		UpdareWordFromGlyph(word, *ptr, wasCarriageReturn, wasNewLine, wasTab);

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
		UpdareWordFromGlyph(word,*ptr,wasCarriageReturn,wasNewLine,wasTab);
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
		UpdareWordFromGlyph(word, *ptr, wasCarriageReturn, wasNewLine, wasTab);
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
		UpdareWordFromGlyph(word, c, wasCarriageReturn, wasNewLine, wasTab);
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
		UpdareWordFromGlyph(word, c, wasCarriageReturn, wasNewLine, wasTab);
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
