#pragma once
#include <Rendering/RenderedObjectBase.h>

class ShaderProgramBase;
class ZoranEngine_EXPORT GUIRenderer : public RenderedObjectBase
{
public:
	GUIRenderer();
	~GUIRenderer();

	virtual ShaderProgramBase* GetShaderProgram()const = 0;
};

