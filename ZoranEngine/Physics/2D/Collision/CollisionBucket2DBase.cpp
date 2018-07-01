#include "stdafx.h"
#include "CollisionBucket2DBase.h"
#include <Core/2D/SceneObject2D.h>

int CollisionBucket2DBase::FindObject(CollisionObject2DBase * object)
{
	std::vector<CollisionObject2DBase*>::iterator iter = std::find(collisionObjects.begin(), collisionObjects.end(), object);
	if (iter != collisionObjects.end())
	{
		return iter - collisionObjects.begin();
	}
	else return -1;
}

CollisionBucket2DBase::CollisionBucket2DBase(std::string name, Vec2D pos, unsigned maxObjects) : CollisionBucketBase(maxObjects)
{
	this->sceneObject = new SceneObject2D(name);
	this->sceneObject->SetPosition(pos);
	this->maxObjects = maxObjects;
}

CollisionBucket2DBase::~CollisionBucket2DBase()
{
	delete sceneObject;
}

SceneObject* CollisionBucket2DBase::GetSceneObject()
{
	return sceneObject;
}
