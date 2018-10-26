#include "stdafx.h"
#include "OpenGLLineLoopRenderer.h"
#include <Rendering/OpenGL/OpenGLVertexGroup.h>
#include <Rendering/OpenGL/OpenGLBuffer.h>
#include <Rendering/OpenGL/OpenGLContext.h>
#include <GL/glew.h>

OpenGLLineLoopRenderer::OpenGLLineLoopRenderer(OpenGLContext * context) : context(context), isAddingSegments(false), LineLoopRenderer(DT_Dynamic)
{
	vertexGroup = new OpenGLVertexGroup(GL_LINE_LOOP, context);
}

OpenGLLineLoopRenderer::~OpenGLLineLoopRenderer()
{
	delete vertexGroup;
}

void OpenGLLineLoopRenderer::BeginLineAddingSegments()
{
	isAddingSegments = true;
	segments.clear();
}

void OpenGLLineLoopRenderer::EndAddingSegments()
{
	isAddingSegments = false;

	size_t size = segments.size() * 2 * 3;

	float* data = new float[size];

	for (size_t i = 0; i < segments.size(); i++)
	{
		LineSegment& segment = segments[i];

		size_t index = i * 2 * 3;

		data[index + 0] = segment.vertecies[0].x;
		data[index + 1] = segment.vertecies[0].y;
		data[index + 2] = segment.vertecies[0].z;

		data[index + 3] = segment.vertecies[1].x;
		data[index + 4] = segment.vertecies[1].y;
		data[index + 5] = segment.vertecies[1].z;
	}

	OpenGLBuffer * buf = vertexGroup->GetBufferForAttr(0);

	if (buf == 0)
	{
		buf = new OpenGLBuffer(GL_ARRAY_BUFFER, data, 3, GL_DYNAMIC_DRAW, size * sizeof(float), context);
	
		vertexGroup->AddBufferForAttr(0, buf);
	}
	else
	{
		size_t floatSize = segments.size() * 2 * 3 * sizeof(float);

		if (floatSize < buf->GetBufferSize())
		{

		}
	}

	delete[] data;
}

void OpenGLLineLoopRenderer::AddSegment(const LineSegment & segment)
{
	segments.push_back(segment);
}

void OpenGLLineLoopRenderer::SetSegments(const std::vector<LineSegment>& segments)
{
	this->segments = segments;
	EndAddingSegments();
}

void OpenGLLineLoopRenderer::SetLineWidth(float width)
{
	context->SetLineWidth(width);
}

const std::vector<LineSegment>& OpenGLLineLoopRenderer::GetLineSegments() const
{
	return segments;
}
