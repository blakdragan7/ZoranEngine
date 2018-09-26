#include "stdafx.h"
#include "Named3DSceneObject.h"


Named3DSceneObject::Named3DSceneObject(std::string name) : SceneObject3D(name)
{
	root3DComponent = new Component3DBase();
	SetRootComponent(root3DComponent);
}


Named3DSceneObject::~Named3DSceneObject()
{
}
