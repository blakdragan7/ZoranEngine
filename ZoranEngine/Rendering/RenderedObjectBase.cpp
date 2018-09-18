#include "stdafx.h"
#include "RenderedObjectBase.h"
#include "RenderEngineBase.h"
#include "Core/ZoranEngine.h"

RenderedObjectBase::RenderedObjectBase(unsigned type) : cpuVertData(0), cpuUVData(0), renderObjectType(type)
{
}


RenderedObjectBase::~RenderedObjectBase()
{
	
}
