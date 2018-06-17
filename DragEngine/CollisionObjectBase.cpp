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

Vector3D CollisionObjectBase::GetPenetration(CollisionObjectBase * other,Vec3D normal)
{
	Vec3D origin = GetScenePos();
	Vec3D size = GetSize();
	Vec3D pepPoint = origin + ((size / 2.0) * -normal);

	Vec3D otherOrigin = other->GetScenePos();
	Vec3D otherSize = other->GetSize();
	Vec3D point = otherOrigin + ((otherSize / 2.0) * normal);

	Vec3D diff = (pepPoint - point).getAbs();

	Log(LogLevel_None, "point: %f,%f,%f\n", point.x, point.y, point.z);
	Log(LogLevel_None, "pepPoint: %f,%f,%f\n", pepPoint.x, pepPoint.y, pepPoint.z);
	Log(LogLevel_None, "diff: %f,%f,%f", diff.x, diff.y, diff.z);


	return normal * (diff);
}

void CollisionObjectBase::SetDynamics(CollisionDynamics dynamics)
{
	collisionDynamics = dynamics;
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
