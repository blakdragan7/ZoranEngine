#pragma once
#include <Rendering\FrameBufferBase.h>

class OpenGLContext;
class ZoranEngine_EXPORT OpenGLFrameBuffer : public FrameBufferBase
{
private:
	OpenGLContext * context;
	unsigned	frameBuffer;
	unsigned	renderBuffer;
	unsigned	pixelBuffer;
	Vector2I	size;
private:
	void SetupOpenGL();

public:
	OpenGLFrameBuffer(Vector2I renderSize, RenderDataType renderType, RenderDataFormat format, OpenGLContext * context);
	OpenGLFrameBuffer(TextureBase* targetTexture, OpenGLContext * context);
	~OpenGLFrameBuffer();

	virtual void RenderObject(const Matrix44& cameraMatrix)override;
};

