#pragma once
#include "Core/PlatformTypes.h"
#include "Rendering/RenderTypes.h"
#include "Math/Vector2.h"
#include <unordered_map>

#include <Rendering/ShaderProgramBase.h>
/*
	RenderEngine, this is an abstract base class that allows common functionality across different render types such as OpenGL vs DirectX
*/

class SceneObject;
class TextureBase;
class FrameBufferBase;
class ShaderProgramBase;
class RenderedObjectBase;
class VisibleComponentBase;
class LineRenderer;
class LineLoopRenderer;
class ModelRenderer;
class QuadRenderer;
class QuadStripRenderer;
class SpriteRenderer;
class TriangleRenderer;
class TriangleStripRenderer;
class FontRenderer;
class GUIRenderer;
/* typedefs for dealing with shaders */
typedef std::unordered_map<const ShaderInitMap*, ShaderProgramBase*> ShaderMap;
typedef std::pair<const ShaderInitMap*, ShaderProgramBase*> ShaderMapPair;

class ZoranEngine_EXPORT RenderEngineBase
{
private:
	/* used to make creating shaders more efficient */
	ShaderMap * shaderMap;

public:
	RenderEngineBase();
	virtual ~RenderEngineBase();

	virtual void InitEngine(WindowHandle handle) = 0;
	/* Moves The Current NDC to this location winthin the window */
	virtual void SetViewport(int x, int y, int width, int height) = 0;
	/* Enables Checking the z Value for rendering and clipping */
	virtual void EnableDepthTesting() = 0;
	/* Disabled Checking the z value for rnedering and clipping */
	virtual void DisableDepthTesting() = 0;
	/* turns off translucency */
	virtual void DisableAlpha() = 0;
	/* turns on translucency */
	virtual void EnableAlpha() = 0;
	/* clear depth and color buffer */
	virtual void ClearBuffers() = 0;
	/* draw every registered component */
	virtual void DrawScene(const Matrix44& cameraMatrix) = 0;
	/* Draw All GUI Objects */
	/* All Gui Objects are agnostic to what owns them */
	//virtual void DrawAllGUIs(const Matrix44& viewportMatrix) = 0;
	/* Draw Debug GUI */
	virtual void DrawDebugGUI() = 0;
	/* resize viewport to screen */
	virtual void Resize(int w, int h) = 0;

	// GUI Handling

	//virtual void AddGUIToRender();
	//virtual void RemoveGUIFromRender();

	// Create Rendereres

	virtual LineRenderer* CreateLineRenderer() = 0;
	virtual LineLoopRenderer* CreateLineLoopRenderer() = 0;
	virtual ModelRenderer* CreateModelRenderer() = 0;
	virtual QuadRenderer* CreateQuadRenderer() = 0;
	virtual QuadStripRenderer* CreateQuadStripRenderer() = 0;
	virtual SpriteRenderer* CreateSpriteRenderer() = 0;
	virtual TriangleRenderer* CreateTriangleRenderer() = 0;
	virtual TriangleStripRenderer* CreateTriangleStripRenderer() = 0;
	virtual FontRenderer* CreateFontRenderer() = 0;
	virtual GUIRenderer* CreateGUIRenderer() = 0;

	// Creates Texture Memory on GPU and returns a texture object.
	virtual TextureBase* CreateTexture(const char* path, RenderDataType bufferType, RenderDataFormat bufferFormat) = 0;
	virtual TextureBase* CreateTexture(void* data, RenderDataType bufferType, RenderDataFormat bufferFormat, Vec2I size) = 0;
	// Create frame buffer for offscreen rendering and the texture that represents the result
	virtual bool CreateFrameBuffer(FrameBufferBase** outBuffer, TextureBase** outTexture, Vec2I size, RenderDataType bufferType = Render_Data_Type_RGBA_32, RenderDataFormat bufferFormat = Render_Data_Format_Unsigned_Byte) = 0;
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
	*  TODO possibly re visit this and come up with a "better" way of handling creating shaders
	*/

	template<typename t>
	ShaderProgramBase* CreateShaderProgram()
	{
		ShaderProgramBase* program = ShaderProgramForMap(t::initMap);
		if (program == NULL)
		{
			program = static_cast<ShaderProgramBase*>(new t(t::initMap));
			RegisterShaderProgram(program, t::initMap);
		}
		return program;
	}

	inline void RegisterShaderProgram(ShaderProgramBase* program,const ShaderInitMap* map)
	{
		shaderMap->insert({ map,program });
	}

	inline ShaderProgramBase* ShaderProgramForMap(const ShaderInitMap* map)
	{
		ShaderMap::iterator iter = shaderMap->find(map);
		if (iter != shaderMap->end())
		{
			return iter->second;
		}
		return 0;
	}

	virtual const char* GetEngineReadableName() { return "Unimplemented Render Engine"; }
};
