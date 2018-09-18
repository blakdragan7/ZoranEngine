#pragma once
#include "Rendering/RenderedObjectBase.h"

class OpenGLContext;
class ZoranEngine_EXPORT OpenGLRenderObject : public RenderedObjectBase
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

	OpenGLContext* OGLContext;

private:
	unsigned GLDrawTypeFromDrawType(DrawType type)const;
	unsigned GLVertexTypeFromVertexType(VertexType type)const;
	unsigned GLPrimitveFromPrimitiveType(PrimitiveType type)const;

public:
	OpenGLRenderObject(OpenGLContext* engine);
	~OpenGLRenderObject();

	virtual void UpdateObjectFromMemory(unsigned numVerts, unsigned offset,void* verts, void* uv, bool copy = true)override;
	virtual void CreateObjectFromMemory(PrimitiveType pType, VertexType vertType, DrawType drawType, unsigned numVerts, void* verts, void* uv, bool copy = true)override;
	virtual void RenderObject()override;

	void MakeFullScreenQuad()override;

	virtual void SetAlphaEnabled(bool enabled);

	// data is alloced
	virtual bool GetVertDataAsfloat(float** data, unsigned &amount)override;
};

