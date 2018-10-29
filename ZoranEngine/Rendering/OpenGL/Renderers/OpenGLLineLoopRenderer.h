#pragma once
#include <Rendering\Renderers\LineLoopRenderer.h>
class OpenGLContext;
class OpenGLVertexGroup;
class OpenGLShaderProgramBase;
class OpenGLLineLoopRenderer : public LineLoopRenderer
{
private:
	OpenGLContext * context;
	OpenGLVertexGroup* vertexGroup;
	std::vector<LineSegment> segments;

	bool isAddingSegments;
	OpenGLShaderProgramBase* shader;

public:
	OpenGLLineLoopRenderer(OpenGLContext * context);
	~OpenGLLineLoopRenderer();

	virtual void BeginAddingSegments()override;
	virtual void EndAddingSegments()override;
	virtual void AddSegment(const LineSegment& segment)override;
	virtual void SetSegments(const std::vector<LineSegment>& segments)override;
	virtual void SetLineWidth(float width)override;

	virtual const std::vector<LineSegment>& GetLineSegments()const override;

	virtual void RenderObject(const Matrix44& cameraMatrix)override;
};

