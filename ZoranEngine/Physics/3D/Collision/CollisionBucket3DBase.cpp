#include "stdafx.h"
#include "CollisionBucket3DBase.h"
#include <Core/3D/SceneObject3D.h>

int CollisionBucket3DBase::FindObject(CollisionObject3DBase * object)
{
	std::vector<CollisionObject3DBase*>::iterator iter = std::find(collisionObjects.begin(), collisionObjects.end(), object);
	if (iter != collisionObjects.end())
	{
		return static_cast<int>(iter - collisionObjects.begin());
	}
	else return -1;
}

CollisionBucket3DBase::CollisionBucket3DBase(std::string name, Vec3D pos, unsigned maxObjects) : CollisionBucketBase(maxObjects)
{
	this->sceneObject = new SceneObject3D(name);
	this->sceneObject->SetPosition(pos);
	this->maxObjects = maxObjects;
}

CollisionBucket3DBase::~CollisionBucket3DBase()
{
	delete sceneObject;
}

SceneObject * CollisionBucket3DBase::GetSceneObject()
{
	return sceneObject;
}
