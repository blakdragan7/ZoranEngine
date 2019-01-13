#pragma once

class OpenGLBuffer;
class OpenGLContext;
class OpenGLVertexArray
{
private:
	OpenGLContext * context;
	unsigned vao;

	static unsigned EnableVAO;

public:
	OpenGLVertexArray(OpenGLContext * context);
	~OpenGLVertexArray();

	void BindBufferForAttr(unsigned attr, OpenGLBuffer* buffer);
	void EnableArray();
};

