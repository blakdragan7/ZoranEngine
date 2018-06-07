#include "stdafx.h"
#include "CollisionBucketBase.h"
#include "QuadTreeCollision.h"
#include "SceneObject.h"

int CollisionBucketBase::FindObject(CollisionObjectBase * object)
{
	std::vector<CollisionObjectBase*>::iterator iter = std::find(collisionObjects.begin(), collisionObjects.end(),object);
	if (iter != collisionObjects.end())
	{
		return iter - collisionObjects.begin();
	}
	else return -1;
}

CollisionBucketBase::CollisionBucketBase(std::string name, Vec3D pos, Vec3D size, unsigned maxObjects)
{
	this->sceneObject = new SceneObject(name);
	this->sceneObject->SetPosition(pos);
	this->maxObjects = maxObjects;
	this->collision = new QuadTreeCollision(pos - (size / 2.0), pos + (size / 2.0), this->sceneObject);
}

CollisionBucketBase::~CollisionBucketBase()
{
	delete collision;
	delete sceneObject;
}

