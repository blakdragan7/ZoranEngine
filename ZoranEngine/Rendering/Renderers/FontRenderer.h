#pragma once
#include <Rendering/RenderedObjectBase.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>

class FontResource;
struct Font
{
	FontResource* fontResource;
	Vector2D renderStart;
	Vector2D renderSize;
	Vector3D forgroundColor;
	Vector2D backgroundColor;

	std::vector<uint32_t> glyphs;
};

class FontRenderer : public RenderedObjectBase
{
public:
	FontRenderer();
	virtual ~FontRenderer();

	virtual void AddAsciiToRender(const char* ascii, Font* inFont);
	virtual void AddFontToRender(Font* font) = 0;
	virtual void RemoveFontFromRender(Font* font) = 0;
	virtual bool FontIsInRender(Font* inFont) = 0;
};

