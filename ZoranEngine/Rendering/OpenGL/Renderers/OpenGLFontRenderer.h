#pragma once
#include <Rendering\Renderers\FontRenderer.h>

#include <list>

class OpenGLContext;
class FontResource;
class Matrix44;
class FontRender;
class OpenGLFontShader;

class OpenGLFontRenderer : public FontRenderer
{
private:
	OpenGLContext * context;
	std::list<FontRender*> fontsToRender;
	OpenGLFontShader* shader;

private:
	FontRender * FindFont(Font* font);

public:
	OpenGLFontRenderer(OpenGLContext * context);
	~OpenGLFontRenderer();

	virtual void RenderObject(const Matrix44& cameraMatrix)override;
	virtual void AddFontToRender(Font* font)override;
	virtual void RemoveFontFromRender(Font* font)override;
	virtual bool FontIsInRender(Font* inFont)override;
};

