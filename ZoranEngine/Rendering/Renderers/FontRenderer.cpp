#include "stdafx.h"
#include "FontRenderer.h"
#include <Core/Resources/FontResource.h>
#include <Utils/VectorAddons.hpp>
#include <vector>
#include <stdarg.h>

size_t UniWord::n_id = 0;
size_t UniLine::n_id = 0;

bool FontRenderer::UpdateWordFromGlyphInsert(UniWord & word, int position, uint32_t glyph)
{
	double advance = fontResource->GlyphForUnicode(glyph).advance;;

	if (glyph == ' ')
	{
		UniWord copy = word;

		word.glyphs.erase(word.glyphs.begin() + position, word.glyphs.end());
		word.glyphs.push_back({ glyph, advance });
		word.spaceAdvance = spaceAdvance;
		word.advance += spaceAdvance;
		
		UniWord nextWord(word.line);
		nextWord.glyphs.insert(nextWord.glyphs.begin(), copy.glyphs.begin() + position, copy.glyphs.end());
		word.line->words.insert(std::find(word.line->words.begin(), word.line->words.end(), word) + 1, nextWord);

		return true;
	}
	else if (glyph == '\n' || glyph == '\r')
	{
		UniWord copy = word;

		word.glyphs.erase(word.glyphs.begin() + position, word.glyphs.end());
		word.glyphs.push_back({'\n', 0});

		UniLine line;
		UniWord nextWord(&line);
		nextWord.glyphs.insert(nextWord.glyphs.begin(), copy.glyphs.begin() + position, copy.glyphs.end());
		line.words.push_back(nextWord);

		// insert new line
		lines->insert(std::find(lines->begin(), lines->end(), *copy.line) + 1, line);

		lineCount++;

		return true;
	}
	else if (glyph == '\t')
	{
		UniWord copy = word;
		word.glyphs.erase(word.glyphs.begin() + position, word.glyphs.end());
		word.glyphs.push_back({ glyph, advance });

		UniWord nextWord(word.line);
		nextWord.glyphs.insert(nextWord.glyphs.begin(), copy.glyphs.begin() + position, copy.glyphs.end());
		word.line->words.insert(std::find(word.line->words.begin(), word.line->words.end(), word) + 1, nextWord);

		return true;
	}
	else
	{
		word.glyphs.insert(word.glyphs.begin() + position, { glyph, advance });
		word.advance += advance;
		charCount++;
		return false;
	}
}

bool FontRenderer::UpdateWordFromGlyph(UniWord & word, UniLine& line, uint32_t glyph, bool& wasCarriageReturn, bool& wasNewLine, bool& wasTab, float& currentLineSize)
{
	float uniformScale = 1.0f;

	double advance = fontResource->GlyphForUnicode(glyph).advance;

	if (glyph == ' ')
	{
		wasCarriageReturn = false;
		wasNewLine = false;
		wasTab = false;
		word.glyphs.push_back({ glyph, advance });
		word.spaceAdvance = spaceAdvance;
		word.advance += spaceAdvance;
		line.advance += spaceAdvance;
		line.words.push_back(word);
		word = UniWord(&line);
		currentLineSize += spaceAdvance;
		return true;
	}
	else if (glyph == '\n' || glyph == '\r')
	{
		if (wasCarriageReturn && glyph == '\n')
		{
			wasNewLine = true;
			wasCarriageReturn = false;
			line.words[line.words.size() - 1].newLineType = NewLineType_EOL;
			return false;
		}
		
		// we only use new line char for new lines no carriage returnes
		word.glyphs.push_back({ '\n', 0 });

		if (wasNewLine == false && wasTab == false)
		{
			line.words.push_back(word);

			line.advance = currentLineSize;

			lines->push_back(line);

			line = UniLine();
			word = UniWord(&line);

			lineCount++;

			maxLineSize = max(maxLineSize, currentLineSize);

			currentLineSize = 0;
		}

		wasNewLine = true;

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

		return true;
	}
	else if (glyph == '\t')
	{
		word.glyphs.push_back({ glyph, advance });
		
		currentLineSize+= spaceAdvance * 4;

		wasTab = true;
		wasNewLine = false;
		wasCarriageReturn = false;

		line.words.push_back(word);
		word = UniWord(&line);
		return true;
	}
	else
	{
		word.glyphs.push_back({ glyph, advance });
		word.advance += advance;

		wasCarriageReturn = false;
		wasNewLine = false;
		wasTab = false;
		currentLineSize += (float)advance * uniformScale;
		charCount++;
		return false;
	}
}

