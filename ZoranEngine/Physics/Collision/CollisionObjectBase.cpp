#include "stdafx.h"
#include "CollisionObjectBase.h"
#include "Physics/PhysicsObjectBase.h"
#include "Core/SceneObject.h"

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

void CollisionObjectBase::SetDynamics(CollisionDynamics dynamics)
{
	collisionDynamics = dynamics;
}

CollisionDynamics CollisionObjectBase::GetDynamics()
{
	return collisionDynamics;
}

SceneObject* CollisionObjectBase::GetSceneObject()
{
	return sceneObject;
}

PhysicsObjectBase * CollisionObjectBase::GetPhysicsObject()
{
	return physicsObject;
}
