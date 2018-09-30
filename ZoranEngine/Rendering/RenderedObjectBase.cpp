#include "stdafx.h"
#include "RenderedObjectBase.h"
#include "RenderEngineBase.h"
#include "Core/ZoranEngine.h"

RenderedObjectBase::RenderedObjectBase() : primitiveType(PT_Unused), vertType(VT_Unused), drawType(DT_Unused)
{
}

RenderedObjectBase::RenderedObjectBase(PrimitiveType pt, VertexType vt, DrawType dt) : primitiveType(pt), vertType(vt), drawType(dt)
{
}


RenderedObjectBase::~RenderedObjectBase()
{
	
}
