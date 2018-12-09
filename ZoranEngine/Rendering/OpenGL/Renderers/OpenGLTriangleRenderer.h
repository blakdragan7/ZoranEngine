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
	virtual void AddTriangles(const std::vector<float>& verts, std::vector<float>& uvs)override;
	virtual void AddTriangles(float* verts, size_t vertSize, float* uvs, size_t uvSize)override;
};

