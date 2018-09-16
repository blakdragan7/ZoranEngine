#pragma once
#include <OpenGL\OpenGLRenderEngine.h>

typedef std::map  <ShaderProgramBase*, std::vector<VisibleComponentBase*>> GL2DRenderMap;
typedef std::pair <ShaderProgramBase*, std::vector<VisibleComponentBase*>> GL2DRenderMapPair;
typedef std::vector<GL2DRenderMap*> GL2DRenderLayers;
class OpenGL2DRenderEngine : public OpenGLRenderEngine
{
private:
	/* Used to render opengl object from back to front, z sorted */
	GL2DRenderLayers * renderLayers;
	/* used to store components that need to be inserted into render layers*/
	GL2DRenderMap* renderMap;

public:
	OpenGL2DRenderEngine();
	~OpenGL2DRenderEngine();

	virtual void DrawAll()override;

	virtual void AddComponent(VisibleComponentBase* component)override;
	virtual bool RemoveComponent(VisibleComponentBase* component)override;
	virtual const char* GetEngineReadableName() { return "OpenGL 2D RenderEngine"; };
};
