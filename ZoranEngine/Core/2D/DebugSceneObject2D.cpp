#include "stdafx.h"
#include "DebugSceneObject2D.h"
#include <OpenGL/2D/DebugShader2D.h>

DebugSceneObject2D::DebugSceneObject2D(std::string name) : SceneObject2D(name)
{
	component = new DebugRenderComponent();
	root2DComponent = component;
	SetRootComponent(root2DComponent);

	component->SetIsVisible(false);
	component->SetColor({ 0,0,1 });
}


DebugSceneObject2D::~DebugSceneObject2D()
{
}
