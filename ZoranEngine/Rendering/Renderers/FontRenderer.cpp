#include "stdafx.h"
#include "FontRenderer.h"
#include <stdarg.h>

FontRenderer::FontRenderer()
{
}

FontRenderer::~FontRenderer()
{
}

void Font::SetText(const char * text)
{
	glyphs.clear();

	const char* ptr = text;
	for (; *ptr != 0; ptr++)
	{
		glyphs.push_back(*ptr);
	}
}

void Font::SetText(const char16_t * text)
{
	glyphs.clear();

	const char16_t* ptr = text;
	for (; *ptr != 0; ptr++)
	{
		glyphs.push_back(*ptr);
	}
}

void Font::SetText(const char32_t * text)
{
	glyphs.clear();

	const char32_t* ptr = text;
	for (; *ptr != 0; ptr++)
	{
		glyphs.push_back(*ptr);
	}
}

void Font::SetText(const std::string & text)
{
	glyphs.clear();

	for (const char c : text)
	{
		glyphs.push_back(c);
	}
}

void Font::SetText(const std::wstring & text)
{
	glyphs.clear();

	for (const wchar_t c : text)
	{
		glyphs.push_back(c);
	}
}

void Font::SetFormat(const char * format, ...)
{
	
}
