#pragma once
#include <Rendering\Renderers\IndexedTriangleRenderer.h>
class OpenGLVertexGroup;
class OpenGLContext;
class OpenGLIndexedTriangleRenderer : public IndexedTriangleRenderer
{
private:
	OpenGLContext * context;
	OpenGLVertexGroup* vertexGroup;

public:
	OpenGLIndexedTriangleRenderer(OpenGLContext* context);
	~OpenGLIndexedTriangleRenderer();

	virtual void RenderObject(const Matrix44& cameraMatrix)override;

	virtual void MakeFullScreenQuad()override;
	virtual void SetTriangles(const std::vector<TrianglePrimitive>& rectangles)override;
	virtual void SetIndecies(std::vector<unsigned>& indecies)override;
};

