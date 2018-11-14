#pragma once
#include <Rendering\Renderers\GUIColoredRectRenderer.h>

class OpenGLGUIColoredRectShader;
class OpenGLVertexGroup;
class OpenGLContext;
class OpenGLGUIColoredRectRenderer : public GUIColoredRectRenderer
{
private:
	OpenGLVertexGroup * vertGroup;
	OpenGLContext* context;
	OpenGLGUIColoredRectShader* shader;

public:
	OpenGLGUIColoredRectRenderer(OpenGLContext* context);
	~OpenGLGUIColoredRectRenderer();

	virtual void SetLinearBlend()override;
	virtual void RenderObject(const Matrix44& cameraMatrix)override;
};

