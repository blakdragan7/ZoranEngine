#pragma once
#include "VertexBufferBase.h"
class DRAGENGINE_EXPORT OpenGLVertexBuffer : public VertexBufferBase
{
private:
	unsigned vbo;
	unsigned vao;
	unsigned glDrawType;
	unsigned glVertType;

private:
	unsigned GLDrawTypeFromDrawType(DrawType type);
	unsigned GLVertexTypeFromVertexType(VertexType type);

public:
	OpenGLVertexBuffer();
	~OpenGLVertexBuffer();

	virtual void CreateBufferFromMemory(VertexType vertType, DrawType drawType, unsigned numVerts, void* verts)override;
	virtual void BindBuffer()override;
	virtual void UnbindBuffer()override;
	virtual void DrawArrays()override;
};

