#pragma once
#include "RenderedObjectBase.h"
class DRAGENGINE_EXPORT OpenGLObject : public RenderedObjectBase
{
private:
	static unsigned const vertLocation = 0;
	static unsigned const UVLocation = 1;

	unsigned	vbo;
	unsigned	tbo;
	unsigned	vao;

	unsigned	glDrawType;
	unsigned	glVertType;
	unsigned	glBufferDrawType;

private:
	unsigned GLDrawTypeFromDrawType(DrawType type);
	unsigned GLVertexTypeFromVertexType(VertexType type);

public:
	OpenGLObject();
	~OpenGLObject();

	virtual void CreateObjectFromMemory(VertexType vertType, DrawType drawType, unsigned numVerts, void* verts, void* uv, bool copy = true)override;
	virtual void RenderObject()override;
};

