#include "stdafx.h"
#include "DebugSceneObject2D.h"
#include <OpenGL/2D/DebugShader2D.h>

DebugSceneObject2D::DebugSceneObject2D(std::string name) : SceneObject2D(name)
{
	component = new DebugRenderComponent();
	root2DComponent = component;
	SetRootComponent(root2DComponent);
}


DebugSceneObject2D::~DebugSceneObject2D()
{
}
