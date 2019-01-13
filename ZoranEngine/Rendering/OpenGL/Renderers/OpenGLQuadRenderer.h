#pragma once
#include <Rendering\Renderers\QuadRenderer.h>
class OpenGLVertexGroup;
class OpenGLContext;
class OpenGLQuadRenderer : public QuadRenderer
{
private:
	OpenGLContext* context;
	OpenGLVertexGroup * vertexGroup;

public:
	OpenGLQuadRenderer(OpenGLContext* context);
	~OpenGLQuadRenderer();

};

