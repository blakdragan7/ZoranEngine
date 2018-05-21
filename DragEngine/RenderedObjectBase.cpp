#include "stdafx.h"
#include "RenderedObjectBase.h"
#include "RenderEngineBase.h"
#include "DragEngine.h"

RenderedObjectBase::RenderedObjectBase()
{
	cpuVertData = 0;
	cpuUVData = 0;

	renderEngine = dEngine->GetRenderer();
}


RenderedObjectBase::~RenderedObjectBase()
{
	
}
