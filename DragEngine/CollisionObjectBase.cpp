#include "stdafx.h"
#include "CollisionObjectBase.h"
#include "SceneObject.h"

#include <iostream>

CollisionObjectBase::CollisionObjectBase(SceneObject* object, unsigned collisionType)
{
	sceneObject = object;
	collisionLayer = -1;
	this->collisionType = collisionType;
}

CollisionObjectBase::~CollisionObjectBase()
{
}

void CollisionObjectBase::SetSceneObject(SceneObject * object)
{
	sceneObject = object;
}

void CollisionObjectBase::SetPhysicsObject(PhysicsObjectBase * object)
{
	physicsObject = object;
}

unsigned CollisionObjectBase::GetCollisionLayer()
{
	return collisionLayer;
}

void CollisionObjectBase::SetCollisionLayer(unsigned layer)
{
	collisionLayer = layer;
}

Vector3D CollisionObjectBase::GetScenePos()
{
	if(sceneObject != NULL) return sceneObject->GetPosition();
	else
	{
		std::cerr << "Getting Position Before SceneObject Set !\n";
		return Vector3D(0,0,0);
	}
}

SceneObject* CollisionObjectBase::GetSceneObject()
{
	return sceneObject;
}
