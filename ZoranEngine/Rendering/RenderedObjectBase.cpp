#include "stdafx.h"
#include "RenderedObjectBase.h"
#include "RenderEngineBase.h"
#include "Core/ZoranEngine.h"

RenderedObjectBase::RenderedObjectBase(PrimitiveType pt, VertexType vt, DrawType dt) : primitiveType(pt), vertType(vt), drawType(dt)
{
}


RenderedObjectBase::~RenderedObjectBase()
{
	
}
