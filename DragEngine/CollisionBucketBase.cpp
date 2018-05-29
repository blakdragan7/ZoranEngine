#include "stdafx.h"
#include "CollisionBucketBase.h"
#include "BoxCollisionObject.h"
#include "SceneObject.h"

CollisionBucketBase::CollisionBucketBase(Vec3D pos, Vec3D size, unsigned maxObjects)
{
	this->sceneObject = new SceneObject(NULL);
	this->sceneObject->SetPosition(pos);
	this->maxObjects = maxObjects;
	this->collision = new BoxCollisionObject(pos - (size / 2.0), pos + (size / 2.0), this->sceneObject);
}


CollisionBucketBase::~CollisionBucketBase()
{
	delete collision;
	delete sceneObject;
}
