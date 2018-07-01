#include "stdafx.h"
#include "CollisionBucketBase.h"
#include "Core/SceneObject.h"



CollisionBucketBase::CollisionBucketBase(unsigned maxObjects)
{
	this->maxObjects = maxObjects;
}

CollisionBucketBase::~CollisionBucketBase()
{
}
