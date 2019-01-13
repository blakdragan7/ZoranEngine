#pragma once
#include<Rendering\Renderers\QuadStripRenderer.h>
class OpenGLVertexGroup;
class OpenGLContext;
class OpenGLQuadStripRenderer : public QuadStripRenderer
{
private:
	OpenGLContext * context;
	OpenGLVertexGroup* vertexGroup;

public:
	OpenGLQuadStripRenderer(OpenGLContext* context);
	~OpenGLQuadStripRenderer();
};

