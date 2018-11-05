#include "stdafx.h"
#include "FontRenderer.h"
#include <stdarg.h>

FontRenderer::FontRenderer(FontResource* font) : isDirty(false), fontResource(font), shouldClip(true), shouldWordWrap(true), pxRange(2.0f), 
		thickness(0.0f), border(0.125f), shadowSoftness(0.5f), shadowOpacity(0.5f)
{
	glyphs = new std::vector<uint32_t>();
	shadowVector = Vector2D(0.0625f, 0.03125f);
	topColor = Vector3D(1.0f, 1.0f, 1.0f);
	bottomColor = Vector3D(1.0f, 1.0f, 1.0f);
	borderColor = Vector3D(1.0f, 0.0f, 0.0f);
}

FontRenderer::~FontRenderer()
{
	delete glyphs;
}

void FontRenderer::SetText(const char * text)
{
	glyphs->clear();

	const char* ptr = text;
	for (; *ptr != 0; ptr++)
	{
		glyphs->push_back(*ptr);
	}

	isDirty = true;
}

void FontRenderer::SetText(const char16_t * text)
{
	glyphs->clear();

	const char16_t* ptr = text;
	for (; *ptr != 0; ptr++)
	{
		glyphs->push_back(*ptr);
	}

	isDirty = true;
}

void FontRenderer::SetText(const char32_t * text)
{
	glyphs->clear();

	const char32_t* ptr = text;
	for (; *ptr != 0; ptr++)
	{
		glyphs->push_back(*ptr);
	}

	isDirty = true;
}

void FontRenderer::SetText(const std::string & text)
{
	glyphs->clear();

	for (const char c : text)
	{
		glyphs->push_back(c);
	}

	isDirty = true;
}

void FontRenderer::SetText(const std::wstring & text)
{
	glyphs->clear();

	for (const wchar_t c : text)
	{
		glyphs->push_back(c);
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
