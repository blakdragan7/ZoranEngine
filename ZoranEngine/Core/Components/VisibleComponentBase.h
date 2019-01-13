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
class Matrix44;
class ZoranEngine_EXPORT VisibleComponentBase
{
protected:
	RenderedObjectBase * renderedObject;
	RenderEngineBase* engine;
	ShaderProgramBase * program;

public:
	VisibleComponentBase(RenderEngineBase* engine = 0);
	virtual ~VisibleComponentBase();

	virtual void PreRender() = 0;
	virtual void Render(const Matrix44& cameraMatrix) = 0;
	virtual void PostRender() = 0;

	virtual void SetShaderProgram(ShaderProgramBase* program) = 0;

	/* Allows render engine to have access to the rendered object */
	inline RenderedObjectBase* GetRenderedObject()const { return renderedObject; }
	inline ShaderProgramBase* GetShaderProgram()const { return program; }
};
