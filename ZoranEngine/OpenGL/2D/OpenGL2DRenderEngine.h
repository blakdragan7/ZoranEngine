#pragma once
#include <OpenGL\OpenGLRenderEngine.h>

#include <unordered_map>
#include <map>
#include <vector>

typedef std::unordered_map  <ShaderProgramBase*, std::vector<VisibleComponentBase*>> GL2DRenderMap;
typedef std::pair <ShaderProgramBase*, std::vector<VisibleComponentBase*>> GL2DRenderMapPair;
typedef std::map<unsigned,GL2DRenderMap*> GL2DRenderLayers;
typedef std::vector<VisibleComponentBase*> VComponentArray;
class OpenGL2DRenderEngine : public OpenGLRenderEngine
{
private:
	/* Used to render opengl object from back to front, z sorted */
	GL2DRenderLayers * renderLayers;
	/* used to store components that need to be inserted into render layers*/
	VComponentArray* componentsToSort;

public:
	OpenGL2DRenderEngine();
	~OpenGL2DRenderEngine();

	virtual void DrawAll()override;

	virtual void AddComponent(VisibleComponentBase* component)override;
	virtual bool RemoveComponent(VisibleComponentBase* component)override;
	virtual const char* GetEngineReadableName() { return "OpenGL 2D RenderEngine"; };
};
