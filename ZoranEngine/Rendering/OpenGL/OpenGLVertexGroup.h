#pragma once
#include <ThirdParty/sparsehash/dense_hash_map>

class OpenGLBuffer;
class OpenGLContext;
class OpenGLVertexArray;

class OpenGLVertexGroup
{
private:
	google::dense_hash_map<unsigned, OpenGLBuffer*> bufferMap;
	OpenGLVertexArray*	vao;
	unsigned vertexDrawType;
	unsigned numVerts;
	bool hasIndecies;

	OpenGLContext* OGLContext;
	OpenGLBuffer* indexBuffer;

public:
	OpenGLVertexGroup(unsigned vertexDrawType, OpenGLContext* context);
	OpenGLVertexGroup(unsigned vertexDrawType, unsigned numVertecies, OpenGLContext* context);
	~OpenGLVertexGroup();

	inline void SetNumVerts(unsigned numVerts) { this->numVerts = numVerts; }
	inline unsigned GetNumVerts() { return numVerts; }

	void RenderObject();
	void AddBufferForAttr(unsigned attr, unsigned type, void * data, unsigned numComponents, size_t size, unsigned usage);
	void AddBufferForAttr(unsigned attr, OpenGLBuffer * buffer);
	void AddBufferForAttrNoCheck(unsigned attr, OpenGLBuffer * buffer);
	void SetElementBuffer(OpenGLBuffer* buffer);

	OpenGLBuffer* GetBufferForAttr(unsigned attr)const;

};

