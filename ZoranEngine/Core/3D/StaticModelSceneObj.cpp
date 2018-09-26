#include "stdafx.h"
#include "StaticModelSceneObj.h"
#include <Core/3D/Components/StaticObjComponent.h>

StaticModelSceneObj::StaticModelSceneObj(std::string name) : SceneObject3D(name)
{
	model = new StaticObjComponent();

	root3DComponent = model;
	SetRootComponent(root3DComponent);
}

StaticModelSceneObj::~StaticModelSceneObj()
{
}

void StaticModelSceneObj::SetMesh(const char * meshPath)
{
	model->LoadFile(meshPath);
}
