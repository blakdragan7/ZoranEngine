#include "stdafx.h"
#include "ShaderProgramBase.h"
#include "DragEngine.h"

ShaderProgramBase::ShaderProgramBase()
{
	engine = dEngine->GetRenderer();
}


ShaderProgramBase::~ShaderProgramBase()
{
}
