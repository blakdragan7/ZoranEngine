#pragma once
#include <Rendering\Renderers\GUIRenderer.h>

class OpenGLContext;
class OpenGLGUIShader;
class OpenGLTriangleStripRenderer;
class OpenGLGUIRenderer : public GUIRenderer
{
private:
	OpenGLContext * context;
	OpenGLGUIShader* shader;
	OpenGLTriangleStripRenderer* internalRenderer;

public:
	OpenGLGUIRenderer(OpenGLContext * context);
	~OpenGLGUIRenderer();

	virtual void RenderObject(const Matrix44& cameraMatrix)override;
};

