#pragma once
class OpenGLContext;
class OpenGLBuffer
{
private:
	OpenGLContext * context;
	unsigned bufferType;
	unsigned glBuffer;
	size_t bufferSize;
	unsigned usage;
	unsigned numComponents;

public:
	static OpenGLBuffer* BufferFromVertexVector(const std::vector<Vector3D>& vector, OpenGLContext* context);

	OpenGLBuffer(OpenGLContext * context);
	OpenGLBuffer(unsigned bufferType, void* data, unsigned numComponents, unsigned usage, size_t size, OpenGLContext * context);
	~OpenGLBuffer();

	void BindBuffer();
	void UpdateBuffer(void* data, size_t offset, size_t size);
	void ReCreateBuiffer(void* data,size_t size, unsigned numComponents,unsigned usage);
	void ClearBuffer();
	void ClearBuffer(size_t offset, size_t size);
	inline unsigned GetNumComponents()const { return numComponents; }
	inline size_t GetBufferSize()const { return bufferSize; }
};

