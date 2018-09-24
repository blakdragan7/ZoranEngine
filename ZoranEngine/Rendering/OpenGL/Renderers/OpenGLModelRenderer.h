#pragma once
#include <Rendering\Renderers\ModelRenderer.h>
class OpenGLVertexGroup;
class OpenGLContext;
class OpenGLModelRenderer : public ModelRenderer
{
private:
	OpenGLContext * context;

public:
	OpenGLModelRenderer(OpenGLContext* context);
	~OpenGLModelRenderer();
};

