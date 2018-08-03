#include "stdafx.h"
#include "RenderedObjectBase.h"
#include "RenderEngineBase.h"
#include "Core/ZoranEngine.h"

RenderedObjectBase::RenderedObjectBase()
{
	cpuVertData = 0;
	cpuUVData = 0;

	renderEngine = zEngine->GetRenderer();
}


RenderedObjectBase::~RenderedObjectBase()
{
	
}
