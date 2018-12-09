#pragma once
#include <Rendering\Renderers\FontRenderer.h>
#include <ThirdParty/sparsehash/dense_hash_map>
#include <list>

class OpenGLContext;
class FontResource;
class Matrix44;
class OpenGLShaderProgramBase;
class OpenGLTriangleRenderer;
class OpenGLIndexedTriangleRenderer;
class OpenGLFontRenderer : public FontRenderer
{
private:

	struct GlyphIndex{
		size_t indecies[6];
		float normSize;
	};

	OpenGLContext * context;
	OpenGLShaderProgramBase* shader;
	OpenGLTriangleRenderer* renderer;

	google::dense_hash_map<uint32_t, GlyphIndex> indecieMap;

private:
	void SetupTriangles();

public:
	OpenGLFontRenderer(FontResource* font, OpenGLContext * context);
	~OpenGLFontRenderer();

	virtual void RenderObject(const Matrix44& cameraMatrix)override;
	virtual void UpdateRender()override;
};

