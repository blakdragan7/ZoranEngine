#include "stdafx.h"
#include "DebugSceneObject2D.h"
#include <OpenGL/DebugShader2D.h>

DebugSceneObject2D::DebugSceneObject2D(std::string name) : SceneObject2D(name)
{
	SetShaderProgram(new DebugShader2D(this));
}


DebugSceneObject2D::~DebugSceneObject2D()
{
}
