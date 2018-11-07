#include "stdafx.h"
#include "FontRenderer.h"
#include <Core/Resources/FontResource.h>
#include <stdarg.h>

void FontRenderer::UpdareWordFromGlyph(UniWord & word, uint32_t glyph, bool& wasCarriageReturn, bool& wasNewLine, bool& wasTab)
{
	if (glyph == ' ')
	{
		word.spaceAdvance = fontResource->GlyphForUnicode(' ').advance;
		words->push_back(word);
		word = UniWord();
	}
	else if (glyph == '\n' || glyph == '\r')
	{
		if (wasCarriageReturn && glyph == '\n')
		{
			wasNewLine = true;
			wasCarriageReturn = false;
			(*words)[words->size() - 1].newLineType = NewLineType_EOL;
			return;
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
	}
	else
	{
		word.glyphs.push_back(glyph);
		word.advance += fontResource->GlyphForUnicode(glyph).advance;
		wasCarriageReturn = false;
		wasNewLine = false;
		wasTab = false;
	}
}

size_t FontRenderer::GetCharCount()
{
	size_t count = 0;
	
	for (auto word : *words)
	{
		count += word.glyphs.size();
	}

	return count;
}

FontRenderer::FontRenderer(FontResource* font) : isDirty(false), fontResource(font), shouldClip(true), shouldWordWrap(true), pxRange(2.0f),
		thickness(0.0f), border(0.125f), shadowSoftness(0.5f), shadowOpacity(0.0f)
{
	words = new std::vector<UniWord>();
	shadowVector = Vector2D(0.0625f, 0.03125f);
	topColor = Vector3D(1.0f, 1.0f, 1.0f);
	bottomColor = Vector3D(1.0f, 1.0f, 1.0f);
	borderColor = Vector3D(1.0f, 0.0f, 0.0f);
}

FontRenderer::~FontRenderer()
{
	delete words;
}

void FontRenderer::SetText(const char * text)
{
	words->clear();
	const char* ptr = text;
	
	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	UniWord word;
	for (; *ptr != 0; ptr++)
	{
		
	}

	isDirty = true;
}

void FontRenderer::SetText(const char16_t * text)
{
	words->clear();
	const char16_t* ptr = text;

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	UniWord word;
	for (; *ptr != 0; ptr++)
	{
		UpdareWordFromGlyph(word,*ptr,wasCarriageReturn,wasNewLine,wasTab);
	}

	isDirty = true;
}

void FontRenderer::SetText(const char32_t * text)
{
	words->clear();
	const char32_t* ptr = text;

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	UniWord word;
	for (; *ptr != 0; ptr++)
	{
		UpdareWordFromGlyph(word, *ptr, wasCarriageReturn, wasNewLine, wasTab);
	}

	isDirty = true;
}

void FontRenderer::SetText(const std::string & text)
{
	words->clear();

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	UniWord word;
	for (const char c : text)
	{
		UpdareWordFromGlyph(word, c, wasCarriageReturn, wasNewLine, wasTab);
	}

	isDirty = true;
}

void FontRenderer::SetText(const std::wstring & text)
{
	words->clear();

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	UniWord word;

	for (const wchar_t c : text)
	{
		UpdareWordFromGlyph(word, c, wasCarriageReturn, wasNewLine, wasTab);
	}

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
