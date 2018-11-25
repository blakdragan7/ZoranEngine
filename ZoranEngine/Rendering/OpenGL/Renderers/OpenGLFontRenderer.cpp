#include "stdafx.h"
#include "OpenGLFontRenderer.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/OpenGL/GUI/OpenGLFontShader.h>
#include <Core/Resources/FontResource.h>
#include <Rendering/TextureBase.h>
#include <Math/Matrix44.h>
#include "OpenGLTriangleRenderer.h"

OpenGLFontRenderer::OpenGLFontRenderer(FontResource* font,OpenGLContext * context) :  context(context) , FontRenderer(font)
{
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

	std::vector<TrianglePrimitive> triangles(GetCharCount() * 2);

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
			Glyph space = fontResource->GlyphForUnicode(' ');

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
			
			TrianglePrimitive trianglel, triangler;
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

			startX += static_cast<float>(glyph.advance * scale);

			maxX = max(abs(x - renderStart.x),max(abs(startX - renderStart.x), maxX));
			maxY = max(abs(y - renderStart.y),max(abs(startY - renderStart.y), maxY));

		}

		startX += static_cast<float>(word.spaceAdvance * scale);

		max(abs(startX - renderStart.x), maxX);
		max(abs(startY - renderStart.y), maxY);
	}

	totalSize = Vector2D(maxX, maxY).getAbs();

	renderer->AddTriangles(triangles);
}

