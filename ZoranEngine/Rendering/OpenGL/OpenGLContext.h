#pragma once
#include <Math/Vector2.h>
#include "Rendering/RenderTypes.h"

class FrameBufferBase;
class OpenGLTexture;
class RenderedObjectBase;
class ZoranEngine_EXPORT OpenGLContext
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

 	RenderedObjectBase* CreateRenderedObject();
	OpenGLTexture* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat);
	OpenGLTexture* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size);

	bool CreateFrameBuffer(FrameBufferBase** outBuffer, OpenGLTexture** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size);

	void SetLineWidth(float width);

	void CheckErrors(const char* text);
	void ClearErrors();
};
