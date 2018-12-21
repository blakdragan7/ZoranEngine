#include "stdafx.h"
#include "OpenGLFontRenderer.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/OpenGL/GUI/OpenGLFontShader.h>
#include <Core/Resources/FontResource.h>
#include <Rendering/TextureBase.h>
#include <Math/Matrix44.h>
#include "OpenGLIndexedTriangleRenderer.h"
#include "OpenGLTriangleRenderer.h"

void OpenGLFontRenderer::SetupTriangles()
{
	// * 2 because there are two triangles per quad
	std::vector<TrianglePrimitive> triangles(fontResource->GetNumberOfGlyphs() * 2);

	size_t i = 0;
	for (auto& itr : *fontResource)
	{
		{
			TrianglePrimitive trianglel, triangler;
			Glyph glyph = fontResource->GlyphForUnicode(itr.first);

			float u = glyph.UVOffset.x + (glyph.translate.x / 4.0f);
			float v = glyph.UVOffset.y + (glyph.translate.y / 8.0f);

			float uvxAdvance = glyph.uvAdvance - (glyph.translate.x / 4.0f);
			float uvyAdvance = glyph.uvAdvance - (glyph.translate.y / 3.0f);

			Vector2D bearing = glyph.bearing;

			float x = (bearing.x);
			float y = (bearing.y);

			float w = glyph.size.w;
			float h = glyph.size.h;

			trianglel.vertecies[0] = { x, y + h,0 };
			trianglel.vertecies[1] = { x, y, 0 };
			trianglel.vertecies[2] = { x + w, y, 0 };

			triangler.vertecies[0] = { x + w, y + h,0 };
			triangler.vertecies[1] = { x + w, y,0 };
			triangler.vertecies[2] = { x, y + h,0 };

			trianglel.coords[0] = { u, v + uvyAdvance };
			trianglel.coords[1] = { u, v };
			trianglel.coords[2] = { u + uvxAdvance, v };

			triangler.coords[0] = { u + uvxAdvance, v + uvyAdvance };
			triangler.coords[1] = { u + uvxAdvance, v };
			triangler.coords[2] = { u, v + uvyAdvance };

			size_t index = 2 * (i++);

			triangles[index] = trianglel;
			triangles[index + 1] = triangler;

			GlyphIndex gI;

			gI.indecies[0] = (i * 6) + 0;
			gI.indecies[1] = (i * 6) + 1;
			gI.indecies[2] = (i * 6) + 2;
			gI.indecies[3] = (i * 6) + 3;

			gI.normSize = static_cast<float>(glyph.advance);

			indecieMap.insert({ itr.first, gI });
		}

		renderer->AddTriangles(triangles);
	}
}

OpenGLFontRenderer::OpenGLFontRenderer(FontResource* font,OpenGLContext * context) :  context(context) , FontRenderer(font)
{
	indecieMap.set_empty_key(0);
	indecieMap.set_deleted_key(-1);
	renderer = new OpenGLTriangleRenderer(context);
	if(font->GetType() == Font_SDF_Type_MSDF)
		shader = static_cast<OpenGLShaderProgramBase*>(rEngine->CreateShaderProgram<OpenGLMSDFFontShader>());
	else
		shader = static_cast<OpenGLShaderProgramBase*>(rEngine->CreateShaderProgram<OpenGLSDFFontShader>());
}

OpenGLFontRenderer::~OpenGLFontRenderer()
{
	delete renderer;
}

void OpenGLFontRenderer::RenderObject(const Matrix44 & cameraMatrix)
{
	if (charCount == 0)return;

	FontRenderer::RenderObject(cameraMatrix);

	shader->BindProgram();
	shader->setUniformMat4("MVP", &cameraMatrix[0]);
	shader->setUniform("msdf", 0);
	shader->setUniform("fontColor", &fontColor);
	shader->setUniform("borderColor", &borderColor);
	shader->setUniform("shadowVector", &shadowVector);
	shader->setUniform("shadowColor", &shadowColor);
	shader->setUniform("pxRange", fontResource->GetPxRange());
	shader->setUniform("thickness", thickness);
	shader->setUniform("border", border);
	shader->setUniform("shadowSoftness", shadowSoftness);
	shader->setUniform("shadowOpacity", shadowOpacity);

	fontResource->GetFontTexture()->UseTexture(0);
	renderer->RenderObject(cameraMatrix);
}

void OpenGLFontRenderer::UpdateRender()
{
	if (words->size() < 1)return;
	//TODO: optimize changed text

	Glyph space = fontResource->GlyphForUnicode(' ');
	float* verts = new float[charCount * 36];
	float* uvs = new float[charCount * 24];

	float startX = renderStart.x;
	float startY = renderStart.y;

	float maxX = 0;
	float maxY = 0;

	float scale = pptSize;

	size_t i = 0;

	float maxh = 0;

	float newLineShift = (pptSize);// +(pptSize * 0.05f);

	for (UniWord& word : *words)
	{
		// render a new line when newline char
		if (word.isNewLine)
		{
			startX = renderStart.x;
			startY -= newLineShift;
			continue;
		}

		if (word.isTab)
		{
			startX += static_cast<float>(space.advance * scale * 4);
			continue;
		}

		if (shouldWordWrap)
		{
			if (bounds.w <= ((startX + (word.advance * scale)) - renderStart.x))
			{
				startX = renderStart.x;
				startY -= newLineShift;
			}
		}

		for (uint32_t uni : word.glyphs)
		{
			
			Glyph glyph = fontResource->GlyphForUnicode(uni);

			if (uni == ' ')
			{
				startX += static_cast<float>(glyph.advance * scale);
				continue;
			}

			float u = glyph.UVOffset.x + (glyph.translate.x / 4.0f);
			float v = glyph.UVOffset.y + (glyph.translate.y / 8.0f);
			
			float uvxAdvance = glyph.uvAdvance - (glyph.translate.x / 4.0f);
			float uvyAdvance = glyph.uvAdvance - (glyph.translate.y / 3.0f);

			Vector2D bearing = glyph.bearing;

			float x = startX + (bearing.x * scale);
			float y = startY - (bearing.y * scale);

			float w = glyph.size.w * scale;
			float h = glyph.size.h * scale;

			float diffx = (x + w) - renderStart.x;
			float diffy = (renderStart.y - y + h);

			if (shouldClip && (bounds.w <= diffx || bounds.h <= diffy))
			{
				continue;
			}

			if (glyph.size.h > maxh)
				maxh = glyph.size.h;

			size_t vindex = 36 * (i);
			size_t uindex = 24 * (i++);

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

			startX += static_cast<float>(glyph.advance * scale);
		}

		startX += static_cast<float>(word.spaceAdvance * scale);
	}

	renderer->AddTriangles(verts, charCount * 36,uvs, charCount * 24);

	delete[] verts;
	delete[] uvs;
}
