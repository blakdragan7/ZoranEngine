#include "stdafx.h"
#include "PhysicsObjectBase.h"
#include "SceneObject.h"

#include <iostream>

PhysicsObjectBase::PhysicsObjectBase(SceneObject* object)
{
	sceneObject = object;
	physicsLayer = -1;
}

PhysicsObjectBase::~PhysicsObjectBase()
{
}

void PhysicsObjectBase::SetSceneObject(SceneObject * object)
{
	sceneObject = object;
}

unsigned PhysicsObjectBase::GetPhysicsLayer()
{
	return physicsLayer;
}

void PhysicsObjectBase::SetPhysicsLayer(unsigned layer)
{
	physicsLayer = layer;
}

Vector3D PhysicsObjectBase::GetScenePos()
{
	if(sceneObject != NULL) return sceneObject->GetPosition();
	else
	{
		std::cerr << "Getting Position Before SceneObject Set !\n";
		return Vector3D(0,0,0);
	}
}

SceneObject* PhysicsObjectBase::GetSceneObject()
{
	return sceneObject;
}
