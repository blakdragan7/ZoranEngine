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
	float pptSize;

	std::vector<uint32_t> glyphs;

	/* overrides current glyphs and replaces them with contents of text */
	void SetText(const char * text);
	/* overrides current glyphs and replaces them with contents of text */
	void SetText(const char16_t * text);
	/* overrides current glyphs and replaces them with contents of text */
	void SetText(const char32_t * text);
	/* overrides current glyphs and replaces them with contents of text */
	void SetText(const std::string& text);
	/* overrides current glyphs and replaces them with contents of text */
	void SetText(const std::wstring& text);
	/* overrides current glyphs and replaces them with contents of text */
	template<typename ... Args>
	void SetFormat(const char* format, Args ...args)
	{
		size_t size = snprintf(nullptr, 0, format, args ...) + 1; // Extra space for '\0'
		char* buf = new char[size];
		snprintf(buf, size, format, args ...);

		SetText(buf);
		delete[] buf;
	}
};

class FontRenderer : public RenderedObjectBase
{
public:
	FontRenderer();
	virtual ~FontRenderer();

	virtual void AddFontToRender(Font* font) = 0;
	virtual void RemoveFontFromRender(Font* font) = 0;
	virtual bool FontIsInRender(Font* inFont) = 0;
};

