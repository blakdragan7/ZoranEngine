#pragma once
#include "Core/PlatformTypes.h"
#include "Rendering/RenderTypes.h"
#include "Math/Vector2.h"
#include <unordered_map>
/*
	RenderEngine, this is an abstract base class that allows common functionality across different render types such as OpenGL vs DirectX
*/

class SceneObject;
class TextureBase;
class FrameBufferBase;
class ShaderProgramBase;
class RenderedObjectBase;
class VisibleComponentBase;

/* typedefs for dealing with shaders */
typedef std::unordered_map<unsigned, const char*> ShaderInitMap;
typedef std::unordered_map<ShaderInitMap, ShaderProgramBase*> ShaderMap;
typedef std::pair<ShaderInitMap, ShaderProgramBase*> ShaderMapPair;

class ZoranEngine_EXPORT RenderEngineBase
{
private:
	/* used to make creating shaders more efficient */
	ShaderMap * shaderMap;

public:
	RenderEngineBase();
	virtual ~RenderEngineBase();

	virtual void InitEngine(WindowHandle handle) = 0;

	/* turns off translucency */
	virtual void DisableAlpha() = 0;
	/* turns on translucency */
	virtual void EnableAlpha() = 0;
	/* clear depth and color buffer */
	virtual void ClearBuffers() = 0;
	/* draw every registered component */
	virtual void DrawAll() = 0;
	/* resize viewport to screen */
	virtual void Resize(int w, int h) = 0;

	/* Each implementation must implement way of adding Render Objects 
	*  ex. DirectX may have a different render loop style then OpenGL */
	virtual void AddComponent(VisibleComponentBase* component) = 0;
	virtual bool RemoveComponent(VisibleComponentBase* component) = 0;
	// Creates Texture Memory on GPU and returns a texture object.
	virtual TextureBase* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat) = 0;
	virtual TextureBase* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size) = 0;
	virtual RenderedObjectBase* CreateRenderedObject() = 0;
	// Create frame buffer for offscreen rendering and the texture that represents the result
	virtual bool CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size) = 0;
	/* Sets the line width for drawing raw lines like gl_line_loop or similiar */
	virtual void SetLineWidth(float width) = 0;
	/* prints errors found within the render engine pre appending text to the message */
	virtual void CheckErrors(const char* text) = 0;
	virtual void ClearErrors() = 0;

	/* 
	*  This is the only way i could think of dynamically creating shaders since each shader has individual functionality.
	*  I don't want to leave it up to the user because it's not effecient to create the shader multiple times so there must be
	*  a map of some sort tracking the shader. 
	*
	*  To Do possibly re visit this and come up with a "better" way of handling creating shaders
	*/

	template<typename t>
	ShaderProgramBase* CreateShaderProgram(ShaderInitMap map)
	{
		t* program = ShaderProgramForMap(map);
		if (program = NULL)
		{
			program = new t(map);
			RegisterShaderProgram(program, map);
		}
		return program;
	}

	inline void RegisterShaderProgram(ShaderProgramBase* program, ShaderInitMap map)
	{
		shaderMap->insert({ map,program });
	}

	inline ShaderProgramBase* ShaderProgramForMap(ShaderInitMap map)
	{
		ShaderMap::iterator iter = shaderMap->find(map);
		if (iter != shaderMap->end())
		{
			return iter->second;
		}
		return 0;
	}
};
