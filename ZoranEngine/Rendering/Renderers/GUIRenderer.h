#pragma once
#include <Rendering/RenderedObjectBase.h>

class ShaderProgramBase;
class GUIRenderer : public RenderedObjectBase
{
public:
	GUIRenderer();
	~GUIRenderer();

	virtual ShaderProgramBase* GetShaderProgram()const = 0;
};

