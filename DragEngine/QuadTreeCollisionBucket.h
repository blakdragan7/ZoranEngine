#pragma once

#include "CollisionBucketBase.h"

#define TL_BUCKET 0
#define TR_BUCKET 0
#define BL_BUCKET 0
#define BR_BUCKET 0


class DRAGENGINE_EXPORT QuadTreeCollisionBucket : CollisionBucketBase
{
private:
	QuadTreeCollisionBucket * parent;
	QuadTreeCollisionBucket* children[4];

	bool hasSubdivided;

private:
	void Subdivide();

public:
	QuadTreeCollisionBucket(Vec3D pos,Vec3D size,unsigned maxObjects = 32,QuadTreeCollisionBucket * parent=0);
	~QuadTreeCollisionBucket();

	void AddObject(CollisionObjectBase* object)override;
	CollisionObjectBase* RemoveObject(CollisionObjectBase* object)override;

	void CheckAllCollision()override;
	bool ObjectIsWithinBucket(CollisionObjectBase* object)override;
};

