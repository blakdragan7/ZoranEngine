#pragma once
#include <Rendering\Renderers\TriangleRenderer.h>
class OpenGLVertexGroup;
class OpenGLContext;
class OpenGLTriangleRenderer : public TriangleRenderer
{
private:
	OpenGLContext * context;
	OpenGLVertexGroup* vertexGroup;

public:
	OpenGLTriangleRenderer(OpenGLContext* context);
	~OpenGLTriangleRenderer();
};

