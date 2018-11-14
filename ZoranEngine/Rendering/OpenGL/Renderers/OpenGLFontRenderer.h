#pragma once
#include <Rendering\Renderers\FontRenderer.h>

#include <list>

class OpenGLContext;
class FontResource;
class Matrix44;
class OpenGLShaderProgramBase;
class OpenGLTriangleRenderer;
class OpenGLFontRenderer : public FontRenderer
{
private:
	OpenGLContext * context;
	OpenGLShaderProgramBase* shader;
	OpenGLTriangleRenderer* renderer;

public:
	OpenGLFontRenderer(FontResource* font, OpenGLContext * context);
	~OpenGLFontRenderer();

	virtual void RenderObject(const Matrix44& cameraMatrix)override;
	virtual void UpdateRender()override;
};

