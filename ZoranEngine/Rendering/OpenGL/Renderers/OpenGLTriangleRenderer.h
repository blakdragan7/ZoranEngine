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

	virtual void RenderObject(const Matrix44& cameraMatrix)override;

	virtual void MakeFullScreenQuad()override;
	virtual void AddTriangles(const std::vector<TrianglePrimitive>& rectangles)override;
};

