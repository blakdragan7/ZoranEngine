#include "stdafx.h"
#include "ShaderProgramBase.h"
#include "Core/ZoranEngine.h"

ShaderProgramBase::ShaderProgramBase()
{
	engine = zEngine->GetRenderer();
}


ShaderProgramBase::~ShaderProgramBase()
{
}
