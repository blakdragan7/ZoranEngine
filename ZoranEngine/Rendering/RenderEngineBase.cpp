#include "stdafx.h"
#include "RenderEngineBase.h"


RenderEngineBase::RenderEngineBase()
{
	shaderMap = new ShaderMap();
}


RenderEngineBase::~RenderEngineBase()
{
	for (auto& iter : *shaderMap)
	{
		delete iter.second;
	}

	delete shaderMap;
}
