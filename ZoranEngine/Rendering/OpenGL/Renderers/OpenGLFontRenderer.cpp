#include "stdafx.h"
#include "OpenGLFontRenderer.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/OpenGL/GUI/OpenGLFontShader.h>
#include <Core/Resources/FontResource.h>
#include <Rendering/TextureBase.h>
#include <Math/Matrix44.hpp>
#include "OpenGLTriangleRenderer.h"

class FontRender
{
	OpenGLTriangleRenderer* renderer;
	Font* font;

	FontRender(Font* font, OpenGLContext* context) : font(font) 
	{
		renderer = new OpenGLTriangleRenderer(context);
	}

	~FontRender() {delete renderer; }

	friend class OpenGLFontRenderer;
};

FontRender * OpenGLFontRenderer::FindFont(Font* font)
{
	for(FontRender* fr : fontsToRender)
	{
		if (fr->font == font)
		{
			return fr;
		}
	}

	return 0;
}

OpenGLFontRenderer::OpenGLFontRenderer(OpenGLContext * context) :  context(context)
{
	shader = static_cast<OpenGLFontShader*>(rEngine->CreateShaderProgram<OpenGLFontShader>(OpenGLFontShader::initMap));
}

OpenGLFontRenderer::~OpenGLFontRenderer()
{
}

void OpenGLFontRenderer::RenderObject(const Matrix44 & cameraMatrix)
{
	shader->BindProgram();
	shader->setUniformMat4("MVP", &cameraMatrix[0]);
	shader->setUniform("msdf", 0);

	for (auto fr : fontsToRender)
	{
		fr->font->fontResource->GetFontTexture()->UseTexture(0);
		fr->renderer->RenderObject(cameraMatrix);
	}
}

void OpenGLFontRenderer::AddFontToRender(Font * font)
{
	static float scale = 10;

	FontRender* fr = new FontRender(font, context);

	std::vector<TrianglePrimitive> triangles(font->glyphs.size() * 2);

	float startX = font->renderStart.x;
	float startY = font->renderStart.y;

	size_t i = 0;

	for (uint32_t uni : font->glyphs)
	{
		TrianglePrimitive trianglel, triangler;
		Glyph glyph = font->fontResource->GlyphForUnicode(uni);

		if (uni == ' ')
		{
			startX += static_cast<float>(glyph.advance * scale);
			continue;
		}

		float uvAdvance = glyph.uvAdvance;

		float u = glyph.UVOffset.x;
		float v = glyph.UVOffset.y;

		Vector2D bearing = -glyph.bearing;

		float x = startX + (scale);
		float y = startY - (bearing.y * scale);

		float w = glyph.size.w * scale;
		float h = glyph.size.h * scale;

		trianglel.vertecies[0] = { x, y + h,0 };
		trianglel.vertecies[1] = { x, y, 0 };
		trianglel.vertecies[2] = { x + w, y, 0 };

		triangler.vertecies[0] = { x + w, y + h,0 };
		triangler.vertecies[1] = { x + w, y,0 };
		triangler.vertecies[2] = { x, y + h,0 };

		trianglel.coords[0] = { u, v + uvAdvance };
		trianglel.coords[1] = { u, v};
		trianglel.coords[2] = { u + uvAdvance, v };

		triangler.coords[0] = { u + uvAdvance, v + uvAdvance };
		triangler.coords[1] = { u + uvAdvance, v };
		triangler.coords[2] = { u, v + uvAdvance };

		size_t index = 2 * (i++);

		triangles[index] = trianglel;
		triangles[index + 1] = triangler;

		startX += static_cast<float>(glyph.advance * scale);
	}

	fr->renderer->AddTriangles(triangles);

	fontsToRender.push_back(fr);
}

void OpenGLFontRenderer::RemoveFontFromRender(Font * font)
{
	FontRender* fr = FindFont(font);
	if (fr)
	{
		fontsToRender.remove(fr);
		delete fr;
	}
}

bool OpenGLFontRenderer::FontIsInRender(Font * inFont)
{
	return FindFont(inFont) != 0;
}
