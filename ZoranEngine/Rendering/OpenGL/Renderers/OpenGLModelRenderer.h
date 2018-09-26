#pragma once
#include <Rendering\Renderers\ModelRenderer.h>
class OpenGLVertexGroup;
class OpenGLContext;
class OpenGLModelRenderer : public ModelRenderer
{
private:
	OpenGLContext * context;
	OpenGLVertexGroup* vertexGroup;

protected:
	virtual void NewModelLoaded()override;

public:
	OpenGLModelRenderer(OpenGLContext* context);
	~OpenGLModelRenderer();

	virtual void RenderObject()override;
};

