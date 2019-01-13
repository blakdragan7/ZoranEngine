#pragma once
#include <Rendering\RenderedObjectBase.h>
#include <Rendering\Primitives.h>

class ZoranEngine_EXPORT LineLoopRenderer : public RenderedObjectBase
{
public:
	LineLoopRenderer(DrawType dt);
	~LineLoopRenderer();

	virtual void BeginAddingSegments() = 0;
	virtual void EndAddingSegments() = 0;
	virtual void AddSegment(const LineSegment& segment) = 0;
	virtual void SetSegments(const std::vector<LineSegment>& segments) = 0;

	virtual void SetLineWidth(float width) = 0;

	virtual const std::vector<LineSegment>& GetLineSegments()const = 0;
	
};

