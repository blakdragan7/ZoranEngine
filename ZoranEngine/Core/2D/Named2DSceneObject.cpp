#include "stdafx.h"
#include "Named2DSceneObject.h"

#include <Core/2D/Components/Component2DBase.h>

Named2DSceneObject::Named2DSceneObject(std::string name) : SceneObject2D(name)
{
	Component2DBase* component = new Component2DBase();
	root2DComponent = component;
	SetRootComponent(root2DComponent);
}


Named2DSceneObject::~Named2DSceneObject()
{
}
