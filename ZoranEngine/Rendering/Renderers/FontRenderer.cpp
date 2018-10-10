#include "stdafx.h"
#include "FontRenderer.h"


FontRenderer::FontRenderer()
{
}


FontRenderer::~FontRenderer()
{
}

void FontRenderer::AddAsciiToRender(const char * ascii, Font* inFont)
{
	size_t len = strlen(ascii);
	for (size_t i = 0; i < len; i++)
	{
		inFont->glyphs.push_back(ascii[i]);
	}

	AddFontToRender(inFont);
}
