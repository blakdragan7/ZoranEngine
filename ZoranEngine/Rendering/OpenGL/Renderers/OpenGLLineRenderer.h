#pragma once
#include <Rendering\Renderers\LineRenderer.h>
class OpenGLVertexGroup;
class OpenGLContext;
class OpenGLLineRenderer : public LineRenderer
{
private:
	OpenGLContext * context;
	OpenGLVertexGroup* vertexGroup;

public:
	OpenGLLineRenderer(OpenGLContext* context);
	~OpenGLLineRenderer();
};

