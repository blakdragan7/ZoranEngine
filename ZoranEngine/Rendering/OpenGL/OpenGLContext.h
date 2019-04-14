#pragma once
#include <Math/Vector2.h>
#include "Rendering/RenderTypes.h"

#include <Resources/FontResource.h>

class FrameBufferBase;
class TextureBase;
class OpenGLTexture;
class OpenGLLineRenderer;
class OpenGLLineLoopRenderer;
class OpenGLFontRenderer;
class OpenGLModelRenderer;
class OpenGLQuadRenderer;
class OpenGLQuadStripRenderer;
class OpenGLSpriteRenderer;
class OpenGLTriangleRenderer;
class OpenGLTriangleStripRenderer;
class OpenGLGUIRenderer;
class OpenGLGUIColoredRectRenderer;
class OpenGLContext
{
private:
	void* context;
	bool alphaEnabled;

public:
	OpenGLContext(WindowHandle handle);
	~OpenGLContext();

	void EnableDepthTesting();
	void DisableDepthTesting();
	void DisableAlpha();
	void EnableAlpha();
	void ClearBuffers();
	void Resize(int x, int y, int width, int height);

	OpenGLTexture* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat, int* error);
	OpenGLTexture* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size);

	bool CreateFrameBuffer(FrameBufferBase** outBuffer, ImageResource* outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size);

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
	OpenGLFontRenderer* CreateFontRenderer(FontResource font);
	OpenGLGUIRenderer* CreateGUIRenderer();
	OpenGLGUIColoredRectRenderer* CreateGUIColoredRectRenderer();
};
