#pragma once
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/Primitives.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>

enum NewLineType
{
	NewLineType_Carriage_Return,
	NewLineType_Line_Feed,
	NewLineType_EOL,
};

struct UniWord
{
	std::vector<uint32_t> glyphs; // list of glyphs that make up this word
	double advance; // advances the whole word
	double spaceAdvance; // advances space after word
	bool isNewLine; // if this word is a new line
	NewLineType newLineType; // set if isNewLine is set to true
	bool isTab;

	UniWord() : isNewLine(false), isTab(false), advance(0), spaceAdvance(0) {}
};

class FontResource;
class ZoranEngine_EXPORT FontRenderer : public RenderedObjectBase
{
protected:
	size_t charCount;

	Vector2D totalSize;

	FontResource* fontResource;
	Vector2D renderStart;
	Vector2D renderSize;

	// Uniform values

	Color fontColor;
	Vector3D borderColor;
	Vector3D shadowColor;

	float thickness;
	float border;

	Vector2D shadowVector;
	float shadowSoftness;
	float shadowOpacity;

	// font sizing

	float pptSize;

	// clipping and word wrapping

	bool shouldWordWrap;
	bool shouldClip;

	// includes the edge of the rendered area and the width to wordwrap at
	Vector2D bounds; 

	bool isDirty;

private:
	void UpdareWordFromGlyph(UniWord& word, uint32_t glyph, bool& wasCarriageReturn,bool& wasNewLine, bool& wasTab);

protected:
	inline size_t GetCharCount() { return charCount; }

public:
	FontRenderer(FontResource* font);
	virtual ~FontRenderer();

	std::vector<UniWord>* words;


	virtual void UpdateRender() = 0;

	inline Vec2D GetTotalSize() { return totalSize; };

	inline void SetShadowColor(Vec3D color) { shadowColor = color; }
	inline void SetShadowVector(Vec2D vector) { shadowVector = vector; }
	inline void SetShadowSoftness(float softness) { shadowSoftness = softness; }
	inline void SetShadowOpacity(float opacity) { shadowOpacity = opacity; }

	inline void SetFontThickness(float thickness) { this->thickness = thickness; }
	inline void SetFontBorder(float border) { this->border = border; }

	inline void SetRenderStart(Vec2D start) { renderStart = start; isDirty= true;}
	inline void SetRenderSize(Vec2D size) { renderSize = size; isDirty = true;}

	inline void SetFontColor(const Color& bColor) { fontColor = bColor;}
	inline void SetBorderColor(const Color& bColor) { borderColor = bColor;}

	inline void SetBounds(Vec2D Bounds) { bounds = Bounds; isDirty = true;}

	inline void SetPPTSize(float _pptSize) { pptSize = _pptSize; isDirty = true; }
	inline float GetPPTSize()const { return pptSize; }

	inline void SetShouldWordWrap(bool wordWrap) { shouldWordWrap = wordWrap; isDirty = true;}
	inline void SetShouldClip(bool clip) { shouldClip = clip; isDirty = true;}

	inline bool GetShouldWordWrap()const { return shouldWordWrap; }
	inline bool GetShouldClip()const { return shouldClip; }

	// all of these set the actual text to be rendered

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

	virtual void RenderObject(const Matrix44& projection)override;
};

