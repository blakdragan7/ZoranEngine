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
	shader = static_cast<OpenGLFontShader*>(rEngine->CreateShaderProgram<OpenGLFontShader>());
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

	fontResource->GetFontTexture()->UseTexture(0);
	renderer->RenderObject(cameraMatrix);
}

void OpenGLFontRenderer::UpdateRender()
{
	if (glyphs->size() < 1)return;
	//TODO: optimize changed text

	std::vector<TrianglePrimitive> triangles(glyphs->size() * 2);

	float startX = renderStart.x;
	float startY = renderStart.y;

	float scale = pptSize;

	size_t i = 0;

	float maxh = 0;

	for (uint32_t uni : *glyphs)
	{
		TrianglePrimitive trianglel, triangler;
		Glyph glyph = fontResource->GlyphForUnicode(uni);

		if (uni == ' ')
		{
			startX += static_cast<float>(glyph.advance * scale);
			continue;
		}

		float uvAdvance = glyph.uvAdvance;

		float u = glyph.UVOffset.x;
		float v = glyph.UVOffset.y;

		Vector2D bearing = glyph.bearing;

		start:

		float x = startX + (bearing.x * scale);
		float y = startY - (bearing.y * scale);

		float w = glyph.size.w * scale;
		float h = glyph.size.h * scale;

		if (shouldClip && (bounds.w <= ((x + w) - renderStart.x) || bounds.h <= (renderStart.y - (y + h))))
		{
			goto next;
		}

		if(glyph.size.h > maxh)
			maxh = glyph.size.h;

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

	next:

		startX += static_cast<float>(glyph.advance * scale);

		if (shouldWordWrap)
		{
			if (bounds.w <= (startX - renderStart.x))
			{
				startX = renderStart.x;
				startY -= (pptSize * 0.7333f);

				goto start;
			}
		}
	}

	Log(LogLevel_None, "Max Height %f", maxh);

	renderer->AddTriangles(triangles);
}