bool FontRenderer::GlyphWalkForPos(int pos, UniLine** line,UniWord** word, int& insertPos)const
{
	if (pos < -1)return false;
	
	int currentPos = 0;

	for (auto& c_line : *lines)
	{
		for (auto& c_word : c_line.words)
		{
			int iPos = 0;
			for (auto& c : c_word.glyphs)
			{
				if (pos == currentPos)
				{
					*line = &c_line;
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
	}

	return false;
}

void FontRenderer::UpdateRender()
{
	if (lines->size() < 1)return;

	int theCount = static_cast<int>(charCount);

	Glyph space = fontResource->GlyphForUnicode(' ');
	float* verts = new float[theCount * 36];
	float* uvs = new float[theCount * 24];

	float scale = pptSize;

	size_t i = 0;

	float newLineShift = pptSize;

	for (UniLine& line : *lines)
	{
		float startX = line.renderStart.x;
		float startY = line.renderStart.y;

		for (UniWord& word : line.words)
		{
			for (auto& text : word.glyphs)
			{
				uint32_t uni = text.glyph;

				if (uni == '\t')
				{
					text.bl.x = startX;
					text.bl.y = startY;

					text.tr.x = startX + static_cast<float>(space.advance * scale * 4);
					text.tr.y = startY + scale;

					text.baseline.x = startX;
					text.baseline.y = startY;

					text.endline.x = text.tr.x;
					text.endline.y = text.tr.y;

					startX += static_cast<float>(space.advance * scale * 4);
					continue;
				}

				if (uni == '\n')
				{
					text.baseline.x = line.renderStart.x;
					text.baseline.y = startY - newLineShift;

					text.bl.x = text.baseline.x;
					text.bl.y = text.baseline.y;

					text.tr.x = text.baseline.x;
					text.tr.y = text.baseline.y;

					text.endline.x = text.tr.x;
					text.endline.y = text.tr.y;

					continue;
				}

				Glyph glyph = fontResource->GlyphForUnicode(uni);

				if (uni == ' ')
				{
					text.bl.x = startX;
					text.bl.y = startY;

					text.baseline.x = startX;
					text.baseline.y = startY;

					startX += static_cast<float>(glyph.advance * scale);

					text.tr.x = startX + static_cast<float>(glyph.advance);
					text.tr.y = startY + scale;

					text.endline.x = text.tr.x;
					text.endline.y = text.tr.y;

					continue;
				}

				text.baseline.x = startX;
				text.baseline.y = startY;

				float u = glyph.UVOffset.x + (glyph.translate.x / 8.0f);
				float v = glyph.UVOffset.y + (glyph.translate.y / 8.0f);

				float uvxAdvance = glyph.uvAdvance - (glyph.translate.x / 4.0f);
				float uvyAdvance = glyph.uvAdvance - (glyph.translate.y / 4.0f);

				Vector2D bearing = glyph.bearing;

				float x = startX + (bearing.x * scale);
				float y = startY - (bearing.y * scale);

				float w = glyph.size.w * scale;
				float h = glyph.size.h * scale;

				if (shouldClip && (x < bottomLeft.x  || x > topRight.x || (y + h) < bottomLeft.y || y > topRight.y))
				{
					continue;
				}

				size_t vindex = 36 * (i);
				size_t uindex = 24 * (i++);

				text.bl.x = startX;
				text.bl.y = startY;

				text.tr.x = x + w;
				text.tr.y = y + h;

				text.endline.x = x + w * 0.85f;
				text.endline.y = y + h * 0.85f;

				// vert locations

				verts[vindex + 0] = x;
				verts[vindex + 1] = y + h;
				verts[vindex + 2] = 0;

				verts[vindex + 3] = x;
				verts[vindex + 4] = y;
				verts[vindex + 5] = 0;

				verts[vindex + 6] = x + w;
				verts[vindex + 7] = y;
				verts[vindex + 8] = 0;

				verts[vindex + 9] = x + w;
				verts[vindex + 10] = y + h;
				verts[vindex + 11] = 0;

				verts[vindex + 12] = x + w;
				verts[vindex + 13] = y;
				verts[vindex + 14] = 0;

				verts[vindex + 15] = x;
				verts[vindex + 16] = y + h;
				verts[vindex + 17] = 0;

				// texture coordinates

				uvs[uindex + 0] = u;
				uvs[uindex + 1] = v + uvyAdvance;

				uvs[uindex + 2] = u;
				uvs[uindex + 3] = v;

				uvs[uindex + 4] = u + uvxAdvance;
				uvs[uindex + 5] = v;

				uvs[uindex + 6] = u + uvxAdvance;
				uvs[uindex + 7] = v + uvyAdvance;

				uvs[uindex + 8] = u + uvxAdvance;
				uvs[uindex + 9] = v;

				uvs[uindex + 10] = u;
				uvs[uindex + 11] = v + uvyAdvance;

				startX += static_cast<float>((glyph.advance * scale));
			}
		}
	}

	PushToGPU(verts, theCount * 36, uvs, theCount * 24);

	delete[] verts;
	delete[] uvs;
}

FontRenderer::FontRenderer(FontResource* font) : isDirty(false), fontResource(font), shouldClip(true), shouldWordWrap(true),
		thickness(0.0f), border(0.0f), shadowSoftness(0.0f), shadowOpacity(0.0f), charCount(0), lineCount(0), maxLineSize(0)
{
	lines = new std::vector<UniLine>;
	shadowVector = Vector2D(0.0625f, 0.03125f);
	fontColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
	borderColor = Vector3D(1.0f, 0.0f, 0.0f);

	spaceAdvance = static_cast<float>(fontResource->GlyphForUnicode(' ').advance);
}

FontRenderer::~FontRenderer()
{
	delete lines;
}

void FontRenderer::UpdateTextRenderForAlignment(AlignmentBit alignment)
{
	float currentY = topRight.y - pptSize;
	
	for (size_t l=0;l<lines->size();l++)
	{
		bool wasSplit = false;
		auto& line = (*lines)[l];
		float currentX = bottomLeft.x;
		
		for (size_t i=0;i<line.words.size();i++)
		{
			auto& word = line.words[i];

			float wordSize = ((float)word.advance * pptSize);

			if (shouldWordWrap && wordSize > (bounds.w))
			{
				float si = 0;
				
				for (size_t ti=0; ti< word.glyphs.size();ti++)
				{
					auto& t = word.glyphs[ti];
					if (si +  ((float)t.advance * pptSize)> bounds.w)
					{
						UniWord nextWord(word.line);
						nextWord.glyphs.insert(nextWord.glyphs.begin(), word.glyphs.begin() + ti, word.glyphs.end());
						nextWord.spaceAdvance = word.spaceAdvance;
						nextWord.advance = (wordSize - si) / pptSize;

						word.advance -= nextWord.advance;
						word.glyphs.erase(word.glyphs.begin() + ti, word.glyphs.end());

						line.renderStart = { bottomLeft.x, currentY };

						UniLine newLine;
						newLine.words.push_back(nextWord);
						newLine.words.insert(newLine.words.begin() + 1, line.words.begin() + i + 1, line.words.end());

						line.words.erase(line.words.begin() + i + 1, line.words.end());

						lines->insert(lines->begin() + l + 1, newLine);

						wasSplit = true;
						break;
					}
					si += (float)t.advance * pptSize;
				}

				break;
			}

			currentX += wordSize;

			if (shouldWordWrap && currentX >= topRight.x)
			{
				UniLine newLine;
				newLine.words.insert(newLine.words.begin(), line.words.begin()+i,line.words.end());
				line.words.erase(line.words.begin() + i, line.words.end());

				currentX -= (wordSize + (spaceAdvance * pptSize));

				float spaceLeft = topRight.x - currentX;
				if (alignment & Alignment_Center)
				{
					line.renderStart.x = bottomLeft.x + (spaceLeft / 2.0f);
				}
				else if (alignment & Alignment_Right)
				{
					line.renderStart.x = bottomLeft.x + spaceLeft;
				}

				line.renderStart.y = currentY;

				lines->insert(lines->begin() + l + 1, newLine);

				wasSplit = true;
				break;
			}
		}
		if (wasSplit == false)
		{
			float spaceLeft = topRight.x - currentX;
			if (alignment & Alignment_Center)
			{
				line.renderStart.x = bottomLeft.x + (spaceLeft / 2.0f);
			}
			else if (alignment & Alignment_Right)
			{
				line.renderStart.x = bottomLeft.x + spaceLeft;
			}

			line.renderStart.y = currentY;
		}
		currentY -= pptSize;
	}
	
	isDirty = true;
}

TextGlyph * FontRenderer::GlyphForPos(int pos)const
{
	if(lines->size() < 1)
		return nullptr;
	
	UniLine* line;
	UniWord* word;
	int textPos;
	if (GlyphWalkForPos(pos, &line, &word, textPos))
	{
		return &word->glyphs[textPos];
	}
	else
	{
		return 0;
	}
}

int FontRenderer::CursorPosForLocation(Vec2D location)const
{
	int currentPos = 0;
	int closestPos = -1;

	Vector2D closestDiff(100000.0f,100000.0f);

	for (auto& line : *lines)
	{
		for (auto& w : line.words)
		{
			for (auto& bounds : w.glyphs)
			{
				if (auto i = bounds.Intersects(location))
				{
					return  i > 0 ? currentPos : currentPos - 1;
				}
				else
				{
					Vector2D diff(bounds.endline.x - location.x, bounds.baseline.y - location.y);
					diff = diff.getAbs();
					if (diff.y <= closestDiff.y)
					{
						if (diff.x <= closestDiff.x)
						{
							closestPos = currentPos;

							closestDiff.x = diff.x;
						}

						closestDiff.y = diff.y;
					}

					++currentPos;
				}
			}
		}
	}

	return closestPos;
}

int FontRenderer::CursorPosInAboveLine(int pos) const
{
	UniLine* line;
	UniWord* word = 0;
	int wordPos = 0;
	if (GlyphWalkForPos(pos, &line, &word, wordPos))
	{
		Vector2D newLocation(word->glyphs[wordPos].endline.x, word->glyphs[wordPos].baseline.y );

		newLocation.y += pptSize;

		return CursorPosForLocation(newLocation);
	}

	return pos;
}

int FontRenderer::CursorPosInBelowLine(int pos) const
{
	UniLine* line;
	UniWord* word = 0;
	int wordPos = 0;
	if (GlyphWalkForPos(pos, &line, &word, wordPos))
	{
		Vector2D newLocation(word->glyphs[wordPos].endline.x, word->glyphs[wordPos].baseline.y);

		newLocation.y -= pptSize * 0.5f;

		return CursorPosForLocation(newLocation);
		
	}

	return pos;
}

int FontRenderer::GetLastCursorPos() const
{
	int pos = 0;
	
	for (auto& line : *lines)
	{
		for (auto& w : line.words)
		{
			pos += static_cast<int>(w.glyphs.size());
		}
	}
	return pos;
}

inline void FontRenderer::SetBounds(Vec2D position, Vec2D Bounds)
{
	bottomLeft = position;
	topRight = position + Bounds;
	bounds = Bounds;
	isDirty = true;
}

void FontRenderer::InsertGlyph(uint32_t glyph, int pos)
{
	if (lines->size() == 0)
	{
		AddGlyph(glyph);
		return;
	}
	else if (lines->size() == 1 && (*lines)[0].words.size() == 0)
	{
		AddGlyph(glyph);
		return;
	}
	else if (lines->size() == 1 && (*lines)[0].words[0].glyphs.size() == 0)
	{
		UpdateWordFromGlyphInsert((*lines)[0].words[0], 0, glyph);
	}

	int insertPos = 0;
	UniWord* word = 0;
	UniLine* line = 0;

	if (GlyphWalkForPos(pos, &line, &word, insertPos))
	{
		UpdateWordFromGlyphInsert(*word, insertPos, glyph);
	}
	else
	{
		Log(LogLevel_Debug, "pos outside of range for InsertGlpyh, appending to end instead\n");
		AddGlyph(glyph);
	}

}

void FontRenderer::AddGlyph(uint32_t glyph)
{
	if (lines->size() == 0)
	{
		UniLine line;
		UniWord word(&line);
		UpdateWordFromGlyphInsert(word, 0, glyph);
		line.words.push_back(word);
		lines->push_back(line);
	}	
	else
	{
		if (lines->back().words.size() == 0)
		{
			UniLine& line = lines->back();
			UniWord word(&line);
			UpdateWordFromGlyphInsert(word, 0, glyph);
			line.words.push_back(word);
		}
		else
		{
			UniWord& word = lines->back().words.back();
			UpdateWordFromGlyphInsert(word, static_cast<int>(word.glyphs.size()), glyph);
		}
	}
	isDirty = true;
}

void FontRenderer::RemoveLastGlyph()
{
	if (lines->size() > 0)
	{
		if (lines->back().words.size() > 0)
		{
			UniWord& word = lines->back().words[lines->back().words.size() - 1];

			charCount--;
			if (word.glyphs.size() > 0)
			{
				word.glyphs.pop_back();
				if (word.glyphs.size() == 0)
					lines->back().words.pop_back();
			}
		}
	}
	isDirty = true;
}

void FontRenderer::RemoveGlyphAtCursurPos(int pos)
{
	UniLine* line = 0;
	UniWord* word = 0;
	int location = 0;

	if (GlyphWalkForPos(pos, &line, &word,location))
	{
		if (word->glyphs[location] != '\n' && word->glyphs[location] != '\t' && word->glyphs[location] != ' ')
			charCount--;
		word->glyphs.erase(word->glyphs.begin() + location);
		if (word->glyphs.size() == 0)
		{
			remove(line->words, *word);
			if(line->words.size() == 0)
				remove(*lines, *line);
		}
		isDirty = true;
	}
}

void FontRenderer::SetText(const char * text)
{
	lines->clear();
	const char* ptr = text;
	
	charCount = 0;
	lineCount = 0;
	maxLineSize = 0;
	float currentLineSize = 0;
	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	UniLine line;
	UniWord word(&line);
	for (; *ptr != 0; ptr++)
	{
		UpdateWordFromGlyph(word, line, *ptr, wasCarriageReturn, wasNewLine, wasTab, currentLineSize);

	}
	line.words.push_back(word);
	lines->push_back(line);

	maxLineSize = max(maxLineSize, currentLineSize);
	lineCount++;

	isDirty = true;
}

void FontRenderer::SetText(const char16_t * text)
{
	const char16_t* ptr = text;

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	lineCount = 0;
	maxLineSize = 0;
	float currentLineSize = 0;
	UniLine line;
	UniWord word(&line);
	for (; *ptr != 0; ptr++)
	{
		UpdateWordFromGlyph(word, line, *ptr,wasCarriageReturn,wasNewLine,wasTab, currentLineSize);
	}
	line.words.push_back(word);
	lines->push_back(line);

	maxLineSize = max(maxLineSize, currentLineSize);
	lineCount++;

	isDirty = true;
}

void FontRenderer::SetText(const char32_t * text)
{
	lines->clear();
	const char32_t* ptr = text;

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	lineCount = 0;
	maxLineSize = 0;
	float currentLineSize = 0;
	UniLine line;
	UniWord word(&line);
	for (; *ptr != 0; ptr++)
	{
		UpdateWordFromGlyph(word, line, *ptr, wasCarriageReturn, wasNewLine, wasTab, currentLineSize);
	}
	line.words.push_back(word);
	lines->push_back(line);

	maxLineSize = max(maxLineSize, currentLineSize);
	lineCount++;

	isDirty = true;
}

void FontRenderer::SetText(const std::string & text)
{
	lines->clear();

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	lineCount = 0;
	maxLineSize = 0;
	float currentLineSize = 0;
	UniLine line;
	UniWord word(&line);
	for (const char c : text)
	{
		UpdateWordFromGlyph(word, line, c, wasCarriageReturn, wasNewLine, wasTab, currentLineSize);
	}
	line.words.push_back(word);
	lines->push_back(line);

	maxLineSize = max(maxLineSize, currentLineSize);
	lineCount++;

	isDirty = true;
}

void FontRenderer::SetText(const std::wstring & text)
{
	lines->clear();

	bool wasCarriageReturn = false;
	bool wasNewLine = false;
	bool wasTab = false;
	charCount = 0;
	lineCount = 0;
	maxLineSize = 0;
	float currentLineSize = 0;
	UniLine line;
	UniWord word(&line);
	for (const wchar_t c : text)
	{
		UpdateWordFromGlyph(word, line, c, wasCarriageReturn, wasNewLine, wasTab, currentLineSize);
	}
	line.words.push_back(word);
	lines->push_back(line);

	maxLineSize = max(maxLineSize, currentLineSize);
	lineCount++;

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
