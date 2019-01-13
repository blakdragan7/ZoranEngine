#pragma once
#include <Rendering/RenderedObjectBase.h>
#include <Rendering/Primitives.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>

enum NewLineType
{
	NewLineType_Carriage_Return, // \r
	NewLineType_Line_Feed, // \n
	NewLineType_EOL, // \r\n
};

struct TextGlyph
{
	Vector2D bl, tr;
	Vector2D baseline, endline;

	uint32_t glyph;
	float advance;

	TextGlyph(uint32_t glyph, float advance) : glyph(glyph), advance(advance) {}

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

struct UniLine;
struct UniWord
{
	std::vector<TextGlyph> glyphs; // list of glyphs that make up this word
	float advance; // advances the whole word
	NewLineType newLineType; // set if isNewLine is set to true
	size_t id;
	static size_t n_id;

	UniWord() : advance(0), id(n_id++) {}
	UniWord(const UniWord& other) : advance(other.advance), id(other.id) 
	{
		glyphs = std::vector<TextGlyph>(other.glyphs);
	}

	bool operator ==(const UniWord& other)
	{
		return id == other.id;
	}
};

struct UniLine
{
	size_t id;
	static size_t n_id;
	Vector2D renderStart;// The starting render location for this line
	std::vector<UniWord> words; // list of words that make up this line
	float advance; // advances the whole line

	UniLine() : id(n_id++), advance(0){}

	UniLine(const UniLine& other) : advance(other.advance), id(other.id), renderStart(other.renderStart)
	{
		words = std::vector<UniWord>(other.words);
	}

	bool operator ==(const UniLine& other)
	{
		return id == other.id;
	}

	bool operator ==(const size_t& other)
	{
		return id == other;
	}
};

class FontResource;
class ZoranEngine_EXPORT FontRenderer : public RenderedObjectBase
{
protected:
	FontResource* fontResource;

	std::vector<UniLine>* lines;

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

	// font rendering

	size_t charCount;

	size_t lineCount;
	float maxLineSize;

	// clipping and word wrapping

	bool shouldWordWrap;
	bool shouldClip;
	
	Vector2D bottomLeft; // the bottom left bounds of the text bounds
	Vector2D topRight; // the topRight bounds of the text bounds
	Vector2D bounds; // the size of the text bounds

	bool isDirty;

	AlignmentBit alignment;

private:
	bool UpdateWordFromGlyphInsert(int lineIndex,int wordIndex, int position, uint32_t glyph);
	bool UpdateWordFromGlyph(UniWord& word, UniLine& line,uint32_t glyph, bool& wasCarriageReturn,bool& wasNewLine, bool& wasTab);

protected:
	inline size_t GetCharCount()const { return charCount; }
	bool GlyphWalkForPos(int pos, int& lineIt, int& wordIt, int& insertPos)const;

	virtual void PushToGPU(float* verts, size_t vertSize, float* uvs, size_t uvSize) = 0;
	virtual void UpdateRender();

public:
	FontRenderer(FontResource* font);
	virtual ~FontRenderer();

	virtual void UpdateTextRenderForAlignment(AlignmentBit alignment);

	const std::vector<UniLine>& GetLines()const { return *lines; }

	bool GlyphForPos(int inPos, int& outLineIndex, int& outWordIndex, int& outTextPos)const;
	int CursorPosForLocation(Vec2D location)const;
	int CursorPosInAboveLine(int pos)const;
	int CursorPosInBelowLine(int pos)const;

	int GetLastCursorPos()const;

	inline void SetAlignment(AlignmentBit alignment) { this->alignment = alignment; isDirty = true; }

	inline Vec2D GetBottomLeft()const { return bottomLeft; }

	inline float GetMaxLineSize()const { return maxLineSize; }
	inline size_t GetLineCount()const { return lines->size(); }

	inline void SetDirty() { isDirty = true; }
	inline bool GetIsDirty()const { return isDirty; }

	inline FontResource* GetFontResource()const { return fontResource; }

	inline void SetShadowColor(Vec3D color) { shadowColor = color; }
	inline void SetShadowVector(Vec2D vector) { shadowVector = vector; }
	inline void SetShadowSoftness(float softness) { shadowSoftness = softness; }
	inline void SetShadowOpacity(float opacity) { shadowOpacity = opacity; }

	inline void SetFontThickness(float thickness) { this->thickness = thickness; }
	inline void SetFontBorder(float border) { this->border = border; }

	inline void SetFontColor(const Color& bColor) { fontColor = bColor;}
	inline void SetBorderColor(const Color& bColor) { borderColor = bColor;}

	inline void SetBounds(Vec2D position, Vec2D Bounds);

	inline void SetPPTSize(float _pptSize) { pptSize = _pptSize; isDirty = true; }
	inline float GetPPTSize()const { return pptSize; }

	inline void SetShouldWordWrap(bool wordWrap) { shouldWordWrap = wordWrap; isDirty = true;}
	inline void SetShouldClip(bool clip) { shouldClip = clip; isDirty = true;}

	inline bool GetShouldWordWrap()const { return shouldWordWrap; }
	inline bool GetShouldClip()const { return shouldClip; }

	// text insertion

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

