#pragma once
#include <Rendering\Renderers\FontRenderer.h>

class OpenGLContext;
class OpenGLFontRenderer : public FontRenderer
{
private:
	OpenGLContext * context;
public:
	OpenGLFontRenderer(OpenGLContext * context);
	~OpenGLFontRenderer();
};

