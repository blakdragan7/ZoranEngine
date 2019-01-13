#pragma once
#include <Rendering\Renderers\SpriteRenderer.h>
class OpenGLTriangleStripRenderer;
class OpenGLContext;
class OpenGLSpriteRenderer : public SpriteRenderer
{
private:
	OpenGLContext * context;
	OpenGLTriangleStripRenderer* innerRenderer;

public:
	OpenGLSpriteRenderer(OpenGLContext* context);
	~OpenGLSpriteRenderer();

	virtual void RenderObject(const Matrix44& cameraMatrix)override;
};

