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

struct TextGlyph
{
	Vector2D bl, tr;
	Vector2D baseline, endline;

	uint32_t glyph;

	TextGlyph(uint32_t glyph) : glyph(glyph) {}

	// returns -1 if closer to left edge 1 if close to right edge or 0 if no intersection

	inline int Intersects(Vec2D pos)const
	{
		if (bl.x <= pos.x && bl.y <= pos.y && tr.x >= pos.x && tr.y >= pos.y)
		{
			if (pos.x - bl.x < tr.x - pos.x) return -1;
			else return 1;
		}

		return 0;
	}

	void operator = (uint32_t glyph)
	{
		this->glyph = glyph;
	}

	bool operator == (const TextGlyph& other)const
	{
		return glyph == other.glyph;
	}

	bool operator == (const uint32_t other)const
	{
		return glyph == other;
	}

	bool operator != (const uint32_t other)const
	{
		return glyph != other;
	}

	bool operator == (Vec2D pos)const
	{
		return Intersects(pos) != 0;
	}
};

struct UniWord
{
	std::vector<TextGlyph> glyphs; // list of glyphs that make up this word
	double advance; // advances the whole word
	double spaceAdvance; // advances space after word
	NewLineType newLineType; // set if isNewLine is set to true
	size_t id;
	static size_t n_id;

	UniWord() : advance(0), spaceAdvance(0), id(n_id++) {}
	UniWord(const UniWord& other) : advance(other.advance), spaceAdvance(other.spaceAdvance), id(other.id) 
	{
		glyphs = std::vector<TextGlyph>(other.glyphs);
	}

	bool operator ==(const UniWord& other)
	{
		return id == other.id;
	}
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

	float spaceAdvance;

	// font sizing
	float pptSize;

	// clipping and word wrapping

	bool shouldWordWrap;
	bool shouldClip;

	// includes the edge of the rendered area and the width to wordwrap at
	Vector2D bounds; 

	bool isDirty;

private:
	bool UpdateWordFromGlyphInsert(UniWord& word, int position, uint32_t glyph);
	bool UpdateWordFromGlyph(UniWord& word, uint32_t glyph, bool& wasCarriageReturn,bool& wasNewLine, bool& wasTab);

protected:
	inline size_t GetCharCount()const { return charCount; }
	bool GlyphWalkForPos(int pos,UniWord** word, int& insertPos)const;

public:
	FontRenderer(FontResource* font);
	virtual ~FontRenderer();

	std::vector<UniWord>* words;

	virtual void UpdateRender() = 0;

	TextGlyph* GlyphForPos(int pos)const;
	int CursorPosForLocation(Vec2D location)const;
	int CursorPosInAboveLine(int pos)const;
	int CursorPosInBelowLine(int pos)const;

	int GetLastCursorPos()const;

	inline bool GetIsDirty()const { return isDirty; }

	inline FontResource* GetFontResource()const { return fontResource; }

	inline Vec2D GetRenderStart()const { return renderStart; }
	inline Vec2D GetTotalSize()const { return totalSize; };

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

	void InsertGlyph(uint32_t glyph, int pos);
	void AddGlyph(uint32_t glyph);
	void RemoveLastGlyph();
	void RemoveGlyphAtCursurPos(int pos);
	
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

