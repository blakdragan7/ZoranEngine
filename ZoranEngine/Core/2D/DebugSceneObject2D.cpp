#include "stdafx.h"
#include "DebugSceneObject2D.h"
#include <OpenGL/DebugShader2D.h>

DebugSceneObject2D::DebugSceneObject2D(std::string name) : SceneObject2D(name)
{
	static DebugShader2D* dbg = new DebugShader2D(this);
	SetShaderProgram(dbg);
}


DebugSceneObject2D::~DebugSceneObject2D()
{
}
