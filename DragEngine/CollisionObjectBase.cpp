#include "stdafx.h"
#include "CollisionObjectBase.h"
#include "PhysicsObjectBase.h"
#include "SceneObject.h"

#include <iostream>

CollisionObjectBase::CollisionObjectBase(SceneObject* object, CollisionDynamics collisionDynamics, unsigned collisionType)
{
	physicsObject = 0;
	sceneObject = object;
	collisionLayer = -1;
	this->collisionType = collisionType;
	this->collisionDynamics = collisionDynamics;
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

CollisionDynamics CollisionObjectBase::GetDynamics()
{
	return collisionDynamics;
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

PhysicsObjectBase * CollisionObjectBase::GetPhysicsObject()
{
	return physicsObject;
}
