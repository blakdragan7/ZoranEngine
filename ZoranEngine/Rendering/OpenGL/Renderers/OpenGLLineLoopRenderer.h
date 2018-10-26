#pragma once
#include <Rendering\Renderers\LineLoopRenderer.h>
class OpenGLContext;
class OpenGLVertexGroup;
class OpenGLLineLoopRenderer : public LineLoopRenderer
{
private:
	OpenGLContext * context;
	OpenGLVertexGroup* vertexGroup;
	std::vector<LineSegment> segments;

	bool isAddingSegments;

public:
	OpenGLLineLoopRenderer(OpenGLContext * context);
	~OpenGLLineLoopRenderer();

	virtual void BeginLineAddingSegments()override;
	virtual void EndAddingSegments()override;
	virtual void AddSegment(const LineSegment& segment)override;
	virtual void SetSegments(const std::vector<LineSegment>& segments)override;
	virtual void SetLineWidth(float width)override;

	virtual const std::vector<LineSegment>& GetLineSegments()const override;
};

