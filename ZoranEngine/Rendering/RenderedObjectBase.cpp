#include "stdafx.h"
#include "RenderedObjectBase.h"
#include "RenderEngineBase.h"
#include "Core/ZoranEngine.h"

RenderedObjectBase::RenderedObjectBase(RenderEngineBase * engine, unsigned type) : renderEngine(engine), renderObjectType(type)
{
}

RenderedObjectBase::RenderedObjectBase(RenderEngineBase* engine,unsigned type) : renderEngine(engine), renderObjectType(type)
{
	cpuVertData = 0;
	cpuUVData = 0;
}


RenderedObjectBase::~RenderedObjectBase()
{
	
}
