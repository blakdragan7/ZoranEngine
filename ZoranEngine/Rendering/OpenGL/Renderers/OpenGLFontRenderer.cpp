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
	indecieMap.set_empty_key(-1);

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

void OpenGLFontRenderer::PushToGPU(float * verts, size_t vertSize, float * uvs, size_t uvSize)
{
	renderer->AddTriangles(verts, vertSize, uvs, uvSize);
}
