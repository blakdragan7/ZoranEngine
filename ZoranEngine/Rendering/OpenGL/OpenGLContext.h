#pragma once
#include <Math/Vector2.h>
#include "Rendering/RenderTypes.h"

class FrameBufferBase;
class TextureBase;
class OpenGLTexture;
class OpenGLLineRenderer;
class OpenGLLineLoopRenderer;
class OpenGLModelRenderer;
class OpenGLQuadRenderer;
class OpenGLQuadStripRenderer;
class OpenGLSpriteRenderer;
class OpenGLTriangleRenderer;
class OpenGLTriangleStripRenderer;
class OpenGLContext
{
private:
	void* context;

public:
	OpenGLContext(WindowHandle handle);
	~OpenGLContext();

	void EnableDepthTesting();
	void DisableDepthTesting();
	void DisableAlpha();
	void EnableAlpha();
	void ClearBuffers();
	void Resize(int x, int y);

	OpenGLTexture* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat);
	OpenGLTexture* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size);

	bool CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size);

	void SetLineWidth(float width);

	bool CheckErrors(const char* text);
	void ClearErrors();

	OpenGLLineRenderer* CreateLineRenderer();
	OpenGLLineLoopRenderer* CreateLineLoopRenderer();
	OpenGLModelRenderer* CreateModelRenderer();
	OpenGLQuadRenderer* CreateQuadRenderer();
	OpenGLQuadStripRenderer* CreateQuadStripRenderer();
	OpenGLSpriteRenderer* CreateSpriteRenderer();
	OpenGLTriangleRenderer* CreateTriangleRenderer();
	OpenGLTriangleStripRenderer* CreateTriangleStripRenderer();
};