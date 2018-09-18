#pragma once
/*
*	Any component derived from this can be rendered.
*	Any component derived from this must also be derived from component base or one of it's sub classes.
*   This does not derive from component base in order to prevent from deriving from component base more then once and
*   to provide a way to make a component either visible or not visible without re writing the coordanite system coode
*/
class RenderedObjectBase;
class RenderEngineBase;
class ShaderProgramBase;
class ZoranEngine_EXPORT VisibleComponentBase
{
private:
	ShaderProgramBase * program;
	/*
	*   renderLayer is used differently for 2d vs 3d. 
	*	for 2D it determins the background position of the render. 
	*	For example a backdrop might be 0 while the sprite level might be 4.
	*	TODO: determine 3D use of render layer
	*/
	unsigned renderLayer;

protected:
	RenderedObjectBase * renderedObject;
	RenderEngineBase * engine; // cache of current render engine

public:
	VisibleComponentBase(unsigned renderLayer, RenderEngineBase* engine = 0);
	virtual ~VisibleComponentBase();

	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

	void SetShaderProgram(ShaderProgramBase* program);

	/* Allows render engine to have access to the rendered object */
	inline RenderedObjectBase* GetRenderedObject()const { return renderedObject; }
	inline ShaderProgramBase* GetShaderProgram()const { return program; }

	inline unsigned GetRenderLayer() { return renderLayer; }
	void SetRenderLayer(unsigned renderLayer);
};
