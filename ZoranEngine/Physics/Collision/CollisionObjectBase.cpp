#include "stdafx.h"
#include "CollisionObjectBase.h"
#include "Physics/PhysicsObjectBase.h"
#include "Core/SceneObject.h"

#include <iostream>

CollisionObjectBase::CollisionObjectBase(CollisionDynamics collisionDynamics, unsigned collisionType) : physicsObject(0),
collisionLayer(COLLISION_LAYER_DYNAMIC), collisionType(collisionType), collisionDynamics(collisionDynamics)
{
}

CollisionObjectBase::~CollisionObjectBase()
{
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

PhysicsObjectBase * CollisionObjectBase::GetPhysicsObject()
{
	return physicsObject;
}
