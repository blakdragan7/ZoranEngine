#pragma once
#include <Rendering\Renderers\TriangleStripRenderer.h>
class OpenGLVertexGroup;
class OpenGLContext;
class OpenGLTriangleStripRenderer : public TriangleStripRenderer
{
private:
	OpenGLContext * context;
	OpenGLVertexGroup* vertexGroup;
	std::vector<Vector3D> triangleBuilder;

public:
	OpenGLTriangleStripRenderer(OpenGLContext* context);
	~OpenGLTriangleStripRenderer();

	void MakeFullScreenQuad()override;
	virtual void RenderObject()override;

	virtual void BeginTriangles(Vector3D ll, Vector3D tc, Vector3D lr)override;
	virtual void AddTriangleToStrip(Vector3D nextVertex)override;
	virtual void EndTriangles()override;
};

